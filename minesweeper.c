#include "minesweeper.h"

VOID RewindGrid(PGRID pGrid)
{
    PCELL pCurrentCell, pEndCell;
    if (!pGrid || !pGrid->pFirstCell)
    {
        DisplayLastError(INVALID_ARGS);
        return;
    }
    pCurrentCell = pGrid->pFirstCell;
    pEndCell = pCurrentCell + (pGrid->gridParameters.nCols * pGrid->gridParameters.nRows);
    for(; pCurrentCell != pEndCell; pCurrentCell++)
    {
        pCurrentCell->state = CELL_HIDDEN;
    }
}

/*++

    Initialise une structure contenant la taille et le nombre de mines de la grille,
    indiqu�s en argument.

    Arguments :
        nRows - Le nombre de lignes.
        nCols - Le nombre de colonnes.
        nMines - Le nombre de mines.

    Renvoie NULL en cas d'�chec de la fonction, un pointeur sur la structure sinon.

--*/

VOID InitializeGridParameters(UINT nRows, UINT nCols, UINT nMines, PGRID_PARAMETERS pGridParameters)
{
	if (nMines > nRows * nCols || !pGridParameters)
    {
        DisplayLastError(INVALID_ARGS);
        return;
    }
    pGridParameters->nRows = nRows;
    pGridParameters->nCols = nCols;
    pGridParameters->nMines = nMines;
}

/*++

    Initialise une grille de d�mineur, dont les param�tres sont indiqu�s en argument.

    Arguments :
        pGridParameters - Un pointeur sur une structure contenant le nombre de lignes, de colonnes
        et de mines dans la grille, initialis�e par un appel � InitializeGridParameters().

    Renvoie NULL en cas d'�chec de la fonction, un pointeur sur la grille sinon.

--*/

PGRID InitializeGrid (PGRID_PARAMETERS pGridParameters)
{
    PCELL pLastCell, pFirstCell, pCell;
    PGRID pGrid;
    if (!pGridParameters)
    {
        DisplayLastError(INVALID_ARGS);
        return NULL;
    }
    pFirstCell = malloc(pGridParameters->nRows * pGridParameters->nCols * sizeof(CELL));
    pGrid = malloc(sizeof(GRID));
    if (!pFirstCell || !pGrid)
    {
        free(pFirstCell);
        free(pGrid);
        DisplayLastError(MEMORY_ERROR);
        return NULL;
    }
    pGrid->pFirstCell = pFirstCell;
    pGrid->gridParameters = *pGridParameters;

    //
    // Initialise chaque cellule de la grille.
    //

    pLastCell = pFirstCell + (pGridParameters->nRows * pGridParameters->nCols);
    pCell = pFirstCell;
    for (; pCell != pLastCell; pCell++)
    {
        pCell->nValue = 0;
        pCell->state = CELL_HIDDEN;
    }
    return pGrid;
}

/*++

    Remplit la grille, point�e par pGrid, du bon nombre de mines (indiqu� par pGrid->nMines), de fa�on al�atoire.

    Arguments :
        pGrid - Le pointeur sur la grille � remplir.

    Renvoie NULL en cas d'�chec de la fonction, un tableau de pointeurs vers les cellules min�es sinon.

--*/

PPCELL FillGridWithMines (PGRID pGrid)
{
    UINT nPos, nCount = 0;
    PCELL pCurrentCell;
    PPCELL apMines;
    if (!pGrid || !pGrid->pFirstCell)
    {
        DisplayLastError(INVALID_ARGS);
        return NULL;
    }
    apMines = malloc(pGrid->gridParameters.nMines * sizeof(PCELL));
    if (!apMines)
    {
        DisplayLastError(MEMORY_ERROR);
        return NULL;
    }
    while (nCount < pGrid->gridParameters.nMines)
    {

        //
        // On r�cup�re une cellule de la grille de mani�re al�atoire.
        // Si elle n'est pas d�j� remplie avec une mine, on met une
        // mine � l'int�rieur de cette cellule.
        //

        nPos = GenerateRandomNumber(pGrid->gridParameters.nCols * pGrid->gridParameters.nRows);
        pCurrentCell = pGrid->pFirstCell + nPos;
        if(pCurrentCell->nValue == 0)
        {
            apMines[nCount] = pCurrentCell;
            nCount++;
            pCurrentCell->nValue = MINE;
        }
    }

    //
    // Renvoie un tableau contenant des pointeurs sur les cellules min�es.
    //

    return apMines;
}

/*++

    Cette fonction compl�te l'initialisation de la grille.
    Chaque cellule est mise � jour pour que son champ nValue
    soit �gal au nombre de mines contenues dans les cellules
    imm�diatement voisines.

    Arguments :
        pGrid - Le pointeur sur la grille � remplir.
        apMines - Un tableau de pointeurs vers les cellules min�es.
                  Doit �tre obtenu par un appel � FillGridWithMines().

    Renvoie SUCCESS en cas de r�ussite de la fonction,
            INVALID_ARGS si les param�tres d'entr�e sont invalides.

    En cas d'�chec, la grille est inchang�e.

--*/

STATUS ComputeCellValues (PGRID pGrid, PPCELL apMines)
{
    UINT i, nCol, nRow;
    if (!pGrid || !pGrid->pFirstCell ||
        (pGrid->gridParameters.nMines != 0 && !apMines))
    {
        return INVALID_ARGS;
    }
    for (i = 0; i < pGrid->gridParameters.nMines; i++)
    {

        //
        // On r�cup�re les coordonn�es de chaque mine,
        // et on incr�mente de 1 le champ nValue des cellules
        // autour.
        //

        nRow = (apMines[i] - pGrid->pFirstCell) / pGrid->gridParameters.nCols;
        nCol = (apMines[i] - pGrid->pFirstCell) % pGrid->gridParameters.nCols;
        UPDATE_ADJACENT_CELL(pGrid, nRow - 1, nCol - 1)
        UPDATE_ADJACENT_CELL(pGrid, nRow - 1, nCol    )
        UPDATE_ADJACENT_CELL(pGrid, nRow - 1, nCol + 1)
        UPDATE_ADJACENT_CELL(pGrid, nRow    , nCol - 1)
        UPDATE_ADJACENT_CELL(pGrid, nRow    , nCol + 1)
        UPDATE_ADJACENT_CELL(pGrid, nRow + 1, nCol - 1)
        UPDATE_ADJACENT_CELL(pGrid, nRow + 1, nCol    )
        UPDATE_ADJACENT_CELL(pGrid, nRow + 1, nCol + 1)
    }

    //
    // On lib�re apMines, puisqu'on n'utilisera pas
    // ce tableau dans le futur.
    //

    free(apMines);
    return SUCCESS;
}

/*++

    Lib�re la m�moire utilis�e par la grille.

    Arguments :
        pGrid - Le pointeur sur la grille.

--*/

VOID FreeGrid (PGRID pGrid)
{
    if (pGrid)
    {
        //
        // Lib�re la m�moire utilis�e par les cellules
        // et par la structure gridParameters.
        //

		free(pGrid->pFirstCell);
    }
	free(pGrid);
    pGrid = NULL;
}

STATUS PlayOneStep(PGRID pGrid, CELL_PARAMETERS CellParameters, LPCRITICAL_SECTION pCriticalSection)
{
    PCELL pPlayedCell;
    if (!pGrid ||
        CellParameters.nCol >= pGrid->gridParameters.nCols ||
        CellParameters.nRow >= pGrid->gridParameters.nRows)
    {
        return INVALID_ARGS;
    }
    pPlayedCell = pGrid->pFirstCell + OFFSET(pGrid->gridParameters.nCols,
                                             CellParameters.nRow,
                                             CellParameters.nCol);
    if(pPlayedCell->state == CELL_HIDDEN)
    {
        if (!CellParameters.bIsFlag)
        {
            RevealCells(pGrid, pPlayedCell);
        }
        else
        {
            pPlayedCell->state = CELL_IS_FLAG;
			UpdateTitle(pCriticalSection, -1);
        }
    }
    else if (pPlayedCell->state == CELL_IS_FLAG && CellParameters.bIsFlag)
    {
        pPlayedCell->state = CELL_HIDDEN;
		UpdateTitle(pCriticalSection, 1);
    }
    return PrintGrid(pGrid, FALSE);
}

/*++

    L'�tat de la partie (gagn�, perdu, en cours) est d�termin� grace � cette fonction.

    Arguments :
        pGrid - Le pointeur sur la grille.

    Renvoie GAME_LOSS si le joueur a perdu (une mine a �t� d�voil�e donc jou�e)
            GAME_WIN si le joueur a gagn� (toutes les cases non min�es ont �t� d�voil�es)
            GAME_UNDEFINED si les param�tres d'entr�e sont invalides.
            GAME_IN_PROGRESS si la partie est en cours.

--*/

GAME_STATUS HasPlayerWon (PGRID pGrid)
{
    UINT nNonRevealedCellsLeft;
    PCELL pCurrentCell, pEndCell;
    if (!pGrid || !pGrid->pFirstCell)
    {
        DisplayLastError(INVALID_ARGS);
        return GAME_UNDEFINED;
    }
    nNonRevealedCellsLeft = 0;
    pCurrentCell = pGrid->pFirstCell;
    pEndCell = pCurrentCell + (pGrid->gridParameters.nCols * pGrid->gridParameters.nRows);
    for(; pCurrentCell != pEndCell; pCurrentCell++)
    {

        //
        // On compte le nombre de cellules non r�v�l�es.
        //

        if(pCurrentCell->state != CELL_REVEALED)
        {
            nNonRevealedCellsLeft++;
        }

        //
        // Si le joueur a jou� une cellule min�e, il a perdu.
        //

        else if (pCurrentCell->nValue == MINE)
        {
            return GAME_LOSS;
        }
    }

    //
    // S'il est �gal au nombre de cellules min�es, le joueur a gagn�.
    // On ne peut rien dire sinon.
    //

    return (pGrid->gridParameters.nMines == nNonRevealedCellsLeft) ? GAME_WIN : GAME_IN_PROGRESS;
}

/*++

    R�v�le la cellule de coordonn�es (nRow, nCol), et, si son champ nValue est nul
    (aucune mine � proximit�), ajoute un pointeur sur cette cellule � la liste pList.

    Arguments :
        nRow - La ligne de la cellule.
        nCol - La colonne de la cellule.
        pList - Le pointeur sur la liste.
        pGrid - Le pointeur sur la grille.

    Renvoie INVALID_ARGS ou MEMORY_ERROR en cas d'�chec de la fonction, SUCCESS sinon.
    Cette fonction conserve la liste, la cellule et la grille initiales en cas d'�chec.

--*/

STATUS RevealAndPushCell (PLIST pList, PGRID pGrid, UINT nRow, UINT nCol)
{
    STATUS status;
    if (!pList || !pGrid || !pGrid->pFirstCell)
    {
        return INVALID_ARGS;
    }
    if (nCol < pGrid->gridParameters.nCols && nRow < pGrid->gridParameters.nRows)
    {
        PCELL pCurrentCell = pGrid->pFirstCell + OFFSET(pGrid->gridParameters.nCols, nRow, nCol);
        if (pCurrentCell->state == CELL_HIDDEN)
        {
            //
            // Si la cellule n'est pas entour�e de mines (champ nValue == 0),
            // on l'ajoute � la liste et on la r�v�le.
            //

            if (pCurrentCell->nValue == 0)
            {
                if ((status = AddEntryToList(pList, pCurrentCell)) != SUCCESS)
                {
                    return status;
                }
            }
            pCurrentCell->state = CELL_REVEALED;
        }
    }
    return SUCCESS;
}

/*++

    R�v�le la cellule point�e par pCell, et, le cas �ch�ant, son entourage.

    Arguments :
        pGrid - Le pointeur sur la grille.
        pCell - Le pointeur sur la cellule � d�voiler.

    La fonction renvoie SUCCESS en cas de r�ussite.
                        INVALID_ARGS si les param�tres d'entr�e sont incorrects.
                        MEMORY_ERROR, ENTRY_NOT_FOUND, INVALID_LIST_REFERENCE
                        si une erreur a eu lieu lorsque la fonction a essay� de d�voiler les cellules voisines � pCell.

    En cas d'�chec, la grille et la cellule restent dans un �tat valide.

--*/

STATUS RevealCells (PGRID pGrid, PCELL pCell)
{
    STATUS status;
    UINT nCol, nRow;
    if (!pGrid || !pCell || !pGrid->pFirstCell)
    {
        return INVALID_ARGS;
    }
    if (pCell->state != CELL_HIDDEN)
    {
        return INVALID_ARGS;
    }

    //
    // On r�v�le la cellule point�e par pCell.
    //

    pCell->state = CELL_REVEALED;
    if (pCell->nValue == 0)
    {
        PLIST pList = InitializeNewList();
        if (!pList)
        {
            return MEMORY_ERROR;
        }
        RETURN_ON_FAILURE(AddEntryToList(pList, pCell), pList, status);
        while(pList->nElementsCount > 0)
        {
            __PLIST_ENTRY pListEntry = GetFirstEntry(pList);
            if (!pListEntry)
            {
                DeleteList(pList);
                return ENTRY_NOT_FOUND;
            }
            nRow = (pListEntry->pCell - pGrid->pFirstCell) / pGrid->gridParameters.nCols;
            nCol = (pListEntry->pCell - pGrid->pFirstCell) % pGrid->gridParameters.nCols;
            RETURN_ON_FAILURE(DeleteFirstEntry(pList), pList, status);

            //
            // Si la cellule a un champ nValue nul (pas de cellules min�es autour), on d�voile son entourage.
            // On r�applique le m�me proc�d� aux cellules voisines qui ont un champ nValue nul.
            //

            RETURN_ON_FAILURE(RevealAndPushCell (pList, pGrid, nRow - 1, nCol - 1), pList, status);
            RETURN_ON_FAILURE(RevealAndPushCell (pList, pGrid, nRow - 1, nCol    ), pList, status);
            RETURN_ON_FAILURE(RevealAndPushCell (pList, pGrid, nRow - 1, nCol + 1), pList, status);
            RETURN_ON_FAILURE(RevealAndPushCell (pList, pGrid, nRow    , nCol - 1), pList, status);
            RETURN_ON_FAILURE(RevealAndPushCell (pList, pGrid, nRow    , nCol + 1), pList, status);
            RETURN_ON_FAILURE(RevealAndPushCell (pList, pGrid, nRow + 1, nCol - 1), pList, status);
            RETURN_ON_FAILURE(RevealAndPushCell (pList, pGrid, nRow + 1, nCol    ), pList, status);
            RETURN_ON_FAILURE(RevealAndPushCell (pList, pGrid, nRow + 1, nCol + 1), pList, status);
        }
        RETURN_ON_FAILURE(DeleteList(pList), NULL, status);
    }
    return SUCCESS;
}
