#include "graphics.h"

#define _m(m, r, b, g, a) m[0]=r;m[1]=b;m[2]=g;m[3]=a

const Material *get_material(Colour colour) {
    return get_material_a(colour, 1.0f);
}

const Material *get_material_a(Colour colour, float alpha) {
    static Material material;
    switch (colour) {
        default:
        case RED:
        _m(material, 0.5f, 0.0f, 0.0f, alpha);
            break;
        case BLUE:
        _m(material, 0.0f, 0.0f, 0.5f, alpha);
            break;
        case GREEN:
        _m(material, 0.0f, 0.5f, 0.0f, alpha);
            break;
        case YELLOW:
        _m(material, 0.75f, 0.75f, 0.0f, alpha);
            break;
        case ORANGE:
        _m(material, 0.5f, 0.32f, 0.0f, alpha);
            break;
        case BLACK:
        _m(material, 0.0f, 0.0f, 0.0f, alpha);
            break;
        case DARK:
        _m(material, 0.125f, 0.125f, 0.125f, alpha);
            break;
        case MEDIUM:
        _m(material, 0.5f, 0.5f, 0.5f, alpha);
            break;
        case LIGHT:
        _m(material, 0.7f, 0.7f, 0.7f, alpha);
            break;
        case WHITE:
        _m(material, 1.0f, 1.0f, 1.0f, alpha);
            break;
    }
    return &material;
}

