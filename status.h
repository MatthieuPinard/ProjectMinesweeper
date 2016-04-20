/*++

    Ce module contient des définitions de codes d'erreurs et des macros pour l'affichage
    de messages d'erreurs sur la sortie d'erreur standard.

--*/

#ifndef STATUS_H_INCLUDED
#define STATUS_H_INCLUDED

#include <Windows.h>
#include "types.h"

/*++

    Ces macros définissent des statuts d'erreurs.

--*/

#define SUCCESS                     0
#define INVALID_LIST_REFERENCE      1
#define ENTRY_NOT_FOUND             2
#define MEMORY_ERROR                3
#define INVALID_ARGS                4
#define GUI_INITIALIZATION_ERROR    5

/*++

    Ces macros sont utilisées pour afficher une fenêtre
    d'erreur.

--*/

#ifdef _DEBUG
	#define _TO_STRING(X)  #X
	#define TO_STRING(X) _TO_STRING(X)
	#define ERROR_DISPLAY(str) MessageBoxA(NULL, str "\nFile : "__FILE__ "\nLine : "TO_STRING(__LINE__), "Minesweeper", MB_ICONHAND)
#else
	#define ERROR_DISPLAY(str) MessageBoxA(NULL, str, "Minesweeper", MB_ICONHAND)
#endif

VOID DisplayLastError(STATUS);

#endif
