#include "listepoints.h"

CellulePoint* allouerCellule(Point pval){
    CellulePoint* cell = (CellulePoint *)malloc (sizeof(CellulePoint));
    if (cell != NULL){
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

void affichageLstPoint(ListePoint liste){
    for(;liste; liste= liste->suivant){
        printf("%d %d\n", liste->p.x, liste->p.y);
    }
}