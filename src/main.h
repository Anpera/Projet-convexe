#ifndef _MAIN_H_
#define _MAIN_H_
#include <MLV/MLV_all.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "listepoints.h"
#include "enveloppeconvex.h"
#include "convexemboite.h"

#define PI (3.141592653589793)
#define TAILLE_X 600
#define TAILLE_Y 600
#define NBRPOINTS 800

void dessinePoint(Point p);
void dessineLstPoint(ListePoint l);
void dessineConvexe(ConvexHull conv);
void choixcouleur(int nbrCol, MLV_Color * poly, MLV_Color * point);
void dessineLstConvex(ListeConvex lst);
int entrerPolygone(ListePoint *liste, ListeConvex *conv, int emboite);
int polyAleaCarre(ListePoint *liste, ListeConvex *conv, int centre_X, int centre_Y, int rayon, int nbpoints, int emboite);
int polyAleaCercle(ListePoint *liste, ListeConvex *conv, int centre_X, int centre_Y, int rayon, int nbpoints, int emboite);
int trouverayon(Point origine, int figure);
int choixfigure(int figure);
int menu();

#endif