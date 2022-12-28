#ifndef _ENVELOPPECONVEX_H_
#define _ENVELOPPECONVEX_H_
#include "Listepoints.h"

typedef struct _vrtx_{
    Point *s;
    struct _vrtx_ *prev;
    struct _vrtx_ *next;
} Vertex, *Polygon;

typedef struct{
    Polygon pol;
    int curlen;
    int maxlen;
} ConvexHull;

#endif
