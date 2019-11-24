/******************************************************************************
* MainsPowerOutletDevice.h
******************************************************************************
* Program Description:
* This file contains the MainsPowerOutlet Device
******************************************************************************/

#ifndef _MAINS_POWER_OUTLET_H_
#define _MAINS_POWER_OUTLET_H_

#include "ZCL_Foundation.h"

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

void Handle_ZCL_Message( uint8_t buffer_index, APSDE_Data_Indication_t *p_buffer );


#endif 									    /* _MAINS_POWER_OUTLET_H_*/

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
