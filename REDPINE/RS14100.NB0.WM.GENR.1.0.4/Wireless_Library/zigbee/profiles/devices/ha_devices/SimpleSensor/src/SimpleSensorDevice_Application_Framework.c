/*******************************************************************************
* SimpleSensorDevice_Application_Framework.c
********************************************************************************
* Program Description:
*   This module provides all the functionality for Simple sensor device event
*   handling
*******************************************************************************/

/*-----------------------------------------------------------------------------
* Includes
*----------------------------------------------------------------------------*/

#include "ZCL_Foundation.h"
#include "zdp_interface.h"
#include "SimpleSensorDevice_Application_Framework.h"
#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Basic_Cluster.h"
#endif                              /* g_BASIC_CLUSTER_ENABLE_d */
#if(g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
#include "ZCL_General_Power_Config_Cluster.h"
#endif                              /* g_POWER_CONFIGURATION_CLUSTER_ENABLE_d */
#if(g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1)
#include "ZCL_General_Device_Temp_Config_Cluster.h"
#endif                              /* g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d */
#if(g_ALARMS_CLUSTER_ENABLE_d ==1 )
#include "ZCL_General_Alarms_Cluster.h"
#endif                              /* g_ALARMS_CLUSTER_ENABLE_d */
#if(g_IDENTIFY_CLUSTER_ENABLE_d ==1 )
#include "ZCL_General_Identify_Cluster.h"
#endif                              /* g_IDENTIFY_CLUSTER_ENABLE_d */
#if(g_GROUPS_CLUSTER_ENABLE_d ==1 )
#include "ZCL_General_Groups_Cluster.h"
#endif                              /* g_GROUPS_CLUSTER_ENABLE_d */
#if ( g_BINARY_INPUT_CLUSIER_ENABLE_d == 1)
#include "ZCL_General_Binary_Input_Cluster.h"
#endif                                      /* g_BINARY_INPUT_CLUSIER_ENABLE_d */
#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1)
#include "ZCL_Simple_Metering_Cluster.h"
#endif                                      /* g_SIMPLE_METERING_CLUSTER_ENABLE_d */
#include "ApplicationThinLayer.h"
#include "app_configuration_default.h"
/*-----------------------------------------------------------------------------
* Global Constants
*----------------------------------------------------------------------------*/
#define m_BROADCAST_END_POINT_c						    0xFF


/*-----------------------------------------------------------------------------
* Public Memory declarations
*----------------------------------------------------------------------------*/
/* None */

/*-----------------------------------------------------------------------------
* Private Memory declarations
*----------------------------------------------------------------------------*/
const cluster_id_t a_In_Cluster_List_For_Simple_Sensor[] =
{
#if ( g_BASIC_CLUSTER_ENABLE_d == 1 )
   g_BASIC_CLUSTER_c,
#endif                                  /* g_BASIC_SERVER_CLUSTER_ENABLE_d */
#if ( g_IDENTIFY_CLUSTER_ENABLE_d == 1 )
   g_IDENTIFY_CLUSTER_c,
#endif                                  /* g_IDENTIFY_SERVER_CLUSTER_ENABLE_d */
#if ( g_POWER_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
   g_POWER_CONFIGURATION_CLUSTER_c,
#endif                                  /* g_POWER_CONFIGURATION_SERVER_CLUSTER_ENABLE_d */
#if ( g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_ENABLE_d == 1 )
   g_DEVICE_TEMPERATURE_CONFIGURATION_CLUSTER_c,
#endif                                  /* g_DEVICE_TEMPERATURE_CONFIGURATION_SERVER_CLUSTER_ENABLE_d */
#if ( g_ALARMS_CLUSTER_ENABLE_d == 1 )
   g_ALARMS_CLUSTER_c,
#endif                                  /* g_ALARMS_SERVER_CLUSTER_ENABLE_d */
#if ( g_BINARY_INPUT_CLUSIER_ENABLE_d == 1 )
   g_BINARY_INPUT_CLUSTER_c
#endif                                  /* g_BINARY_INPUT_SERVER_CLUSTER_ENABLE_d */
};


const cluster_id_t a_Out_Cluster_List_For_Simple_Sensor[] =
{
#if ( g_SIMPLE_METERING_CLUSTER_ENABLE_d == 1 )
   g_SIMPLE_METERING_CLUSTER_c
#endif                                  /* g_SIMPLE_METERING_SERVER_CLUSTER_ENABLE_d */
};

/* simple descriptor for Simple Sensor */
const Simple_Descriptor_t Simple_Sensor_Simple_Desc =
{	
	g_APP_PROFILE_ID_c,
	g_SIMPLE_SENSOR_c,
	g_APS_DEVICE_VERSION_FLAGS_c,
    sizeof(a_In_Cluster_List_For_Simple_Sensor) /	sizeof(cluster_id_t),	
	(cluster_id_t*)a_In_Cluster_List_For_Simple_Sensor,
	sizeof(a_Out_Cluster_List_For_Simple_Sensor) / sizeof(cluster_id_t),
	(cluster_id_t*)a_Out_Cluster_List_For_Simple_Sensor
};

/*---------------------------------------------------------------------------*/
Endpoint_Description_t  ga_Endpoint_Descriptors[g_MAX_NO_OF_ENDPOINTS_c] =
{
	{ (Simple_Descriptor_t*)&Simple_Sensor_Simple_Desc,
            g_SIMPLE_SENSOR_ENDPOINT_c },
};

Endpoint_Description_t  g_Broadcast_Endpoint_Descriptors =
{
	g_NULL_c, m_BROADCAST_END_POINT_c
};

Endpoint_Description_t *gp_Endpoint_Descriptors = ga_Endpoint_Descriptors;
/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Public Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Private Functions
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* Interrupt Service Routines
*----------------------------------------------------------------------------*/

/* None */

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
