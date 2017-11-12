#include "Color.h"

Color colorNew(ColorComponent r, ColorComponent g, ColorComponent b, ColorComponent a) {
    Color color = {.r = r, .g = g, .b = b, .a = a};
    return color;
}

Color colorAdd(Color a, Color b) {
    return colorNew(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
}

Color colorSubtract(Color a, Color b) {
    return colorNew(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);
}

Color colorMultiply(Color color, ColorComponent n) {
    return colorNew(color.r * n, color.g * n, color.b * n, color.a * n);
}
