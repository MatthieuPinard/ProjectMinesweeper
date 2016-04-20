/*++

    Ce module implémente les fonctions nécessaires au jeu du Démineur.

--*/

#ifndef MINESWEEPER_H_INCLUDED
#define MINESWEEPER_H_INCLUDED

#include "list.h"
#include "random.h"
#include "gui.h"

#define MINE            -1

#define OFFSET(x, y, z) (((y) * (x)) + (z))

/*++

    Cette macro incrémente le champ nValue d'une cellule de coordonnées (nRow, nCol)
    si celle-ci n'est pas une mine,
    et qu'elle est bien une cellule correcte (contrôle des coordonnées)

--*/

#define UPDATE_ADJACENT_CELL(pGrid, nRow, nCol)                                                                \
        if ((nCol) < (pGrid)->gridParameters.nCols &&                                                          \
            (nRow) < (pGrid)->gridParameters.nRows &&                                                          \
            ((pGrid)->pFirstCell + OFFSET((pGrid)->gridParameters.nCols, (nRow), (nCol)))->nValue != MINE)     \
        {                                                                                                      \
            ((pGrid)->pFirstCell + OFFSET((pGrid)->gridParameters.nCols, (nRow), (nCol)))->nValue++;           \
        }

/*++

    Cette macro est utilisée dans RevealCells().
    Elle sert à quitter la fonction si un appel Expr a renvoyé un statut différent de SUCCESS.

--*/

#define RETURN_ON_FAILURE(Expr, pList, Status)                                                  \
        if(((Status) = (Expr)) != SUCCESS)                                                      \
        {                                                                                       \
            DeleteList(pList);                                                                  \
            return (Status);                                                                    \
        }

VOID InitializeGridParameters(UINT, UINT, UINT, PGRID_PARAMETERS);

PGRID InitializeGrid (PGRID_PARAMETERS);

PPCELL FillGridWithMines (PGRID);

VOID RewindGrid(PGRID);

VOID FreeGrid (PGRID);

GAME_STATUS HasPlayerWon (PGRID);

STATUS ComputeCellValues (PGRID, PPCELL);

STATUS RevealCells (PGRID, PCELL);

STATUS RevealAndPushCell (PLIST, PGRID, UINT, UINT);

STATUS PlayOneStep(PGRID, CELL_PARAMETERS, LPCRITICAL_SECTION);

#endif
