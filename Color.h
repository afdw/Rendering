#ifndef COLOR_H

typedef float ColorComponent;

typedef struct Color {
    ColorComponent r;
    ColorComponent g;
    ColorComponent b;
    ColorComponent a;
} Color;

Color colorNew(ColorComponent r, ColorComponent g, ColorComponent b, ColorComponent a);

#define COLOR_H
#endif
