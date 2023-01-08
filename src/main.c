#include "main.h"

/**
 * @brief Permet de dessiner un point unique.
 * 
 * @param p Point à dessiner
 */
void dessinePoint(Point p){
    MLV_draw_filled_circle(p.x, p.y, 4, MLV_COLOR_WHITE);
    MLV_actualise_window();
    MLV_clear_window(MLV_COLOR_BLACK);
}

/**
 * @brief Permet de dessiner tous les points présents dans
 *        une liste de points
 * 
 * @param l Liste chaînée de points
 */
void dessineLstPoint(ListePoint l) {
    for(; l; l = l->suivant)
        MLV_draw_filled_circle(l->p.x, l->p.y, 4, MLV_COLOR_WHITE);
}

/**
 * @brief Dessine le polygone formé par les points
 *        contenue dans une enveloppe convexe
 * 
 * @param conv Enveloppe convexe contenant les points
 */
void dessineConvexe(ConvexHull conv){
    Polygon cell = conv.pol;

    for (int i = 0; i < conv.curlen; i++, cell = cell->next) {
        MLV_draw_line(cell->s->x, cell->s->y, 
                    cell->next->s->x, cell->next->s->y, 
                    MLV_COLOR_ORCHID1);
    }
}

/**
 * @brief Transforme deux variables MLV_Color et leur attribue une couleur
 *        selon la variable nbrCol
 * 
 * @param nbrCol Identifiant de l'enveloppe convexe qui permet d'attribuer une couleur
 * @param poly Couleur pour le polygone associer à l'enveloppe convexe
 * @param point Couleur des points formant le polygone
 */
void choixcouleur(int nbrCol, MLV_Color * poly, MLV_Color * point){
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

/**
 * @brief Dessine toutes les enveloppes convexes
 *        contenus dans une liste d'enveloppes convexes
 * 
 * @param lst Liste d'enveloppes convexes
 */

void dessineLstConvex(ListeConvex lst){
    int lstX[LENMAX];
    int lstY[LENMAX];
    MLV_Color couleur;
    MLV_Color point;
    int col = 0;

    for (CelluleConvex *index = lst; index; index=index->suivant, col++) {  // Pour chaque enveloppe convexe
        Polygon cell = index->conv.pol;
        choixcouleur(col, &couleur, &point);

        for(int i = 0; i<index->conv.curlen; i++, cell = cell->next) {  // Pour chaque point de l'enveloppe convexe
            lstX[i] = cell->s->x;
            lstY[i] = cell->s->y;
        }

        MLV_draw_filled_polygon(lstX, lstY, index->conv.curlen, couleur);
        for (int i =0; i<index->conv.curlen; i++)
            MLV_draw_filled_circle(lstX[i], lstY[i], 4, point);
    }
            
}
/**
 * @brief Permet de dessiner en direct un polygone selon les clics de la souris
 * 
 * @param liste Liste de points permettant de stocker les points de la souris
 * @param conv  Liste d'enveloppes convexes permettant de stocker la ou les
 *              enveloppe.s convexes
 * @param emboite Option pour savoir si on a choisit le mode enveloppes convexes
 *                emboîtées
 * @return int  Permet de savoir si tous s'est bien passé.
 */
int entrerPolygone(ListePoint *liste, ListeConvex *conv, int emboite) {
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

/**
 * @brief Permet de dessiner automatique une enveloppe convexe formé à partir
 *        de points aléatoire contenus dans un carré.
 * 
 * @param liste         Liste de points
 * @param conv          Liste d'enveloppes convexes
 * @param centre_X      Position X du centre de la forme
 * @param centre_Y      Position Y du centre de la forme
 * @param rayon         Longueur maximum qu'atteindra le carré.
 * @param nbpoints      Nombre de points à générer
 * @param emboite       Option pour savoir si c'est des enveloppes emboîtées ou non.
 * @return int          Valeur pour savoir si tout s'est bien passé.
 */

int polyAleaCarre(ListePoint *liste, ListeConvex *conv, int centre_X, int centre_Y, int rayon, int nbpoints, int emboite) {
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
        MLV_wait_milliseconds(50);
    }
    return 1;
}

/**
 * @brief Permet de dessiner automatique une enveloppe convexe formé à partir
 *        de points aléatoire contenus dans un cercle.
 * 
 * @param liste         Liste de points
 * @param conv          Liste d'enveloppes convexes
 * @param centre_X      Position X du centre de la forme
 * @param centre_Y      Position Y du centre de la forme
 * @param rayon         Longueur maximum qu'atteindra le cercle.
 * @param nbpoints      Nombre de points à générer
 * @param emboite       Option pour savoir si c'est des enveloppes emboîtées ou non.
 * @return int          Valeur pour savoir si tout s'est bien passé.
 */
int polyAleaCercle(ListePoint *liste, ListeConvex *conv, int centre_X, int centre_Y, int rayon, int nbpoints, int emboite) {
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
        MLV_wait_milliseconds(50);
    }
    return 1;
}

/**
 * @brief       Fonction permettant de trouver le rayon à partir avec un second point 
 *              et selon la figure
 * 
 * @param origine   Point d'origine pour calculer le rayon.
 * @param figure    Type de figure pour pouvoir connaitre le rayon.
 * @return int      Valeur du rayon qu'on a sélectionner
 */

int trouverayon(Point origine, int figure){
    int rayon;
    Point rayonClic;

    MLV_wait_mouse(&rayonClic.x, &rayonClic.y);
    if (figure >= 1 && figure <= 2)
        rayon = pow(rayonClic.x - origine.x, 2) > pow(rayonClic.y-origine.y,2) ? sqrt(pow(rayonClic.x  - origine.x,2)) : sqrt(pow(rayonClic.y - origine.y,2));

    else
        rayon = sqrt( pow(rayonClic.x - origine.x, 2) + pow(rayonClic.y - origine.y, 2));


    return rayon;
}
/**
 * @brief   Permet de choisir le mode d'exécution entre forme carré ou circulaire,
 *          enveloppe emboîtée ou non, manuel ou générer aléatoirement
 * 
 * @param figure Numéro d'identification du mode
 * @return int 
 */
int choixfigure(int figure){
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
/**
 * @brief Permet d'afficher le menu de sélection du mode d'exécution
 * 
 * @return int 
 */
int menu() {
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
            choixfigure(5);
        else if (type == 2)
            choixfigure(6);
    }

    else if(choix == 2) {
        int forme;
        printf("\nVeuillez choisir la forme de l'enveloppe convexe :\n"\
        "1 : Carré\n"\
        "2 : Cercle\n");
        scanf("%d", &forme);

        while (forme != 1 && forme != 2) {
            printf("\nVeuillez choisir une forme valide :\n");
            scanf("%d", &forme);
        }

        MLV_create_window("listechaine","", TAILLE_X, TAILLE_Y);

        if (type == 1) {
            if (forme == 1)
                choixfigure(1);
            else if (forme == 2)
                choixfigure(3);
        }

        else if (type == 2) {
            if (forme == 1)
                choixfigure(2);
            else if (forme == 2)
                choixfigure(4);
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