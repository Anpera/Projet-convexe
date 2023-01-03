#include "etape1.h"

void dessineLstPoint(ListePoint l){
    for(;l;l = l->suivant){
        MLV_draw_filled_circle(l->p.x, l->p.y, 4, MLV_COLOR_WHITE);
    }    
}

void dessineConvexe(ConvexHull conv){
    Polygon cell = conv.pol;
    for (int i = 0; i<conv.curlen; i++, cell = cell->next){
        MLV_draw_line(cell->s->x, cell->s->y, 
                    cell->next->s->x, cell->next->s->y, 
                    MLV_COLOR_ORCHID1);
    }

}

int entrerPolygone(ListePoint *liste, ConvexHull *conv){
    
    Point souris;

    MLV_Event event = MLV_wait_keyboard_or_mouse(NULL, NULL, NULL, &souris.x, &souris.y);

    while (event!= MLV_KEY) {
        MLV_clear_window(MLV_COLOR_BLACK);
        
        insererTetePoint(liste, souris);
        testNotInConvex(conv, &(*liste)->p);

        dessineLstPoint(*liste);
        dessineConvexe(*conv);

        MLV_actualise_window();
        event = MLV_wait_keyboard_or_mouse(NULL, NULL, NULL, &souris.x, &souris.y);
    };
    return 1;
}

int polyAleaCarre(ListePoint *liste, ConvexHull *conv, int centre_X, int centre_Y, int rayon){
    Point souris;

    for (int n = 0; n<=200; n++) {

        MLV_clear_window(MLV_COLOR_BLACK);

        souris.x = MLV_get_random_integer(centre_X-rayon, centre_X+rayon);
        souris.y = MLV_get_random_integer(centre_Y-rayon, centre_Y+rayon);

        insererTetePoint(liste, souris);
        testNotInConvex(conv, &(*liste)->p);

        dessineLstPoint(*liste);
        dessineConvexe(*conv);

        MLV_actualise_window();

        MLV_wait_milliseconds(10);
    }

    return 1;
}

int polyAleaCercle(ListePoint *liste, ConvexHull *conv, int centre_X, int centre_Y, int rayon){
    Point souris;
    double tempx; 
    double tempy;
    double po;
    double r;

    for (int n = 0; n<=200; n++){

        MLV_clear_window(MLV_COLOR_BLACK);

        po = MLV_get_random_double(0,1) * 2 * PI;
        r = rayon * sqrt(MLV_get_random_double(0,1));
        // Pour cercle qui augmente petit à petit, changer le rayon ici

        tempx = r * cos(po);
        tempy = r * sin(po);

        souris.x = (int) centre_X + tempx;
        souris.y = (int) centre_Y + tempy;

        insererTetePoint(liste, souris);
        testNotInConvex(conv, &(*liste)->p);

        dessineLstPoint(*liste);
        dessineConvexe(*conv);

        MLV_actualise_window();

        MLV_wait_milliseconds(50);
    }
    return 1;
}

void test(ConvexHull *conv, ListePoint *lst_p){
    Point souris;
    initConvexHull(conv);

    for (int x = 0; x < 10; x++){
        souris.x = x, souris.y = 10-x;
        // Insère un point en tête de la liste de points
        insererTetePoint(lst_p,souris);
        /*
        Insère un point à la fin de la liste des
        points de l'enveloppe convexe 
        */ 
        enfileConvex(conv, &(*lst_p)->p);
    }
    printf("Affichage de la liste de points :\n");
    affichageLstPoint(*lst_p);
    printf("Affichage de la liste des points de l'enveloppe :\n");
    parcoursConvex(*conv);

    souris.x = 35, souris.y = 35;
    insererTetePoint(lst_p,souris);
    insertConvex(conv->pol->prev, &(*lst_p)->p, conv);
    conv->curlen++;
    insertConvex(conv->pol->next->next, &(*lst_p)->p, conv);
    conv->curlen++;
    printf("Affichage de la liste des points de l'enveloppe :\n");
    parcoursConvex(*conv);
}

int main(){
    ListePoint lst_p = NULL;
    ConvexHull conv;
    initConvexHull(&conv);

    //test(&conv, &lst_p);
    MLV_create_window("listechaine","", TAILLE_X, TAILLE_Y);
    

    //entrerPolygone(&lst_p, &conv);

    //polyAleaCarre(&lst_p, &conv, TAILLE_X/2, TAILLE_Y/2, TAILLE_X/3);

    polyAleaCercle(&lst_p, &conv, TAILLE_X/2, TAILLE_Y/2, TAILLE_X/3);

    // for (int i = 0; i <= 20; i++){
    //     lst_p = NULL;
    //     initConvexHull(&conv);
    //     polyAleaCarre(&lst_p, &conv, TAILLE_X/2, TAILLE_Y/2, TAILLE_X/3);

    //     MLV_wait_milliseconds(100);
    // }

    MLV_wait_keyboard_or_mouse_or_seconds(NULL, NULL, NULL, NULL, NULL, 30);
    MLV_free_window();
    return 0;
}