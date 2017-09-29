#include <stdio.h>

static inline void v3_add(V3 c, V3 a, V3 b);
static inline void v3_sub(V3 c, V3 a, V3 b);
static inline void v3_scale(V3 c, V3 a, double b);
static inline double v3_dot(V3 a, V3 b);
static inline void v3_cross(V3 c, V3 a, V3 b);
