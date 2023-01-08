#include "enveloppeconvex.h"


/**
 * @brief Fonction qui initialise une enveloppe Convexe
 * 
 * @param conv Enveloppe convexe à initialiser
 */
void initConvexHull(ConvexHull * conv) {
    conv->curlen = 0;
    conv->maxlen = LENMAX;
    conv->pol = NULL;
}

/**
 * @brief   Fonction allouant la mémoire nécessaire pour une cellule de l'enveloppe convexe
            et y place l'adresse d'un point
 * 
 * @param pval      Adresse de point à placer
 * @return Vertex*  Cellule résultante de l'allocation
 */
Vertex* allouerVertex(Point *pval) {
    Vertex *cell = (Vertex *)malloc (sizeof(Vertex));

    if (cell) {
        cell->s = pval;
        cell->prev = NULL;
        cell->next = NULL;
    }
    return cell;
}

/**
 * @brief Fonction testant si l'enveloppe est vide.
 * 
 * @param conv Enveloppe convexe à tester
 * @return int 1 si vide, 0 sinon
 */
int estVide(ConvexHull *conv) {
    if (conv->pol)
        return 0;
    return 1;
}

/**
 * @brief Fonction ajoutant, à la fin, une adresse de point à l'enveloppe convexe
 * 
 * @param conv Enveloppe convexe à modifier
 * @param pval Adresse d'un point à ajouter à l'enveloppe convexe.
 * @return int Indice pour savoir si tous s'est bien passé.
 */
int enfileConvex(ConvexHull *conv, Point *pval) {
    Vertex* nouveau = allouerVertex(pval);
    if (nouveau) {

        if (estVide(conv)) {
            nouveau->next = nouveau;
            nouveau->prev = nouveau;
            conv->pol = nouveau;
            conv->curlen = 1;
        }

        else {
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

/**
 * @brief   Fonction permettant d'insérer l'adresse d'un point après une
 *          vertex précise.
 * 
 * @param maillon   Vertex référence pour savoir où ajouter l'adresse du point
 * @param pval      Adresse du point à ajouter
 * @param conv      Enveloppe convexe à modifier
 * @return int      Permet de savoir si tous s'est bien passé
 */
int insertConvex(Vertex *maillon, Point *pval, ConvexHull *conv) {
    Vertex* nouveau = allouerVertex(pval);
    if (nouveau) {
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

/**
 * @brief Fonction testant si le triangle, formé par trois points, est indirect ou non
 * 
 * Généralement
 * @param A Point d'origine
 * @param B Son suivant
 * @param C Le suivant du suivant
 * @return int 1 si indirect, 0 sinon
 */
int test_triangle_Indirect(Point A, Point B, Point C) {
    if (((B.x - A.x) * (C.y - A.y)) - ((C.x - A.x) * (B.y - A.y)) < 0)
        return 1;
    return 0;
}

/**
 * @brief Supprime un point d'une enveloppe convexe
 * 
 * @param suppression   Vertex à supprimer
 * @param conv          Enveloppe convexe à modifier
 */
void suppresionVertex(Vertex* suppression, ConvexHull *conv) {
    if (conv->curlen > 3) {
        if (suppression == conv->pol)
            conv->pol = conv->pol->next;

        suppression->next->prev = suppression->prev;
        suppression->prev->next = suppression->next;

        free(suppression);
        conv->curlen--;
    }
}
/**
 * @brief Fonction nettoyant l'enveloppe convexe en supprimant les points avants qui ne sont pas dans l'enveloppe
 * 
 * @param origine   Point où on commence le nettoyage
 * @param conv      Enveloppe convexe à modifier
 */
void nettoyageAvant(Vertex* origine, ConvexHull *conv) {
    while (test_triangle_Indirect(*origine->s, *(origine->next)->s, *(origine->next->next)->s))
        suppresionVertex(origine->next, conv);
}

/**
 * @brief Fonction nettoyant l'enveloppe convexe en supprimant les points arrières qui ne sont pas dans l'enveloppe
 * 
 * @param origine   Point où on commence le nettoyage
 * @param conv      Enveloppe convexe à modifier
 */
void nettoyageArriere(Vertex* origine, ConvexHull *conv) {
    while (test_triangle_Indirect(*origine->s, *(origine->prev->prev)->s, *(origine->prev)->s))
        suppresionVertex(origine->prev, conv);
}

/**
 * @brief Fonction testant si un point est dans l'enveloppe convexe
 * 
 * @param conv Enveloppe convexe à tester
 * @param point Point à tester
 * @return int 1 si dans l'enveloppe, 0 sinon
 */
int testNotInConvex(ConvexHull *conv, Point *point) {
    Polygon cell = conv->pol;
    Point A;
    Point B;
    Point C;

    if (conv->curlen < 2) {
        if (enfileConvex(conv, point))
            return 1;
    }

    else if (conv->curlen == 2) {
        A = *point;
        B = *(conv->pol->s);
        C = *(conv->pol->next->s);

        if (test_triangle_Indirect(A, B, C)) {
            if (insertConvex(conv->pol, point, conv))
                return 1;
        }
        
        else {
            if(enfileConvex(conv, point))
                return 1;
        }
    }

    else {
        for (int i = 0; i<conv->curlen; i++, cell = cell->next) {
            A = *point;
            B = *(cell->s);
            C = *(cell->next->s);
            if (test_triangle_Indirect(A,B,C)) {
                if(insertConvex(cell, point, conv)) {
                    nettoyageArriere(cell->next, conv);
                    nettoyageAvant(cell->next, conv);
                    return 1;
                }
            }
        }
    }
    return 0;
}
