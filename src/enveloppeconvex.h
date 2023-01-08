#ifndef _ENVELOPPECONVEX_H_
#define _ENVELOPPECONVEX_H_
#include "listepoints.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct _vrtx_ {
    Point *s;
    struct _vrtx_ *prev;
    struct _vrtx_ *next;
} Vertex, *Polygon;

typedef struct {
    Polygon pol;
    int curlen;
    int maxlen;
} ConvexHull;

void initConvexHull(ConvexHull * conv);
Vertex* allouerVertex(Point *pval);
int estVide(ConvexHull *conv);
int enfileConvex(ConvexHull *conv, Point *pval);
void parcoursConvex(ConvexHull conv);
int insertConvex(Vertex *maillon, Point *pval, ConvexHull *conv);
int test_triangle_Indirect(Point p, Point B, Point C);
void suppresionVertex(Vertex* suppresion, ConvexHull *conv);
void nettoyageAvant(Vertex* origine, ConvexHull *conv);
void nettoyageArriere(Vertex* origine, ConvexHull *conv);
int testNotInConvex(ConvexHull *conv, Point *point);

#endif
