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

int entrerPolygone(ListePoint *liste, ListeConvex *conv, int emboite){
    
    Point souris; 

    MLV_Event event = MLV_wait_keyboard_or_mouse(NULL, NULL, NULL, &souris.x, &souris.y);

    while (event!= MLV_KEY) {
        MLV_clear_window(MLV_COLOR_BLACK);
        
        insererTetePoint(liste, souris);
        dessineLstPoint(*liste);
        if (emboite){
            testInConvexLST(conv, &(*liste)->p);
            for (CelluleConvex *index = *conv; index; index=index->suivant)
                dessineConvexe(index->conv);
                }
        else{
            testNotInConvex(&(*conv)->conv, &(*liste)->p);
            dessineConvexe((*conv)->conv);
        }

        MLV_actualise_window();
        event = MLV_wait_keyboard_or_mouse(NULL, NULL, NULL, &souris.x, &souris.y);
    };
    return 1;
}

int polyAleaCarre(ListePoint *liste, ListeConvex *conv, int centre_X, int centre_Y, int rayon, int nbpoints, int emboite){
    Point souris;
    int tempx, tempy;
    double pas = (double) rayon/nbpoints;
    double r;

    for (int n = 1; n<=nbpoints; n++) {
        MLV_clear_window(MLV_COLOR_BLACK);
        if (n<10 && nbpoints >= 200)
            r = n+1;
        else
            r = n * pas;

        tempx = MLV_get_random_double(centre_X-r, centre_X+r) * MLV_get_random_integer(0,2);
        if (tempx == 0){
            tempx = centre_X + (r * pow(-1, MLV_get_random_integer(0,2)));
            tempy = MLV_get_random_integer(centre_Y-r, centre_Y+r);
        }

        else{
            tempy = centre_Y + r * (pow(-1, MLV_get_random_integer(0,2)));
        }

        if (tempx < 0)
            tempx = 0;
        else if (tempx > TAILLE_X)
            tempx = TAILLE_X;
        if (tempy < 0)
            tempy = 0;
        else if (tempy > TAILLE_Y)
            tempy = TAILLE_Y;

        souris.x = tempx;
        souris.y = tempy;

        insererTetePoint(liste, souris);
        dessineLstPoint(*liste);

        if (emboite){
            testInConvexLST(conv, &(*liste)->p);
            for (CelluleConvex *index = *conv; index; index=index->suivant)
            dessineConvexe(index->conv);
            }
        else{
            testNotInConvex(&(*conv)->conv, &(*liste)->p);
            dessineConvexe((*conv)->conv);
            }

        MLV_actualise_window();

        MLV_wait_milliseconds(10);
    }

    return 1;
}

int polyAleaCercle(ListePoint *liste, ListeConvex *conv, int centre_X, int centre_Y, int rayon, int nbpoints, int emboite){
    Point souris;
    double tempx; 
    double tempy;
    double point;
    double r;
    double pas = (double) rayon/nbpoints;

    for (int n = 1; n<=nbpoints; n++){

        MLV_clear_window(MLV_COLOR_BLACK);

        point = MLV_get_random_double(0,1) * 2 * PI;
        //r = rayon * sqrt(MLV_get_random_double(0,1));
        if (n<10 && nbpoints >= 200)
            r = n+1;
        else
            r = n * pas;
        // if (r<10)
        //     r = n * sqrt(MLV_get_random_double(0,1));
        // Pour cercle qui augmente petit à petit, changer le rayon ici pour une valeur fixe

        tempx = r * cos(point);
        tempy = r * sin(point);

        souris.x = (int) centre_X + tempx;
        souris.y = (int) centre_Y + tempy;

        if (souris.x < 0)
            souris.x = 0;
        else if (souris.x > TAILLE_X)
            souris.x = TAILLE_X;
        if (souris.y < 0)
            souris.y = 0;
        else if (souris.y > TAILLE_Y)
            souris.y = TAILLE_Y;

        insererTetePoint(liste, souris);
        dessineLstPoint(*liste);

        if (emboite){
            testInConvexLST(conv, &(*liste)->p);
            for (CelluleConvex *index = *conv; index; index=index->suivant)
            dessineConvexe(index->conv);
            }
        else{
            testNotInConvex(&(*conv)->conv, &(*liste)->p);
            dessineConvexe((*conv)->conv);
            }
        MLV_actualise_window();

        

        MLV_wait_milliseconds(10);
    }

    
    return 1;
}
int Choixfigure(int figure){
    ListePoint lst_p = NULL;
    ListeConvex lstConv= alloueConvex();
    Point souris;
    if (figure == 1 || figure == 2 || figure == 3 || figure == 4){
        MLV_wait_mouse(&souris.x, &souris.y);
    }
    
    switch (figure)
    {
    case 1:
        polyAleaCarre(&lst_p, &lstConv, souris.x, souris.y, TAILLE_X/3, NBRPOINTS, 0);
        break;
    
    case 2:
        polyAleaCarre(&lst_p, &lstConv, souris.x, souris.y, TAILLE_X/3, 250, 1);
        break;

    case 3:
        polyAleaCercle(&lst_p, &lstConv, souris.x, souris.y, TAILLE_X/3, NBRPOINTS, 0);
        break;
    
    case 4:
        polyAleaCercle(&lst_p, &lstConv, souris.x, souris.y, TAILLE_X/3, 50, 1);
        break;

    case 5:
        entrerPolygone(&lst_p, &lstConv, 1);

    default:
        entrerPolygone(&lst_p, &lstConv, 0);
        break;
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

    //test(&conv, &lst_p);
    MLV_create_window("listechaine","", TAILLE_X, TAILLE_Y);

    //PolyLstConvex(&lst_p, &lstConv);
    
    Choixfigure(2);

    MLV_wait_keyboard_or_mouse_or_seconds(NULL, NULL, NULL, NULL, NULL, 30);
    MLV_free_window();
    return 0;
}