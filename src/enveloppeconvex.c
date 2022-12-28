#include "enveloppeconvex.h"

void initConvexHull(ConvexHull * conv){
    conv->curlen = 0;
    conv->maxlen = LENMAX;
    conv->pol = NULL;
}

Vertex* allouerVertex(Point *pval){
    /*
    Alloue la mémoire nécessaire pour une cellule
    de l'enveloppe convexe
    ET y place l'adresse d'un point
    */
    Vertex *cell = (Vertex *)malloc (sizeof(Vertex));
    if (cell){
        cell->s = pval;
        cell->prev = NULL;
        cell->next = NULL;
    }
    return cell;
}

int estVide(ConvexHull *conv){
    /* Test si l'enveloppe est vide.
    1 si vide
    0 sinon
    */
    if (conv->pol)
        return 0;
    return 1;
}

int enfileConvex(ConvexHull *conv, Point *pval){
    // Ajoute à la fin une adresse de point à l'enveloppe Convex
    Vertex* nouveau = allouerVertex(pval);
    if (nouveau){
        if (estVide(conv)){
        nouveau->next = nouveau;
        nouveau->prev = nouveau;
        conv->pol = nouveau;
        conv->curlen = 1;
        }
        else{
            nouveau->next = conv->pol;
            nouveau->prev = conv->pol->prev;

            conv->pol->prev->next = nouveau;
            conv->pol->prev = nouveau;

            conv->curlen++;
        }
        return 1;
    }
    return 0;
}

void parcoursConvex(ConvexHull conv){
    /*
    Affichage de chaque point dans l'enveloppe mais
    pourra facilement servir pour faire un polygone
    */
    Polygon cell = conv.pol;
    for (int i = 0; i<conv.curlen; i++, cell = cell->next){
        printf("%d %d\n", cell->s->x, cell->s->y);
    }
}

// int test_triangle_Indirect(Point p, Point B, Point C){
//     if ((B.x - p.x) * (C.y - p.y) - (C.x - p.x) * (B.y - p.y) < 0)
//         return 1; 

//     return 0;
// }

// int testNotInConvex(ConvexHull *conv, Point point){
//     Polygon poly = conv->pol;
//     for (int n = 0;n <= conv->curlen; n++){
//         if (test_triangle_Indirect(point, *(poly->s), *(poly->next->s))){
//             return 1;
//         }
//         poly = poly->next;

//     }
//     return 0;
// }