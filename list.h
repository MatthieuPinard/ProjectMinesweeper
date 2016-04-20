/*++

    Ce module implémente des listes de pointeurs sur des cellules, ainsi que
    des fonctions de manipulation sur ces listes.

--*/

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdlib.h>
#include "status.h"
#include "types.h"

PLIST InitializeNewList (VOID);

STATUS DeleteList (PLIST);

STATUS AddEntryToList (PLIST, PCELL);

UINT GetListSize (PLIST);

__PLIST_ENTRY GetFirstEntry (PLIST);

STATUS DeleteFirstEntry (PLIST);

#endif
