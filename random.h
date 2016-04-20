/*++

    Ce module impl�mente des fonctions n�cessaires � la manipulation des nombres al�atoires.

--*/

#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <windows.h>
#include <limits.h>
#include "types.h"

VOID InitializeRandomGenerator(VOID);

VOID ReleaseRandomGenerator(VOID);

UINT GenerateRandomNumber (UINT);

#endif
