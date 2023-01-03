#ifndef _ETAPE1_H_
#define _ETAPE1_H_
#include <MLV/MLV_all.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "listepoints.h"
#include "enveloppeconvex.h"

#define PI (3.141592653589793)
#define TAILLE_X 512
#define TAILLE_Y 512

void dessineLstPoint(ListePoint l);
int entrerPolygone(ListePoint *liste, ConvexHull *conv);
int polyAleaCarre(ListePoint *liste, ConvexHull *conv, int centre_X, int centre_Y, int taille);
int polyAleaCercle(ListePoint *liste, ConvexHull *conv, int centre_X, int centre_Y, int rayon);
void test(ConvexHull *conv, ListePoint *lst_p);

#endif