#include "main.h"

void dessinePoint(Point p){
    MLV_draw_filled_circle(p.x, p.y, 4, MLV_COLOR_WHITE);
    MLV_actualise_window();
    MLV_clear_window(MLV_COLOR_BLACK);
}

void dessineLstPoint(ListePoint l) {
    /*
    Dessine la liste de points
    */
    for(; l; l = l->suivant)
        MLV_draw_filled_circle(l->p.x, l->p.y, 4, MLV_COLOR_WHITE);
}

void dessineConvexe(ConvexHull conv) {
    /*
    Dessine l'enveloppe convexe
    */
    Polygon cell = conv.pol;

    for (int i = 0; i < conv.curlen; i++, cell = cell->next) {
        MLV_draw_line(cell->s->x, cell->s->y, 
                    cell->next->s->x, cell->next->s->y, 
                    MLV_COLOR_ORCHID1);
    }
}

void choixcouleur(int nbrCol, MLV_Color * poly, MLV_Color * point) {
    /*
    Choisi une couleur en fonction de l'enveloppe convexe
    */
    switch (nbrCol%7) {
        case 1:
            *poly = MLV_rgba(255,0,0, 80);
            *point = MLV_rgba(255,0,0, 255);
            break;

        case 2:
            *poly = MLV_rgba(255,255,0, 80);
            *point = MLV_rgba(255,255,0, 255);
            break;

        case 3:
            *poly = MLV_rgba(0,255,0, 80);
            *point = MLV_rgba(0,255,0, 255);
            break;

        case 4:
            *poly = MLV_rgba(0,255,0, 80);
            *point = MLV_rgba(0,255,0, 255);
            break;

        case 5:
            *poly = MLV_rgba(0,255,255, 80);
            *point = MLV_rgba(0,255,255, 255);
            break;

        case 6:
            *poly = MLV_rgba(0,0,255, 80);
            *point = MLV_rgba(0,0,255, 255);
            break;

        default:
            *poly = MLV_rgba(255,0,255, 80);
            *point = MLV_rgba(255,0,255, 255);
            break;
    }
}

void dessineLstConvex(ListeConvex lst) {
    /*
    Dessine les enveloppes convexes emboitées avec leurs points et couleurs respectives
    */
    int lstX[LENMAX];
    int lstY[LENMAX];
    MLV_Color couleur;
    MLV_Color point;
    int col = 0;

    for (CelluleConvex *index = lst; index; index=index->suivant, col++) {
        Polygon cell = index->conv.pol;
        choixcouleur(col, &couleur, &point);

        for(int i = 0; i<index->conv.curlen; i++, cell = cell->next) {
            lstX[i] = cell->s->x;
            lstY[i] = cell->s->y;
        }

        MLV_draw_filled_polygon(lstX, lstY, index->conv.curlen, couleur);
        for (int i =0; i<index->conv.curlen; i++)
            MLV_draw_filled_circle(lstX[i], lstY[i], 4, point);
    }
            
}

int entrerPolygone(ListePoint *liste, ListeConvex *conv, int emboite) {
    /*
    Fonction permettant de dessiner un polygone à la souris
    */
    Point souris; 
    MLV_Event event = MLV_wait_keyboard_or_mouse(NULL, NULL, NULL, &souris.x, &souris.y);

    while (event!= MLV_KEY) {
        MLV_clear_window(MLV_COLOR_BLACK);
        
        insererTetePoint(liste, souris);
        if (emboite) {
            testInConvexLST(conv, &(*liste)->p);
            dessineLstConvex(*conv);
        } 
        
        else {
            dessineLstPoint(*liste);
            testNotInConvex(&(*conv)->conv, &(*liste)->p);
            dessineConvexe((*conv)->conv);
        }

        MLV_actualise_window();
        MLV_wait_milliseconds(100);
        event = MLV_wait_keyboard_or_mouse(NULL, NULL, NULL, &souris.x, &souris.y);
    }
    return 1;
}

int polyAleaCarre(ListePoint *liste, ListeConvex *conv, int centre_X, int centre_Y, int rayon, int nbpoints, int emboite) {
    /*
    Fonction permettant de dessiner un polygone aléatoire dans un carré
    */
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
        if (tempx == 0) {
            tempx = centre_X + (r * pow(-1, MLV_get_random_integer(0,2)));
            tempy = MLV_get_random_integer(centre_Y-r, centre_Y+r);
        }

        else
            tempy = centre_Y + r * (pow(-1, MLV_get_random_integer(0,2)));

        if (!emboite){
            if (tempx < 0)
                tempx = 0;

            else if (tempx > TAILLE_X)
                tempx = TAILLE_X;

            if (tempy < 0)
                tempy = 0;

            else if (tempy > TAILLE_Y)
                tempy = TAILLE_Y;
        }

        souris.x = tempx;
        souris.y = tempy;

        insererTetePoint(liste, souris);
        if (emboite) {
            testInConvexLST(conv, &(*liste)->p);
            dessineLstConvex(*conv);
        } 
        
        else {
            dessineLstPoint(*liste);
            testNotInConvex(&(*conv)->conv, &(*liste)->p);
            dessineConvexe((*conv)->conv);
        }

        MLV_actualise_window();
        MLV_wait_milliseconds(10);
    }
    return 1;
}

int polyAleaCercle(ListePoint *liste, ListeConvex *conv, int centre_X, int centre_Y, int rayon, int nbpoints, int emboite) {
    /*
    Fonction permettant de dessiner un polygone aléatoire dans un cercle
    */
    Point souris;
    double tempx; 
    double tempy;
    double point;
    double r;
    double pas = (double) rayon/nbpoints;

    for (int n = 1; n<=nbpoints; n++) {
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
        if (emboite) {
            testInConvexLST(conv, &(*liste)->p);
            dessineLstConvex(*conv);
        } 
        
        else {
            dessineLstPoint(*liste);
            testNotInConvex(&(*conv)->conv, &(*liste)->p);
            dessineConvexe((*conv)->conv);
        }

        MLV_actualise_window();
        MLV_wait_milliseconds(10);
    }
    return 1;
}

int trouverayon(Point origine, int figure){
    /*
    Fonction permettant de trouver le rayon à partir avec un second point 
    et selon la figure
    */ 
    int rayon;
    Point rayonClic;

    MLV_wait_mouse(&rayonClic.x, &rayonClic.y);
    if (figure >= 1 && figure <= 2)
        rayon = pow(rayonClic.x - origine.x, 2) > pow(rayonClic.y-origine.y,2) ? sqrt(pow(rayonClic.x  - origine.x,2)) : sqrt(pow(rayonClic.y - origine.y,2));

    else
        rayon = sqrt( pow(rayonClic.x - origine.x, 2) + pow(rayonClic.y - origine.y, 2));


    return rayon;
}

int Choixfigure(int figure) {
    /*
    Fonction permettant de choisir la figure à dessiner
    */
    ListePoint lst_p = NULL;
    ListeConvex lstConv= alloueConvex();
    Point souris;
    int rayon;

    if (figure >= 1 && figure <= 4){
        MLV_wait_mouse(&souris.x, &souris.y);
        dessinePoint(souris);
        MLV_wait_milliseconds(50);
        rayon = trouverayon(souris, figure);
    }
        
    switch (figure) {
        case 1:
            polyAleaCarre(&lst_p, &lstConv, souris.x, souris.y, rayon, NBRPOINTS, 0);
            break;
        
        case 2:
            polyAleaCarre(&lst_p, &lstConv, souris.x, souris.y, rayon, 200, 1);
            break;

        case 3:
            polyAleaCercle(&lst_p, &lstConv, souris.x, souris.y, rayon, NBRPOINTS, 0);
            break;
        
        case 4:
            polyAleaCercle(&lst_p, &lstConv, souris.x, souris.y, rayon, 200, 1);
            break;

        case 5:
            entrerPolygone(&lst_p, &lstConv, 0);
            break;

        case 6:
            entrerPolygone(&lst_p, &lstConv, 1);
            break;

        default:
            entrerPolygone(&lst_p, &lstConv, 0);
            break;
    }
    return 1;
}

void test(ConvexHull *conv, ListePoint *lst_p) {
    Point souris;
    initConvexHull(conv);

    for (int x = 0; x < 10; x++) {
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

int menu() {
    /*
    Fonction permettant d'afficher le menu
    */
    int choix;
    int type;

    printf("\n===============================================================\n");
    printf(" Bienvenue dans le programme de dessin de l'enveloppe convexe. \n");
    printf("===============================================================\n");

    printf("\nVeuillez choisir votre mode de génération :\n"\
    "1 : Entrée manuelle\n"\
    "2 : Génération aléatoire\n");

    scanf("%d", &choix);
    
    while (choix != 1 && choix != 2) {
        printf("\nVeuillez choisir un mode de génération valide :\n");
        scanf("%d", &choix);
    }

    printf("\nVeuillez choisir le type d'enveloppe convexe :\n"\
    "1 : Enveloppe convexe simple\n"\
    "2 : Enveloppes convexes emboîtées\n");
    scanf("%d", &type);

    while(type != 1 && type != 2) {
        printf("\nVeuillez choisir un type d'enveloppe convexe valide :\n");
        scanf("%d", &type);
    }

    if(choix == 1) {
        MLV_create_window("listechaine","", TAILLE_X, TAILLE_Y);
        if (type == 1)
            Choixfigure(5);
        else if (type == 2)
            Choixfigure(6);
    }

    else if(choix == 2) {
        int forme;
        printf("\nVeuillez choisir la forme de l'enveloppe convexe :\n"\
        "1 : Carré\n"\
        "2 : Cercle\n");
        scanf("%d", &forme);

        MLV_create_window("listechaine","", TAILLE_X, TAILLE_Y);

        if (type == 1) {
            if (forme == 1)
                Choixfigure(1);
            else if (forme == 2)
                Choixfigure(3);
        }

        else if (type == 2) {
            if (forme == 1)
                Choixfigure(2);
            else if (forme == 2)
                Choixfigure(4);
        }
    }
    return 1;
}


int main() {
    menu();
    MLV_wait_keyboard_or_mouse_or_seconds(NULL, NULL, NULL, NULL, NULL, 30);
    MLV_free_window();
    return 0;
}