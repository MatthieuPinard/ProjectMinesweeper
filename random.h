/*++

    Ce module implémente des fonctions nécessaires à la manipulation des nombres aléatoires.

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
