#include "convexemboite.h"

/**
 * @brief Permet d'allouer l'espace nécessaire à une enveloppe convex et l'initialise
 * 
 * @return CelluleConvex* Adresse de l'enveloppe convexe allouée
 */
CelluleConvex* alloueConvex() {
    CelluleConvex *cell = (CelluleConvex *)malloc (sizeof(CelluleConvex));

    if (cell) {
        initConvexHull(&cell->conv);
        cell->suivant = NULL;
    }
    return cell;
}

/**
 * @brief Ajoute une enveloppe convexe à la fin d'une liste d'enveloppes convexes
 * 
 * @param l     Liste d'enveloppes à modifier
 * @return int  Permet de savoir si l'allocation s'est bien passé
 */
int enfileLstConvex(ListeConvex *l) {
    /*
    Fonction qui ajoute un maillon à la fin de la liste chainée
    */
    ListeConvex index = *l;
    ListeConvex temp = alloueConvex();

    if (temp) {
        for(; index->suivant; index = index->suivant)
            ;
        index->suivant = temp;
        return 1;
    }
    return 0;
}

/**
 * @brief Supprime un vertex d'une liste de vertex contenue dans
 *          une enveloppe convexe
 * 
 * @param suppression   Adresse du vertex à supprimer
 * @param lstConv       Enveloppe convexe à modifier
 */
void suppresionVertexLST(Vertex* suppression, ListeConvex *lstConv) {
    if ((*lstConv)->conv.curlen > 3) {  // Si la liste contient plus de 3 maillons
        if (suppression == (*lstConv)->conv.pol)
            (*lstConv)->conv.pol = (*lstConv)->conv.pol->next;

        suppression->next->prev = suppression->prev;
        suppression->prev->next = suppression->next;

        if (!(*lstConv)->suivant) 
            enfileLstConvex(lstConv);

        testInConvexLST(&(*lstConv)->suivant, suppression->s);

        free(suppression);
        (*lstConv)->conv.curlen--;
    }
}

/**
 * @brief Fonction qui, en cas de suppression d'un maillon, nettoie les maillons avants qui ne sont plus dans l'enveloppe convexe concernée
 * 
 * @param origine   Vertex d'origine pour le nettoyage
 * @param conv      Enveloppe convexe où est contenue le vertex
 */
void nettoyageAvantLST(Vertex* origine, ListeConvex *conv) {
    while (test_triangle_Indirect(*origine->s, *(origine->next)->s, *(origine->next->next)->s))
        suppresionVertexLST(origine->next, conv);
}

/**
 * @brief Fonction qui, en cas de suppression d'un maillon, nettoie les maillons arrières qui ne sont plus dans l'enveloppe convexe concernée
 * 
 * @param origine   Vertex d'origine pour le nettoyage
 * @param conv      Enveloppe convexe où est contenue le vertex
 */
void nettoyageArriereLST(Vertex* origine, ListeConvex *conv) {
    while (test_triangle_Indirect(*origine->s, *(origine->prev->prev)->s, *(origine->prev)->s))
        suppresionVertexLST(origine->prev, conv);
}

/**
 * @brief   Fonction qui teste si un point est dans l'enveloppe convexe concernée,
 *          Sinon, teste récursivement avec l'enveloppe convexe suivante
 * 
 * @param lstConv   Liste d'enveloppe convexes
 * @param point     Adresse d'un point à tester
 * @return int      Permet de savoir si tout s'est bien passé
 */
int testInConvexLST(ListeConvex *lstConv, Point *point) {
    Polygon cell = (*lstConv)->conv.pol;
    Point A;
    Point B;
    Point C;

    if ((*lstConv)->conv.curlen < 2) {  // Si la liste contient moins de 2 maillons, on ajoute le point
        if (enfileConvex(&(*lstConv)->conv, point))
            return 1;
    }

    else if ((*lstConv)->conv.curlen == 2) {   // Si la liste contient 2 maillons, on teste si le point est dans le triangle formé par les 2 maillons
        A = *point;
        B = *((*lstConv)->conv.pol->s);
        C = *((*lstConv)->conv.pol->next->s);

        if (test_triangle_Indirect(A, B, C)) {
            if (insertConvex((*lstConv)->conv.pol, point, &(*lstConv)->conv))
                return 1;
        }
        
        else {
            if(enfileConvex(&(*lstConv)->conv, point))
            return 1;
        }
    }

    else {  // Si la liste contient plus de 2 maillons, on teste si le point est dans le triangle formé par les 3 maillons successifs
        for (int i = 0; i<(*lstConv)->conv.curlen; i++, cell = cell->next) {
            A = *point;
            B = *(cell->s);
            C = *(cell->next->s);

            if (test_triangle_Indirect(A,B,C)){ // Si le point est dans le triangle, on teste si le point est dans l'enveloppe convexe
                if(insertConvex(cell, point, &(*lstConv)->conv)) {
                        nettoyageArriereLST(cell->next, lstConv);
                        nettoyageAvantLST(cell->next, lstConv);
                        return 1;
                }
            }
        }
    }

    if (!(*lstConv)->suivant)
        enfileLstConvex(lstConv);

    testInConvexLST(&(*lstConv)->suivant, point);

    
    return 0;
}
