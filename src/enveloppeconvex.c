#include "enveloppeconvex.h"

void initConvexHull(ConvexHull * conv){
    conv->curlen = 0;
    conv->maxlen = LENMAX;
    conv->pol = NULL;
}

Vertex* allouerVertex(Point *pval){
    Vertex *cell = (Vertex *)malloc (sizeof(Vertex));
    if (cell){
        cell->s = pval;
        cell->prev = NULL;
        cell->next = NULL;
    }
    return cell;
}

int estVide(ConvexHull *conv){
    if (conv->pol)
        return 0;
    return 1;
}

int enfileConvex(ConvexHull *conv, Point *pval){
    Polygon nouveau = allouerVertex(pval);
    if (nouveau){
        if (estVide(conv)){
        conv->pol = nouveau;
        nouveau->next = nouveau;
        nouveau->prev = nouveau;
        }
        else{
            nouveau->next = conv->pol->next;
            nouveau->prev = conv->pol->prev;
            conv->pol = nouveau;
        }
    return 1;
    }
    return 0;
}

int test_triangle_Indirect(Point p, Point B, Point C){
    if ((B.x - p.x) * (C.y - p.y) - (C.x - p.x) * (B.y - p.y) < 0)
        return 1; 

    return 0;
}

int testNotInConvex(ConvexHull *conv, Point point){
    Polygon poly = conv->pol;
    for (int n = 0;n <= conv->curlen; n++){
        if (test_triangle_Indirect(point, *(poly->s), *(poly->next->s))){
            return 1;
        }
        poly = poly->next;

    }
    return 0;
}