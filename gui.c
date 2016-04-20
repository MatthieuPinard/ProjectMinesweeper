#include "gui.h"


static SDL_Texture     * apRevealedCells          [MAX_CELL_VALUE + 1];
static SDL_Texture     * pFlagCell                              = NULL;
static SDL_Texture     * pHighlightedFlagCell                   = NULL;
static SDL_Texture     * pHiddenCell                            = NULL;
static SDL_Texture     * pHighlightedHiddenCell                 = NULL;
static SDL_Texture     * pMineCell                              = NULL;
static SDL_Renderer    * pRenderer                              = NULL;
static SDL_Window	   * pWindow								= NULL;
static CELL_PARAMETERS   CurrentCellParameters                        ;
static LARGE_INTEGER	 liStartTime;
static unsigned int		 nCurrTime;
static int				 nRemainingMines;
static volatile BOOL	 bTimeThreadRunning;

/*++

	Workaround for Windows platforms, so the top left of the window have the proper icon.

--*/
VOID IconWorkaround(SDL_Window * __pWindow)
{
#ifdef WIN32
	HICON hIcon = LoadIcon(GetModuleHandle(NULL), "MAINICON");
	if (hIcon != NULL)
	{
		SDL_SysWMinfo WMinfo;
		SDL_VERSION(&WMinfo.version);
		if (SDL_GetWindowWMInfo(__pWindow, &WMinfo) == 1)
		{
			HWND hWnd = WMinfo.info.win.window;
			SetClassLong(hWnd, GCL_HICON, (LONG)hIcon);
		}
	}
#endif
}

STATUS InitializeGUIRender(PGRID_PARAMETERS pGridParameters)
{
    char szBMPPath [MAX_PATH];
    if (!pGridParameters)
    {
        return INVALID_ARGS;
    }
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return GUI_INITIALIZATION_ERROR;
    }
    memset(&CurrentCellParameters, 0, sizeof(CELL_PARAMETERS));
    pWindow = SDL_CreateWindow(NULL,
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               pGridParameters->nCols * CELL_SIZE_PX,
                               pGridParameters->nRows * CELL_SIZE_PX,
                               SDL_WINDOW_ALLOW_HIGHDPI);
    pRenderer = SDL_CreateRenderer(pWindow,
                                   -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	IconWorkaround(pWindow);
    for (int i = 0; i <= MAX_CELL_VALUE; i++)
    {
        _snprintf_s(szBMPPath, MAX_PATH, _TRUNCATE, "img\\%d.bmp", i);
        apRevealedCells[i] = SDL_CreateTextureFromSurface(pRenderer, SDL_LoadBMP(szBMPPath));
        if (!apRevealedCells[i])
        {
            SDL_Quit();
            return MEMORY_ERROR;
        }
    }
    pFlagCell = SDL_CreateTextureFromSurface(pRenderer, SDL_LOAD_BMP(FLAG));
    pHiddenCell = SDL_CreateTextureFromSurface(pRenderer, SDL_LOAD_BMP(HIDDEN));
    pMineCell = SDL_CreateTextureFromSurface(pRenderer, SDL_LOAD_BMP(MINE));
    pHighlightedFlagCell = SDL_CreateTextureFromSurface(pRenderer, SDL_LOAD_BMP(HIGHLIGHTED_FLAG));
    pHighlightedHiddenCell = SDL_CreateTextureFromSurface(pRenderer, SDL_LOAD_BMP(HIGHLIGHTED_HIDDEN));
	if (!pWindow || !pFlagCell ||
        !pHiddenCell || !pMineCell ||
        !pHighlightedFlagCell || !pHighlightedHiddenCell)
    {
        SDL_Quit();
        return MEMORY_ERROR;
    }
	nRemainingMines = pGridParameters->nMines;
	bTimeThreadRunning = TRUE;
    return SUCCESS;
}

VOID TerminateGUIRender()
{
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
}

/*++

    Affiche la grille dans la console.

    Arguments :
        pGrid - Le pointeur sur la grille.

    Renvoie INVALID_ARGS si l'argument passé à la fonction est invalide.
            SUCCESS sinon.

--*/

STATUS PrintGrid (PGRID pGrid, BOOL bRevealMines)
{
    if (!pGrid || !pGrid->pFirstCell)
    {
        return INVALID_ARGS;
    }
    SDL_Rect CurrentRect;
    PCELL pCurrentCell = pGrid->pFirstCell;
    PCELL pEndCell = pCurrentCell + (pGrid->gridParameters.nRows * pGrid->gridParameters.nCols);
    CurrentRect.h = CELL_SIZE_PX;
    CurrentRect.w = CELL_SIZE_PX;
    for(; pCurrentCell != pEndCell; pCurrentCell++)
    {
        CurrentRect.x = ((pCurrentCell - pGrid->pFirstCell) % pGrid->gridParameters.nCols) * CELL_SIZE_PX;
        CurrentRect.y = ((pCurrentCell - pGrid->pFirstCell) / pGrid->gridParameters.nCols) * CELL_SIZE_PX;
        if(pCurrentCell->state == CELL_HIDDEN && !(pCurrentCell->nValue == MINE && bRevealMines))
        {
            SDL_RenderCopy(pRenderer, pHiddenCell, NULL, &CurrentRect);
        }
        else if (pCurrentCell->state == CELL_IS_FLAG && !(pCurrentCell->nValue != MINE && bRevealMines))
        {
            SDL_RenderCopy(pRenderer, pFlagCell, NULL, &CurrentRect);
        }

        //
        // Cas CELL_REVEALED
        //

        else
        {
            pCurrentCell->nValue == MINE ?
            SDL_RenderCopy(pRenderer, pMineCell, NULL, &CurrentRect) :
            SDL_RenderCopy(pRenderer, apRevealedCells[pCurrentCell->nValue], NULL, &CurrentRect);
        }
    }
    SDL_RenderPresent(pRenderer);
    return SUCCESS;
}

STATUS HighlightNewCell(CELL_PARAMETERS CellParameters, PGRID pGrid)
{
    if (!pGrid || !pGrid->pFirstCell ||
        CellParameters.nCol >= pGrid->gridParameters.nCols || CellParameters.nRow >= pGrid->gridParameters.nRows)
    {
        return INVALID_ARGS;
    }
    SDL_Rect Rect;
    PCELL pCell;
    BOOL bHasGridStateChanged = FALSE;
    Rect.h = CELL_SIZE_PX;
    Rect.w = CELL_SIZE_PX;
    Rect.x = CurrentCellParameters.nCol * CELL_SIZE_PX;
    Rect.y = CurrentCellParameters.nRow * CELL_SIZE_PX;
    pCell = pGrid->pFirstCell + OFFSET(pGrid->gridParameters.nCols,
                                       CurrentCellParameters.nRow,
                                       CurrentCellParameters.nCol);
    if(pCell->state == CELL_IS_FLAG)
    {
        SDL_RenderCopy(pRenderer, pFlagCell, NULL, &Rect);
        bHasGridStateChanged = TRUE;
    }
    else if (pCell->state == CELL_HIDDEN)
    {
        SDL_RenderCopy(pRenderer, pHiddenCell, NULL, &Rect);
        bHasGridStateChanged = TRUE;
    }
    Rect.x = CellParameters.nCol * CELL_SIZE_PX;
    Rect.y = CellParameters.nRow * CELL_SIZE_PX;
    pCell = pGrid->pFirstCell + OFFSET(pGrid->gridParameters.nCols,
                                       CellParameters.nRow,
                                       CellParameters.nCol);
    if(pCell->state == CELL_IS_FLAG)
    {
        SDL_RenderCopy(pRenderer, pHighlightedFlagCell, NULL, &Rect);
        bHasGridStateChanged = TRUE;
    }
    else if (pCell->state == CELL_HIDDEN)
    {
        SDL_RenderCopy(pRenderer, pHighlightedHiddenCell, NULL, &Rect);
        bHasGridStateChanged = TRUE;
    }
    CurrentCellParameters = CellParameters;
    if (bHasGridStateChanged)
    {
        SDL_RenderPresent(pRenderer);
    }
    return SUCCESS;
}

CELL_PARAMETERS CaptureClickCellParameters(PGRID pGrid)
{
    SDL_Event Event;
	CELL_PARAMETERS CellParameters = { 0 };
    BOOL bHasCapturedClick = FALSE;
    while(!bHasCapturedClick)
    {
        SDL_WaitEvent(&Event);
        if (Event.type == SDL_MOUSEMOTION)
        {
            CellParameters.nCol = Event.motion.x / CELL_SIZE_PX;
            CellParameters.nRow = Event.motion.y / CELL_SIZE_PX;
            HighlightNewCell(CellParameters, pGrid);
        }
        else if (Event.type == SDL_MOUSEBUTTONUP)
        {
            CellParameters.nCol = Event.button.x / CELL_SIZE_PX;
            CellParameters.nRow = Event.button.y / CELL_SIZE_PX;
            if (Event.button.button == SDL_BUTTON_RIGHT)
            {
                CellParameters.bIsFlag = TRUE;
                bHasCapturedClick = TRUE;
            }
            else if (Event.button.button == SDL_BUTTON_LEFT)
            {
                CellParameters.bIsFlag = FALSE;
                bHasCapturedClick = TRUE;
            }
        }
        else if (Event.type == SDL_QUIT)
        {
            TerminateGUIRender();
            exit(SUCCESS);
        }
    }
    return CellParameters;
}

HRESULT CALLBACK TaskDialogCallbackProc(
	_In_ HWND     hWnd,
	_In_ UINT     uNotification,
	_In_ WPARAM   wParam,
	_In_ LPARAM   lParam,
	_In_ LONG_PTR dwRefData
	)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	UNREFERENCED_PARAMETER(dwRefData);
	if (uNotification == TDN_BUTTON_CLICKED)
	{
		return S_OK;
	}
	return S_FALSE;
}

VOID GridSelectWindow(PINT pGridChoice)
{
	TASKDIALOG_BUTTON aButtons[3] = { { BEGINNER, L"Beginner\n10 mines\n9 x 9 tile grid" },
	{ INTERMEDIATE, L"Intermediate\n40 mines\n16 x 16 tile grid" },
	{ ADVANCED, L"Advanced\n99 mines\n16 x 30 tile grid" } };
	TASKDIALOGCONFIG DlgConfig;
	memset(&DlgConfig, 0, sizeof(TASKDIALOGCONFIG));
	DlgConfig.cbSize				= sizeof(TASKDIALOGCONFIG);
	DlgConfig.dwFlags				= TDF_ALLOW_DIALOG_CANCELLATION | TDF_USE_COMMAND_LINKS;
	DlgConfig.pszWindowTitle		= L"Minesweeper";
	DlgConfig.hInstance				= GetModuleHandle(NULL);
	DlgConfig.pszMainIcon			= L"MAINICON";
	DlgConfig.pszMainInstruction	= L"Please select a difficulty.";
	DlgConfig.cButtons				= 3;
	DlgConfig.pButtons				= aButtons;
	DlgConfig.nDefaultButton		= BEGINNER;
	DlgConfig.pfCallback			= TaskDialogCallbackProc;
	TaskDialogIndirect(&DlgConfig, pGridChoice, NULL, NULL);
}

VOID ReplayWindow(PINT pReplayChoice, GAME_STATUS GameStatus)
{
	WCHAR szMessage[BUFSIZ];
	TASKDIALOG_BUTTON aButtons[3] = { { EXIT, L"Exit" }, { PLAY_AGAIN, L"Play Again" }, { RESTART_GAME, L"Restart" } };
	TASKDIALOGCONFIG DlgConfig;
	swprintf_s(szMessage, BUFSIZ, (nCurrTime > 1) ? L"Time: %u seconds\n" : L"Time: %u second\n", nCurrTime);
	memset(&DlgConfig, 0, sizeof(TASKDIALOGCONFIG));
	DlgConfig.cbSize				= sizeof(TASKDIALOGCONFIG);
	DlgConfig.dwFlags				= TDF_ALLOW_DIALOG_CANCELLATION | TDF_USE_COMMAND_LINKS;
	DlgConfig.pszWindowTitle		= L"Minesweeper";
	DlgConfig.hInstance				= GetModuleHandle(NULL);
	DlgConfig.pszMainIcon			= L"MAINICON";
	DlgConfig.pszMainInstruction	= (GameStatus == GAME_WIN) ? L"Congratulations, you won the game!" : L"Sorry, you lost this game.\nBetter luck next time!";
	DlgConfig.cButtons				= (GameStatus == GAME_WIN) ? 2 : 3;
	DlgConfig.pszContent			= szMessage;
	DlgConfig.pButtons				= aButtons;
	DlgConfig.nDefaultButton		= EXIT;
	DlgConfig.pfCallback			= TaskDialogCallbackProc;
	TaskDialogIndirect(&DlgConfig, pReplayChoice, NULL, NULL);
}

VOID UpdateTitle(LPCRITICAL_SECTION pCriticalSection, INT Addend) {
	char szTitle[BUFSIZ];
	LARGE_INTEGER liCurrentTime, liCurrentFrequency;
	QueryPerformanceCounter(&liCurrentTime);
	QueryPerformanceFrequency(&liCurrentFrequency);
	EnterCriticalSection(pCriticalSection);
	nRemainingMines += Addend;
	nCurrTime = (UINT)((liCurrentTime.QuadPart - liStartTime.QuadPart) / liCurrentFrequency.QuadPart);
	_snprintf_s(szTitle,
		BUFSIZ,
		_TRUNCATE,
		"%u seconds - %d remaining mines - Minesweeper",
		nCurrTime,
		nRemainingMines);
	SDL_SetWindowTitle(pWindow, szTitle);
	LeaveCriticalSection(pCriticalSection);
}

VOID UpdateTime(LPCRITICAL_SECTION pCriticalSection)
{
	QueryPerformanceCounter(&liStartTime);
	for (;bTimeThreadRunning;)
	{
		UpdateTitle(pCriticalSection, 0);
		Sleep(1000);
	}
	// The Critical Section is not held as no subsequent calls to UpdateTitle can occur yet.
	WaitForSingleObject(pCriticalSection, INFINITE);
	DeleteCriticalSection(pCriticalSection);
}

VOID StopTimeThread(VOID) {
	bTimeThreadRunning = FALSE;
}