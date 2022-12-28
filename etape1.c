#include <MLV/MLV_all.h>
#include <stdlib.h>
#include "enveloppeconvex.h"
#include "Listepoints.h"

CellulePoint* allouerCellule(Point pval){
    CellulePoint* cell = (CellulePoint *)malloc (sizeof(CellulePoint));
    if (cell!=NULL){
        cell->p = pval;
        cell->suivant = NULL;
    }
    return cell;
}

int insererTetePoint(ListePoint *liste, Point val){
    ListePoint temp = allouerCellule(val);
    if (temp != NULL){
        temp->suivant = *liste;
        *liste = temp;
        return 1;
    }
    return 0;
}

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
            printf("GG\n");
            return 1;
        }
        poly = poly->next;

    }
    return 0;
}

int entrerPolygone(ListePoint *liste){
    MLV_Event event;
    Point souris;
    ListePoint premier = NULL;

    do {
        event = MLV_wait_keyboard_or_mouse(NULL, NULL, NULL, &souris.x, &souris.y);
        if (*liste){
            MLV_draw_line( (*liste)->p.x, (*liste)->p.y, souris.x, souris.y, MLV_COLOR_ORCHID1);
        }
        else if (!premier){
            insererTetePoint(liste, souris);
            premier = (*liste);
        }
        if (!insererTetePoint(liste, souris)){
            return 0;
        };
        MLV_draw_filled_circle(souris.x, souris.y, 2,MLV_COLOR_WHITE);
        MLV_actualise_window();
    } while (event != MLV_KEY);
    MLV_draw_line( (*liste)->p.x, (*liste)->p.y, (*premier).p.x, (*premier).p.y, MLV_COLOR_ORCHID1);
    MLV_actualise_window();
    return 1;
}





void dessineLstPoint(ListePoint l){
    for(;l;l = l->suivant){
        MLV_draw_filled_circle(l->p.x, l->p.y, 4, MLV_COLOR_WHITE);
    }    
}


int main(){
    ListePoint lst_p = NULL;
    ConvexHull conv;
    initConvexHull(&conv);
    MLV_create_window("listechaine","", TAILLE_X, TAILLE_Y);



    entrerPolygone(&lst_p);

    MLV_clear_window(MLV_COLOR_BLACK);

    
    

    MLV_wait_mouse_or_seconds(NULL, NULL, 30);
    MLV_free_window();
    return 0;
}