/*++

    Ce module définit des alias de type.

--*/

#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

typedef unsigned int UINT, STATUS;

typedef enum
{
    CELL_HIDDEN,
    CELL_REVEALED,
    CELL_IS_FLAG
} CELL_STATUS;

typedef struct
{
	UINT nCol;
	UINT nRow;
	BOOL bIsFlag;
} CELL_PARAMETERS;

typedef struct
{
    int nValue;
    CELL_STATUS state;
} CELL, *PCELL, **PPCELL;

typedef struct
{
    UINT nRows;
    UINT nCols;
    UINT nMines;
} GRID_PARAMETERS, *PGRID_PARAMETERS;

typedef struct
{
    PCELL pFirstCell;
	GRID_PARAMETERS gridParameters;
} GRID, *PGRID;

typedef enum
{
	GAME_UNDEFINED,
	GAME_WIN,
	GAME_LOSS,
	GAME_IN_PROGRESS
} GAME_STATUS;

typedef struct __LIST_ENTRY
{
	PCELL pCell;
	struct __LIST_ENTRY * pNextEntry;
} __LIST_ENTRY, *__PLIST_ENTRY;

typedef struct
{
	UINT nElementsCount;
	__PLIST_ENTRY pHead;
} LIST, *PLIST;

#endif
