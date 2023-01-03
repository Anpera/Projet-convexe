#include "enveloppeconvex.h"
#include <MLV/MLV_all.h>
#include <stdio.h>

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
    // Ajoute à la tête une adresse de point à l'enveloppe Convex
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

int insertConvex(Vertex *maillon, Point *pval, ConvexHull *conv){
    Vertex* nouveau = allouerVertex(pval);
    if (nouveau){
        nouveau->next = maillon->next;
        nouveau->prev = maillon;

        maillon->next->prev = nouveau;
        maillon->next = nouveau;

        conv->pol = nouveau;
        conv->curlen++;
        return 1;
    }
    return 0;
}

int test_triangle_Indirect(Point A, Point B, Point C){
    if (((B.x - A.x) * (C.y - A.y)) - ((C.x - A.x) * (B.y - A.y)) < 0){
        return 1;
    }
    return 0;
        
}

void suppresionVertex(Vertex* suppression, ConvexHull *conv){
    if (conv->curlen >3){
        if (suppression == conv->pol){
            printf("entree changé\n");
            conv->pol = conv->pol->next;
        }
        suppression->next->prev = suppression->prev;
        suppression->prev->next = suppression->next;

        free(suppression);
        conv->curlen--;
    }
}

void nettoyageAvant(Vertex* origine, ConvexHull *conv){   
    for (int i = 0; i <= conv->curlen; i++){ 
        if (test_triangle_Indirect(*origine->s, *(origine->next)->s, *(origine->next->next)->s))
            suppresionVertex(origine->next, conv);
    }
}

void nettoyageArriere(Vertex* origine, ConvexHull *conv){
    for (int i = 0; i <= conv->curlen; i++){
        if(test_triangle_Indirect(*origine->s, *(origine->prev->prev)->s, *(origine->prev)->s))
            suppresionVertex(origine->prev, conv);
    }
}

int testNotInConvex(ConvexHull *conv, Point *point){
    Polygon cell = conv->pol;
    Point A;
    Point B;
    Point C;

    if (conv->curlen < 2){
        if (enfileConvex(conv, point)){
            return 1;
        }
    }
    else if (conv->curlen == 2){
        A = *point;
        B = *(conv->pol->s);
        C = *(conv->pol->next->s);
        if (test_triangle_Indirect(A, B, C)){
            if (insertConvex(conv->pol, point, conv)){
                return 1;
            }
            else{
                return 0;
            }
        }
        else{
            if(enfileConvex(conv, point))
            return 1;
        }

    }
    else{
        for (int i = 0; i<conv->curlen; i++, cell = cell->next){
            A = *point;
            B = *(cell->s);
            C = *(cell->next->s);
            if (test_triangle_Indirect(A,B,C)){
                
                // MLV_draw_filled_circle(point->x, point->y, 10, MLV_COLOR_PURPLE1);
                // MLV_draw_filled_circle(cell->s->x, cell->s->y, 10, MLV_COLOR_CYAN1);
                // MLV_draw_filled_circle(cell->next->s->x, cell->next->s->y, 10, MLV_COLOR_GREEN1);

                if(insertConvex(cell, point, conv)){
                        nettoyageArriere(cell->next, conv);
                        nettoyageAvant(cell->next, conv);
                        return 1;
                }
            }
        }
    }
    return 0;
}
