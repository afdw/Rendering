#ifndef UTILS_H
#define UTILS_H

#define structMember(type, member) (((type *) 0)->member)
#define getArrayElementsCount(array) (sizeof(array) / sizeof((array)[0]))

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define sign(n) ((n) == 0 ? 0 : ((n) > 0 ? 1 : -1))
#define abs(n) ((n) >= 0 ? (n) : (-n))
#define clamp(n, min, max) ((n) < (min) ? (min) : (n) > (max) ? (max) : (n))
#define toRadians(degrees) ((degrees) * M_PI / 180.0)
#define toDegrees(radians) ((radians) * 180.0 / M_PI)

#endif
