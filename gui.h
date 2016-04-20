/*++

    Ce module implémente la partie graphique du programme.

--*/

#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <CommCtrl.h>
#include "status.h"
#include "types.h"

#define MINE                                                                -1
#define CELL_SIZE_PX                                                        32
#define SDL_LOAD_BMP(x)                                                     SDL_LoadBMP("img\\"#x".bmp")
#define MAX_CELL_VALUE                                                      8
#define OFFSET(x, y, z)                                                     (((y) * (x)) + (z))

#define EXIT																IDCANCEL
#define PLAY_AGAIN															(IDCANCEL + 1)
#define RESTART_GAME														(IDCANCEL + 2)

#define BEGINNER															(IDCANCEL + 1)
#define INTERMEDIATE														(IDCANCEL + 2)
#define ADVANCED															(IDCANCEL + 3)

VOID IconWorkaround(SDL_Window *);

STATUS PrintGrid (PGRID, BOOL);

STATUS InitializeGUIRender(PGRID_PARAMETERS);

VOID TerminateGUIRender(VOID);

CELL_PARAMETERS CaptureClickCellParameters(PGRID);

STATUS HighlightNewCell(CELL_PARAMETERS, PGRID);

VOID UpdateTime(LPCRITICAL_SECTION);

VOID ReplayWindow(PINT, GAME_STATUS);

VOID GridSelectWindow(PINT);

VOID UpdateTitle(LPCRITICAL_SECTION, INT);

VOID StopTimeThread(VOID);

#endif
