#include "listepoints.h"

/**
 * @brief   Permet d'allouer l'espace de la mémoire suffisant pour une cellule
 *          d'une liste de point.
 * 
 * @param pval Valeur du point à ajouter.
 * @return CellulePoint* Adresse de la cellule créée.
 */
CellulePoint* allouerCellule(Point pval){
    CellulePoint* cell = (CellulePoint *)malloc (sizeof(CellulePoint));
    if (cell != NULL){
        cell->p = pval;
        cell->suivant = NULL;
    }
    return cell;
}

/**
 * @brief Permet d'insérer à la tête de la liste un point.
 * 
 * @param liste Liste de points à modifier
 * @param val   Valeur à ajouter
 * @return int  Indice pour savoir si l'allocation s'est bien passé.
 */
int insererTetePoint(ListePoint *liste, Point val){
    ListePoint temp = allouerCellule(val);
    if (temp != NULL){
        temp->suivant = *liste;
        *liste = temp;
        return 1;
    }
    return 0;
}