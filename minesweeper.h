/*++

    Ce module impl�mente les fonctions n�cessaires au jeu du D�mineur.

--*/

#ifndef MINESWEEPER_H_INCLUDED
#define MINESWEEPER_H_INCLUDED

#include "list.h"
#include "random.h"
#include "gui.h"

#define MINE            -1

#define OFFSET(x, y, z) (((y) * (x)) + (z))

/*++

    Cette macro incr�mente le champ nValue d'une cellule de coordonn�es (nRow, nCol)
    si celle-ci n'est pas une mine,
    et qu'elle est bien une cellule correcte (contr�le des coordonn�es)

--*/

#define UPDATE_ADJACENT_CELL(pGrid, nRow, nCol)                                                                \
        if ((nCol) < (pGrid)->gridParameters.nCols &&                                                          \
            (nRow) < (pGrid)->gridParameters.nRows &&                                                          \
            ((pGrid)->pFirstCell + OFFSET((pGrid)->gridParameters.nCols, (nRow), (nCol)))->nValue != MINE)     \
        {                                                                                                      \
            ((pGrid)->pFirstCell + OFFSET((pGrid)->gridParameters.nCols, (nRow), (nCol)))->nValue++;           \
        }

/*++

    Cette macro est utilis�e dans RevealCells().
    Elle sert � quitter la fonction si un appel Expr a renvoy� un statut diff�rent de SUCCESS.

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
