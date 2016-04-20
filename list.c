#include "list.h"

/*++

    Cette fonction initialise une liste.
    Elle renvoie NULL en cas d'�chec, un pointeur sur la liste cr��e sinon.

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

    Cette fonction lib�re la m�moire utilis�e par une liste.
    Elle renvoie INVALID_LIST_REFERENCE en cas d'�chec
    (pList �tait un pointeur NULL), SUCCESS sinon.
    En cas d'�chec, la liste est inchang�e.

--*/

STATUS DeleteList (PLIST pList)
{
    if (pList)
    {
        __PLIST_ENTRY pCurrentEntry = pList->pHead, pNextEntry;
        while(pCurrentEntry)
        {
            /*++

                On lib�re de la m�moire chaque entr�e de la liste.

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

    Cette fonction ajoute un pointeur sur une cellule � la liste.
    Arguments :
        - pList : le pointeur sur la liste.
        - pCell : le pointeur sur la cellule.

    Elle renvoie MEMORY_ERROR si l'allocation de m�moire pour un nouvel �l�ment � la liste
    a �chou�, INVALID_LIST_REFERENCE si pList �tait NULL, SUCCESS sinon.
    En cas d'�chec, la list est inchang�e.

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

    Renvoie le nombre d'entr�es dans la liste.
    Arguments :
        pList - le pointeur sur la liste.
    0 est renvoy� en cas d'�chec.

--*/

UINT GetListSize (PLIST pList)
{
    return pList ? pList->nElementsCount : 0;
}

/*++

    Renvoie la t�te de la liste � l'aide d'un pointeur sur une structure LIST_ENTRY.
    Arguments :
        pList - le pointeur sur la liste.
    NULL est renvoy� en cas d'�chec.

--*/

__PLIST_ENTRY GetFirstEntry (PLIST pList)
{
    return pList ? pList->pHead : NULL;
}

/*++

    Cette fonction supprime la t�te de la liste de la liste point�e par pList
    et lib�re la m�moire utilis�e par la t�te de la liste.
    Elle renvoie ENTRY_NOT_FOUND en cas d'�chec
    (pList �tait un pointeur NULL ou la t�te de pList �tait NULL), SUCCESS sinon.
    En cas d'�chec, la liste est inchang�e.

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
