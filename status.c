#include "status.h"

/*++

	Provides the user with application-defined error messages.
	Argument:
		Status: The error to display. 

--*/

void DisplayLastError(STATUS Status)
{
    switch(Status)
    {
        case INVALID_LIST_REFERENCE :
            ERROR_DISPLAY("The object was not properly initialized (pointing to NULL)."); 
			break;
        case ENTRY_NOT_FOUND :
            ERROR_DISPLAY("The requested element was not found."); 
			break;
        case MEMORY_ERROR :
            ERROR_DISPLAY("An error occured while allocating ressources.");
			break;
        case INVALID_ARGS :
            ERROR_DISPLAY("Incorrect parameters have been passed to the routine."); 
			break;
        case GUI_INITIALIZATION_ERROR :
            ERROR_DISPLAY("The GUI library failed to initialize."); 
			break;
        default : ;
    }
}
