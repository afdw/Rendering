#ifndef VECTOR_2I_H
#define VECTOR_2I_H

#include "VectorTypes.h"

typedef struct Vector2I {
    I x;
    I y;
} Vector2I;

Vector2I vector2INew(I x, I y);

Vector2I vector2IAdd(Vector2I a, Vector2I b);

Vector2I vector2ISubtract(Vector2I a, Vector2I b);

#endif
