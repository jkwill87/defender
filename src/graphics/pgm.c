/**
 * pgm.c
 *
 * Logic for parsing and transforming map data from PGM files.
 */

#include <ctype.h>
#include <unistd.h>
#include "debug.h"

#define _PATH_BUFFER 100


// External Variables ----------------------------------------------------------

extern Pgm terrain;
extern World world_terrain;


// Module globals --------------------------------------------------------------

static char *current_char;
static char *last_char;
static char pgm_path[_PATH_BUFFER] = {'\0'};


// Private Function Definitions ------------------------------------------------

bool _check_path(const char *prefix, const char *file_name) {
    snprintf(pgm_path, _PATH_BUFFER, "%s%s", prefix, file_name);
    return access(pgm_path, F_OK) != -1;
}

bool _find_file(const char *file_name) {
    if (_check_path("./", file_name)) return true;
    if (_check_path("./assets/", file_name)) return true;
    if (_check_path("../assets/", file_name)) return true;
    return false;
}

static char *_load_file(const char *filename) {
    // open file for reading
    _find_file(filename);
    FILE *file_handle = fopen(pgm_path, "rb");
    if (!file_handle) return NULL;
    // determine file length
    fseek(file_handle, 0, SEEK_END);
    size_t end_position = (size_t) ftell(file_handle);
    // attempt to read file data into buffer
    char *buffer = current_char = calloc(end_position, sizeof(char));
    if (buffer) {
        fseek(file_handle, 0, SEEK_SET);
        size_t read_status = fread(buffer, 1, end_position, file_handle);
        assert_ok(
            read_status != 0,
            "could not read pgm data"
        );
        last_char = buffer + end_position;
    }
    fclose(file_handle);
    return buffer;
}

static bool _is_eof() {
    return current_char == last_char || *current_char == '\0';
}

static void _skip_line() {
    while (!_is_eof() && *(current_char++) != '\n');
}

static void _skip_comment_line() {
    while (*current_char == '#') _skip_line();
}

static void _skip_whitespace() {
    while (!_is_eof() && isblank(*current_char)) current_char++;
}

static unsigned _get_next_number(unsigned max) {
    _skip_comment_line();
    _skip_whitespace();
    char n[PGM_MAX_DIGITS] = {'\0'};
    char c = '\0';
    for (int i = 0; i < PGM_MAX_DIGITS; i++) {
        c = *(current_char++);
        if (!isdigit(c)) break;
        n[i] = c;
    }
    long rval = atol(n);
    assert_ok(c, "no number to parse");
    assert_ok(!isdigit(c), "parsed value exceeds PGM_MAX_DIGITS");
    assert_ok(rval >= 0, "value could not be parsed");
    assert_ok(!max || rval <= max, "parsed value out of range");
    return (unsigned) rval;
}

static void _clear_terrain() {
    // clearout any existing cubes
    #pragma omp parallel for collapse(3)
    for (byte x = 0; x < WORLD_XZ; x++)
        for (byte y = 0; y < WORLD_Y; y++)
            for (byte z = 0; z < WORLD_XZ; z++)
                world_terrain[x][y][z] = 0;
}

static bool _is_floating_block(byte x, byte y, byte z) {
    byte pts = 0;
    // block directly below
    if (world_terrain[x][y - 1][z]) pts += 4;
    // blocks below
    if (y - 1 >= 0 && world_terrain[x][y - 1][z])
        pts += 3;
    else if (y - 2 >= 0 && world_terrain[x][y - 2][z])
        pts += 2;
    else if (y - 3 >= 0 && world_terrain[x][y - 3][z])
        pts += 1;
    // blocks underneath and offset
    if (x - 1 >= 0 && world_terrain[x - 1][y - 1][z]) pts += 4;
    if (x + 1 < WORLD_XZ && world_terrain[x + 1][y - 1][z]) pts += 4;
    if (z - 1 >= 0 && world_terrain[x][y - 1][z - 1]) pts += 4;
    if (z + 1 < WORLD_XZ && world_terrain[x][y - 1][z + 1]) pts += 4;
    // blocks underneath diagonally
    if (x - 1 >= 0 && z - 1 >= 0 && world_terrain[x - 1][y - 1][z - 1])
        pts += 3;
    if (x - 1 >= 0 && z + 1 < WORLD_XZ && world_terrain[x - 1][y - 1][z + 1])
        pts += 3;
    if (x + 1 < WORLD_XZ && z + 1 < WORLD_XZ &&
        world_terrain[x + 1][y - 1][z + 1])
        pts += 3;
    if (x + 1 < WORLD_XZ && z - 1 >= 0 && world_terrain[x + 1][y - 1][z - 1])
        pts += 3;
    // blocks adjacent to
    if ((x - 1 > 0 && world_terrain[x - 1][y][z]) ||
        (z - 1 > 0 && world_terrain[x][y][z - 1]) ||
        (z + 1 < WORLD_XZ && world_terrain[x][y][z + 1]) ||
        (x + 1 < WORLD_XZ && world_terrain[x + 1][y][z]))
        pts += 1;
    return pts < 4;
}

static void _settle_cubes() {
    // drop cubes without neighbors to minimize gaps
    bool retry;
    do {
        retry = false;
        #pragma omp parallel for
        for (byte x = 0; x < WORLD_XZ; x++) {
            for (byte y = WORLD_Y - 1; y > 1; y--) {
                for (byte z = 0; z < WORLD_XZ; z++) {
                    if (world_terrain[x][y][z] == 0) continue;
                    if (!_is_floating_block(x, y, z)) continue;
                    world_terrain[x][y][z] = COLOUR_NONE;
                    world_terrain[x][y - 1][z] = COLOUR_BLACK;
                    retry = true;
                }
            }
        }
    } while (retry);
}

static void _add_base_layer() {
    // add plane of cubes along bottom border
    #pragma omp parallel for collapse(2)
    for (int x = 0; x < WORLD_XZ; x++)
        for (int z = 0; z < WORLD_XZ; z++)
            if (!world_terrain[x][1][z])
                world_terrain[x][0][z] = COLOUR_BLACK;
}

static void _cull_overlapping_cubes() {
    // limit 1 cube to x/z coordinate
    bool ceil;
    #pragma omp parallel for collapse(2)
    for (int x = 0; x < WORLD_XZ; x++) {
        for (int z = 0; z < WORLD_XZ; z++) {
            ceil = false;
            for (int y = WORLD_Y - 1; y >= 0; y--) {
                if (world_terrain[x][y][z] != COLOUR_BLACK)
                    continue;
                else if (!ceil)
                    ceil = true;
                else
                    world_terrain[x][y][z] = COLOUR_NONE;
            }
        }
    }
}


// Public Function Definitions -------------------------------------------------

void pgm_init(const char *filename) {
    // load file
    char *buffer = _load_file(filename);
    assert_ok(buffer, "no data loaded from file");
    // parse dimensions
    _skip_line();
    unsigned x = _get_next_number(0);
    unsigned z = _get_next_number(0);
    unsigned y = _get_next_number(0);
    // initialize
    terrain.z = z;
    terrain.x = x;
    terrain.y = y;
    // parse data
    unsigned datum_counter = 0;
    do {
        terrain.data[datum_counter++] = _get_next_number(y);
    } while (datum_counter < z * x);
    // verify eof
    _skip_whitespace();
    assert_ok(_is_eof(), "unexpected data at end of file");
    free(buffer);
}

unsigned pgm_get_y_value(double x, double z) {
    int i = (int) (z * terrain.z + x);
    assert_lt(x, terrain.x, "x value out of range");
    assert_lt(z, terrain.z, "z value out of range");
    assert_lt(i, terrain.x * terrain.z, "index out of range");
    return (unsigned) terrain.data[i];
}

unsigned pgm_calc_ceil() {
    float ceil = 0;
    for (int i = 0; i < terrain.x * terrain.z; i++) {
        if (terrain.data[i] > ceil) {
            ceil = terrain.data[i];
        }
    }
    ceil += ceil * 0.1f;
    assert_gte(ceil, 0.0f, "ceil underflow imminent");
    return (unsigned) ceil;
}

void pgm_set_world_terrain() {
    _clear_terrain();
    unsigned y_max = pgm_calc_ceil();
    double x_scale = (terrain.x - 1) / (WORLD_XZ - 1.0);
    double y_scale = (y_max - 1) / (WORLD_Y - 1.0);
    double z_scale = (terrain.z - 1) / (WORLD_XZ - 1.0);
    double sx, sy, sz;
    byte y;
    // nearest neighbour interpolation
    #pragma omp parallel for
    for (byte x = 0; x < WORLD_XZ; x++) {
        for (byte z = 0; z < WORLD_XZ; z++) {
            sx = x * x_scale;
            assert_gte(sx, 0.0f, "sx value out of range");
            assert_lt(sx, terrain.x, "sx value out of range");
            sz = z * z_scale;
            assert_gte(sz, 0.0f, "sz value out of range");
            assert_lt(sz, terrain.z, "sz value out of range");
            sy = pgm_get_y_value(sx, sz) / y_scale;
            assert_gte(sy, 0.0f, "sy value out of range");
            assert_lt(sy, WORLD_Y, "sy value out of range");
            y = (byte) sy;
            world_terrain[x][y][z] = COLOUR_BLACK;
        }
    }
    // normalize units
    _settle_cubes();
    _cull_overlapping_cubes();
    _add_base_layer();
}
