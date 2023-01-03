#ifndef _LISTEPOINTS_H_
#define _LISTEPOINTS_H_
#include <stdlib.h>
#include <stdio.h>

#define LENMAX 50

typedef struct{
    int x;
    int y;
} Point;

typedef struct cellulePoint {
    Point p;
    struct cellulePoint *suivant;
} CellulePoint, *ListePoint;

CellulePoint* allouerCellule(Point pval);
int insererTetePoint(ListePoint *liste, Point val);
void affichageLstPoint(ListePoint liste);

#endif