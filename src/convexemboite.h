#ifndef _CONVEXEMBOITE_H_
#define _CONVEXEMBOITE_H_
#include "listepoints.h"
#include "enveloppeconvex.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct CelluleConvex {
    ConvexHull conv;
    struct CelluleConvex *suivant;
} CelluleConvex, *ListeConvex;

CelluleConvex* alloueConvex();
int enfileLstConvex(ListeConvex *l);
void printLstConvex(ListeConvex l);
void suppresionVertexLST(Vertex* suppression, ListeConvex *lstConv);
void nettoyageAvantLST(Vertex* origine, ListeConvex *conv);
void nettoyageArriereLST(Vertex* origine, ListeConvex *conv);
int testInConvexLST(ListeConvex *lstConv, Point *point);

#endif