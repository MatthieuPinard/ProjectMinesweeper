#include "list.h"

/*++

    Cette fonction initialise une liste.
    Elle renvoie NULL en cas d'échec, un pointeur sur la liste créée sinon.

--*/

PLIST InitializeNewList (VOID)
{
    PLIST pList = malloc(sizeof(LIST));
    if (pList)
    {
        pList->nElementsCount = 0U;
        pList->pHead = NULL;
        return pList;
    }
    return NULL;
}

/*++

    Cette fonction libère la mémoire utilisée par une liste.
    Elle renvoie INVALID_LIST_REFERENCE en cas d'échec
    (pList était un pointeur NULL), SUCCESS sinon.
    En cas d'échec, la liste est inchangée.

--*/

STATUS DeleteList (PLIST pList)
{
    if (pList)
    {
        __PLIST_ENTRY pCurrentEntry = pList->pHead, pNextEntry;
        while(pCurrentEntry)
        {
            /*++

                On libère de la mémoire chaque entrée de la liste.

            --*/

            pNextEntry = pCurrentEntry->pNextEntry;
            free(pCurrentEntry);
            pCurrentEntry = pNextEntry;
        }
        free(pList);
        return SUCCESS;
    }
    return INVALID_LIST_REFERENCE;
}

/*++

    Cette fonction ajoute un pointeur sur une cellule à la liste.
    Arguments :
        - pList : le pointeur sur la liste.
        - pCell : le pointeur sur la cellule.

    Elle renvoie MEMORY_ERROR si l'allocation de mémoire pour un nouvel élément à la liste
    a échoué, INVALID_LIST_REFERENCE si pList était NULL, SUCCESS sinon.
    En cas d'échec, la list est inchangée.

--*/

STATUS AddEntryToList (PLIST pList, PCELL pCell)
{
    if(pList)
    {
        __PLIST_ENTRY pNewEntry = malloc(sizeof(__LIST_ENTRY));
        if (!pNewEntry)
        {
            return MEMORY_ERROR;
        }
        pNewEntry->pCell = pCell;
        pNewEntry->pNextEntry = pList->pHead;
        pList->pHead = pNewEntry;
        pList->nElementsCount++;
        return SUCCESS;
    }
    return INVALID_LIST_REFERENCE;
}

/*++

    Renvoie le nombre d'entrées dans la liste.
    Arguments :
        pList - le pointeur sur la liste.
    0 est renvoyé en cas d'échec.

--*/

UINT GetListSize (PLIST pList)
{
    return pList ? pList->nElementsCount : 0;
}

/*++

    Renvoie la tête de la liste à l'aide d'un pointeur sur une structure LIST_ENTRY.
    Arguments :
        pList - le pointeur sur la liste.
    NULL est renvoyé en cas d'échec.

--*/

__PLIST_ENTRY GetFirstEntry (PLIST pList)
{
    return pList ? pList->pHead : NULL;
}

/*++

    Cette fonction supprime la tête de la liste de la liste pointée par pList
    et libère la mémoire utilisée par la tête de la liste.
    Elle renvoie ENTRY_NOT_FOUND en cas d'échec
    (pList était un pointeur NULL ou la tête de pList était NULL), SUCCESS sinon.
    En cas d'échec, la liste est inchangée.

--*/

STATUS DeleteFirstEntry (PLIST pList)
{
    if (pList && pList->pHead)
    {
        __PLIST_ENTRY pNewHead = pList->pHead->pNextEntry;
        free(pList->pHead);
        pList->pHead = pNewHead;
        pList->nElementsCount--;
        return SUCCESS;
    }
    return ENTRY_NOT_FOUND;
}
