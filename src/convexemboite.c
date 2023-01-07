#include "convexemboite.h"


CelluleConvex* alloueConvex(){
    CelluleConvex *cell = (CelluleConvex *)malloc (sizeof(CelluleConvex));
    if (cell){
        initConvexHull(&cell->conv);
        cell->suivant = NULL;
    }
    return cell;
}

int enfileLstConvex(ListeConvex *l){
    ListeConvex index = *l;
    ListeConvex temp = alloueConvex();
    if (temp){
        for(;index->suivant;index = index->suivant){
            ;
        }
        index->suivant = temp;
        return 1;
    }
    return 0;
}

void suppresionVertexLST(Vertex* suppression, ListeConvex *lstConv){
    if ((*lstConv)->conv.curlen >3){
        if (suppression == (*lstConv)->conv.pol){
            (*lstConv)->conv.pol = (*lstConv)->conv.pol->next;
        }
        suppression->next->prev = suppression->prev;
        suppression->prev->next = suppression->next;

        if (!(*lstConv)->suivant)
        enfileLstConvex(lstConv);

        testInConvexLST(&(*lstConv)->suivant, suppression->s);

        free(suppression);
        (*lstConv)->conv.curlen--;
    }
}

void nettoyageAvantLST(Vertex* origine, ListeConvex *conv){   
    while (test_triangle_Indirect(*origine->s, *(origine->next)->s, *(origine->next->next)->s))
        suppresionVertexLST(origine->next, conv);
}

void nettoyageArriereLST(Vertex* origine, ListeConvex *conv){
    while (test_triangle_Indirect(*origine->s, *(origine->prev->prev)->s, *(origine->prev)->s))
        suppresionVertexLST(origine->prev, conv);
}

int testInConvexLST(ListeConvex *lstConv, Point *point){
    Polygon cell = (*lstConv)->conv.pol;
    
    Point A;
    Point B;
    Point C;

    if ((*lstConv)->conv.curlen < 2){
        if (enfileConvex(&(*lstConv)->conv, point)){
            return 1;
        }
    }

    else if ((*lstConv)->conv.curlen == 2){
        A = *point;
        B = *((*lstConv)->conv.pol->s);
        C = *((*lstConv)->conv.pol->next->s);

        if (test_triangle_Indirect(A, B, C)){
            if (insertConvex((*lstConv)->conv.pol, point, &(*lstConv)->conv)){
                return 1;
            }
        }
        
        else{
            if(enfileConvex(&(*lstConv)->conv, point))
            return 1;
        }
    }

    else{
        for (int i = 0; i<(*lstConv)->conv.curlen; i++, cell = cell->next){
            A = *point;
            B = *(cell->s);
            C = *(cell->next->s);
            if (test_triangle_Indirect(A,B,C)){

                if(insertConvex(cell, point, &(*lstConv)->conv)){
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


void printLstConvex(ListeConvex l){
    for (CelluleConvex *index = l; index; index=index->suivant)
        printf("%d \n", index->conv.curlen);
}