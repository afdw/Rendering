#ifndef COLOR_H

typedef float ColorComponent;

typedef struct Color {
    ColorComponent r;
    ColorComponent g;
    ColorComponent b;
    ColorComponent a;
} Color;

Color colorNew(ColorComponent r, ColorComponent g, ColorComponent b, ColorComponent a);

Color colorAdd(Color a, Color b);

Color colorSubstract(Color a, Color b);

Color colorMultiply(Color color, ColorComponent n);

#define COLOR_H
#endif
