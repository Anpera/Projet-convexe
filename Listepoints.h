#ifndef _LISTEPOINTS_H_
#define _LISTEPOINTS_H_

#define TAILLE_X 512
#define TAILLE_Y 512
#define LENMAX 50

typedef struct{
    int x;
    int y;
} Point;

typedef struct cellulePoint {
    Point p;
    struct cellulePoint *suivant;
} CellulePoint, *ListePoint;

#endif