#include "etape1.h"

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