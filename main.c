#include "minesweeper.h"
 
INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow); 
    INT nReplayChoice, nGridChoice;
    CRITICAL_SECTION CriticalSection;
    GRID_PARAMETERS GridParameters;
    PGRID pGrid;
    PPCELL apMines;
    HANDLE hTimeThread; 
    BOOL bPlayAgain, bRevealWholeGrid;
    CELL_PARAMETERS CellParameters;
	nReplayChoice = EXIT;
    bPlayAgain = TRUE;
    pGrid = NULL;
    InitializeRandomGenerator();
    do
    {
        if (nReplayChoice == RESTART_GAME)
        {
            RewindGrid(pGrid);
        }
        else
        {
            GridSelectWindow(&nGridChoice);
            switch(nGridChoice)
            {
                case BEGINNER:
                    InitializeGridParameters(9, 9, 10, &GridParameters);
                    break;
                case INTERMEDIATE:
					InitializeGridParameters(16, 16, 40, &GridParameters);
                    break;
                case ADVANCED:
					InitializeGridParameters(16, 30, 99, &GridParameters);
                    break;
                default:
                    return 1;
            }
            pGrid = InitializeGrid(&GridParameters);
            apMines = FillGridWithMines(pGrid);
            ComputeCellValues(pGrid, apMines);
        }
        if (InitializeGUIRender(&GridParameters) != SUCCESS)
        {
            return 1;
        }
        PrintGrid(pGrid, bRevealWholeGrid = FALSE);
        InitializeCriticalSection(&CriticalSection);
        hTimeThread = CreateThread(NULL,
                                   0,
                                   (LPTHREAD_START_ROUTINE) UpdateTime,
                                   &CriticalSection,
                                   0,
                                   NULL);
        while(HasPlayerWon(pGrid) == GAME_IN_PROGRESS)
        {
            CellParameters = CaptureClickCellParameters(pGrid);
            PlayOneStep(pGrid, CellParameters, &CriticalSection);
        }
        StopTimeThread();
        PrintGrid(pGrid, bRevealWholeGrid = TRUE);
        ReplayWindow(&nReplayChoice, HasPlayerWon(pGrid));
        if (nReplayChoice == EXIT)
        {
            bPlayAgain = FALSE;
        }
        if (nReplayChoice != RESTART_GAME)
        {
            FreeGrid(pGrid);
        }
        TerminateGUIRender();
    } while(bPlayAgain);
	ReleaseRandomGenerator();
    return 0;
}
