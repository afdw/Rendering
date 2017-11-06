#include "Color.h"

Color colorNew(ColorComponent r, ColorComponent g, ColorComponent b, ColorComponent a) {
    Color color = {.r = r, .g = g, .b = b, .a = a};
    return color;
}
