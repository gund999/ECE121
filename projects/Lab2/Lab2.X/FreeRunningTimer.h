/* Template ver 1.0 W2022
 * File:   FreeRunningTimer.h
 * Author: Evan Draeger
 * Brief: 
 * Created on <March> <5>, <2023>, <11r> <pmm>
 * Modified on <month> <day>, <year>, <hour> <pm/am>
 */

#ifndef LIBRARYTEMPLATE_H // Header guard
#define	LIBRARYTEMPLATE_H //

/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/
#include "BOARD.h"
#include "xc.h" 
#include <sys/attribs.h> 
//#include "anotherOne.h"

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC TYPEDEFS                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/**
 * @Function FreeRunningTimer_Init(void)
 * @param void
 * @return void
 * @author Evan Draeger
 * @modified <Your Name>, <year>.<month>.<day> <hour> <pm/am> */
void FreeRunningTimer_Init(void);

/**
 * @Function FreeRunningTimer_GetMilliSeconds(void)
 * @param void
 * @return 32 bit int of time in ms
 * @author Evan Draeger
 * @modified <Your Name>, <year>.<month>.<day> <hour> <pm/am> */
unsigned int FreeRunningTimer_GetMilliSeconds(void);


/**
 * @Function FreeRunningTimer_GetMicroSeconds(void)
 * @param void
 * @return 32 bit int of time in us
 * @author Evan Draeger
 * @modified <Your Name>, <year>.<month>.<day> <hour> <pm/am> */
unsigned int FreeRunningTimer_GetMicroSeconds(void);
   

#endif	/* LIBRARYTEMPLATE_H */ // End of header guard

