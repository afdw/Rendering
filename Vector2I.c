#include "Vector2I.h"

Vector2I vector2INew(I x, I y) {
    Vector2I vector = {.x = x, .y = y};
    return vector;
}

Vector2I vector2IAdd(Vector2I a, Vector2I b) {
    return vector2INew(a.x + b.x, a.y + b.y);
}

Vector2I vector2ISubtract(Vector2I a, Vector2I b) {
    return vector2INew(a.x - b.x, a.y - b.y);
}

Vector2I vector2IOpposite(Vector2I vector) {
    return vector2INew(-vector.x, -vector.y);
}
