/**
 * @file    rsi_ble_hids.c
 * @version 0.1
 * @date    16 Mar 2018
 *
 *  Copyright(C) Redpine Signals 2015
 *  All rights reserved by Redpine Signals.
 *
 *  @section License
 *  This program should be used on your own responsibility.
 *  Redpine Signals assumes no responsibility for any losses
 *  incurred by customers or third parties arising from the use of this file.
 *
 *  @brief : This file contains the HID service for a keyboard / mouse.
 *
 *  @section Description  This application connects as a peripheral and loops back the data
 *  received from the GATT client
 *
 */

/**
 * Include files
 * */

//! BLE include file to refer BLE APIs
#include<rsi_ble_apis.h>
#include<rsi_ble_config.h>
#include<rsi_bt_common_apis.h>

//! Common include file 
#include<rsi_common_apis.h>
#include <string.h>

//! [ble_gls] is a tag for every print
#ifdef RSI_DEBUG_PRINTS
#include <stdio.h>
#define LOG_PRINT(fmt, args...)                      printf("\n [ble_hids] " fmt, ##args)
#define SCANF(fmt, args...)                          scanf(fmt, ##args)
#else
#define LOG_PRINT(fmt, args...)
#define SCANF(fmt, args...)                         
#endif

//! Specify GATT role client/server
#define SERVER                                       0
#define CLIENT                                       1
#define GATT_ROLE                                    SERVER

//! remote device address
#define RANDOM_ADDRESS                               1
#define PUBLIC_ADDRESS                               0
#define RSI_BLE_REMOTE_BD_ADDRESS                    "F5:64:91:A2:F6:6F" //"00:23:A7:00:00:14"
#define RSI_BLE_REMOTE_BD_ADDRESS_TYPE               RANDOM_ADDRESS      //PUBLIC_ADDRESS

//! local device name
#define RSI_BLE_APP_HIDS                            "HID_OVER_GATT"

//! Remote Device Name to connect
#define RSI_REMOTE_DEVICE_NAME                      "Designer Keyboard"

//! local device IO capability
#define RSI_BLE_SMP_IO_CAPABILITY                   0x01
#define RSI_BLE_SMP_PASSKEY                         0x000000

//! attribute properties
#define RSI_BLE_ATT_PROP_RD                         0x02
#define RSI_BLE_ATT_PROP_WR_NO_RESP                 0x04
#define RSI_BLE_ATT_PROP_WR                         0x08
#define RSI_BLE_ATT_PROP_NOTIFY                     0x10
#define RSI_BLE_ATT_PROP_INDICATE                   0x20

//! application event list
#define RSI_APP_EVENT_ADV_REPORT                    0x00
#define RSI_BLE_EVENT_CONN                          0x01
#define RSI_BLE_EVENT_DISCONN                       0x02
#define RSI_BLE_EVENT_GATT_WR                       0x03
#define RSI_BLE_EVENT_GATT_RD                       0x04
#define RSI_BLE_EVENT_GATT_PROFILE_RESP             0x05
#define RSI_BLE_EVENT_GATT_CHAR_SERVICES_RESP       0x06
#define RSI_BLE_EVENT_GATT_CHAR_DESC_RESP           0x07
#define RSI_BLE_EVENT_MTU                           0x08
#define RSI_BLE_EVENT_SMP_REQ                       0x09
#define RSI_BLE_EVENT_SMP_RESP                      0x0a
#define RSI_BLE_EVENT_SMP_PASSKEY                   0x0b
#define RSI_BLE_EVENT_SMP_FAILED                    0x0c
#define RSI_BLE_EVENT_SMP_ENCRYPT_STARTED           0x0d
#define RSI_BLE_EVENT_SMP_PASSKEY_DISPLAY           0x0e
#define RSI_BLE_EVENT_LTK_REQ                       0x0f
#define RSI_BLE_SC_PASSKEY_EVENT                    0x10

#define GATT_READ_RESP                              0x00
#define GATT_READ_BLOB_RESP                         0x01

#define GATT_READ_ZERO_OFFSET                       0x00

//! Memory length for driver
#define BT_GLOBAL_BUFF_LEN                          10000

//! Memory to initialize driver
uint8_t bt_global_buf[BT_GLOBAL_BUFF_LEN];

#ifdef RSI_WITH_OS
//! BLE task stack size
#define RSI_BT_TASK_STACK_SIZE                      1000

//! BT task priority
#define RSI_BT_TASK_PRIORITY                        1

//! Wireless driver task priority
#define RSI_DRIVER_TASK_PRIORITY                    2

//! Wireless driver task stack size
#define RSI_DRIVER_TASK_STACK_SIZE                  3000

void rsi_wireless_driver_task(void);

#endif

//! BLE attribute service types uuid values
#define RSI_BLE_CHAR_SERV_UUID                      0x2803
#define RSI_BLE_CLIENT_CHAR_UUID                    0x2902
#define RSI_BLE_REPORT_REFERENCE_UUID               0x2908
#define RSI_BLE_EXT_REPORT_REFERENCE_UUID           0x2907
#define RSI_BLE_CHAR_PRESENTATION_FORMATE_UUID      0x2904

//! BLE characteristic service uuid
#define RSI_BLE_HID_SERVICE_UUID                    0x1812
#define RSI_BLE_HID_PROTOCOL_MODE_UUID              0x2A4E
#define RSI_BLE_HID_REPORT_UUID                     0x2A4D
#define RSI_BLE_HID_REPORT_MAP_UUID                 0x2A4B
#define RSI_BLE_HID_BOOT_MOUSE_IN_REPORT            0x2A33
#define RSI_BLE_HID_INFO_UUID                       0x2A4A
#define RSI_BLE_HID_CONTROL_POINT_UUID              0x2A4C

//! hid protocol mode
#define PROTOCOL_MODE_BOOT_UP                       0x00
#define PROTOCOL_MODE_REPORT                        0x01
#define HID_KDB_IN_RPT_DATA_LEN                     0x08
#define BD_ADDR_ARRAY_LEN                           0x18

//! global parameters list
static uint32_t ble_app_event_map;
static uint32_t ble_app_event_mask;

static rsi_ble_event_conn_status_t conn_event_to_app;
static rsi_ble_event_disconnect_t disconn_event_to_app;

static rsi_ble_event_write_t app_ble_write_event;
static rsi_ble_read_req_t app_ble_read_event;

static uint32_t  numeric_value;
static rsi_bt_event_encryption_enabled_t glbl_enc_enabled;
static rsi_bt_event_le_ltk_request_t temp_le_ltk_req;
static rsi_ble_event_mtu_t app_ble_mtu_event;

static uint16_t rsi_ble_hid_in_report_val_hndl;
static uint8_t remote_dev_addr[BD_ADDR_ARRAY_LEN];
static uint8_t device_found = 0;
uint16_t att_resp_status = 0;

//! enum for global state
typedef enum {
  REPORT_IN_NOTIFY_ENABLE,
  CONNECTED,
  ENCRYPT_EN,
  ADVERTISE,
  SCAN
} app_state_t;

app_state_t app_state = 0;

typedef struct rsi_ble_att_list_s{
  uint32_t  uuid;
  uint16_t  handle;
  uint16_t  len;         
  void     *value;         
}rsi_ble_att_list_t;

typedef struct rsi_ble_hid_info_s{
#define BLE_ATT_REC_SIZE         1024
#define BLE_ATT_REC_LIST_SIZE    5
  uint8_t   data[BLE_ATT_REC_SIZE];
  uint16_t  data_ix;
  uint16_t  att_rec_list_count;
  rsi_ble_att_list_t  att_rec_list[BLE_ATT_REC_LIST_SIZE];
}rsi_ble_hid_info_t;

static rsi_ble_hid_info_t hid_info_g;

static const uint8_t hid_report_map[] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    
    0x85, 0x01,                    //   REPORT_ID
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)

    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)

    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)

    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)

    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};

/*==============================================*/
/**
 * @fn         rsi_ble_app_init_events
 * @brief      initializes the event parameter.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function is used during BLE initialization.
 */
static void rsi_ble_app_init_events()
{
  ble_app_event_map = 0;
  ble_app_event_mask = 0xFFFFFFFF;
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_set_event
 * @brief      set the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to set/raise the specific event.
 */
static void rsi_ble_app_set_event(uint32_t event_num)
{
  ble_app_event_map |= BIT(event_num);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_clear_event
 * @brief      clears the specific event.
 * @param[in]  event_num, specific event number.
 * @return     none.
 * @section description
 * This function is used to clear the specific event.
 */
static void rsi_ble_app_clear_event(uint32_t event_num)
{
  ble_app_event_map &= ~BIT(event_num);
  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_app_get_event
 * @brief      returns the first set event based on priority
 * @param[in]  none.
 * @return     int32_t
 *             > 0  = event number
 *             -1   = not received any event
 * @section description
 * This function returns the highest priority event among all the set events
 */
static int32_t rsi_ble_app_get_event(void)
{
  uint32_t  ix;

  for (ix = 0; ix < 32; ix++)
  {
    if (ble_app_event_map & (1 << ix))
    {
       return ix;
    }
  }

  return (-1);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_adv_report_event
 * @brief      invoked when advertise report event is received
 * @param[in]  adv_report, pointer to the received advertising report
 * @return     none.
 * @section description
 * This callback function updates the scanned remote devices list
 */
void rsi_ble_on_adv_report_event(rsi_ble_event_adv_report_t *adv_report)
{
  static uint8_t remote_dev_addr[BD_ADDR_ARRAY_LEN] = {0};
  static uint8_t remote_addr_type = 0;
  static uint8_t remote_name[31] = {0};

  if (device_found == 1) {
    return;
  }

  BT_LE_ADPacketExtract (remote_name, adv_report->adv_data, adv_report->adv_data_len);

  remote_addr_type = adv_report->dev_addr_type;
  rsi_6byte_dev_address_to_ascii (remote_dev_addr, (uint8_t *)adv_report->dev_addr);

  LOG_PRINT ("\n Addr : %s, Type %d \n", remote_dev_addr, remote_addr_type);

  if((device_found==0) && (((strcmp(remote_name, RSI_REMOTE_DEVICE_NAME))==0) ||
      ((remote_addr_type == RSI_BLE_REMOTE_BD_ADDRESS_TYPE) && (strcmp(remote_dev_addr, RSI_BLE_REMOTE_BD_ADDRESS)==0))))
  { 
    device_found=1;
    rsi_ble_app_set_event (RSI_APP_EVENT_ADV_REPORT);
  }
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_enhance_conn_status_event
 * @brief      invoked when enhanced connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
void rsi_ble_on_enhance_conn_status_event(rsi_ble_event_enhance_conn_status_t *resp_enh_conn)
{
	conn_event_to_app.dev_addr_type = resp_enh_conn->dev_addr_type;
  memcpy (conn_event_to_app.dev_addr, resp_enh_conn->dev_addr, RSI_DEV_ADDR_LEN);
  conn_event_to_app.status = resp_enh_conn->status;
	rsi_ble_app_set_event (RSI_BLE_EVENT_CONN);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_connect_event
 * @brief      invoked when connection complete event is received
 * @param[out] resp_conn, connected remote device information
 * @return     none.
 * @section description
 * This callback function indicates the status of the connection
 */
static void rsi_ble_on_connect_event(rsi_ble_event_conn_status_t *resp_conn)
{
  LOG_PRINT ("conn \n");
  memcpy(&conn_event_to_app, resp_conn, sizeof(rsi_ble_event_conn_status_t));
  rsi_ble_app_set_event (RSI_BLE_EVENT_CONN);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_disconnect_event
 * @brief      invoked when disconnection event is received
 * @param[in]  resp_disconnect, disconnected remote device information
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This callback function indicates disconnected device information and status
 */
static void rsi_ble_on_disconnect_event(rsi_ble_event_disconnect_t *resp_disconnect, uint16_t reason)
{
  LOG_PRINT ("disconn \n");
  memcpy(&disconn_event_to_app, resp_disconnect, sizeof(rsi_ble_event_conn_status_t));
  rsi_ble_app_set_event (RSI_BLE_EVENT_DISCONN);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_gatt_write_event
 * @brief      its invoked when write/notify/indication events are received.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This callback function is invoked when write/notify/indication events are received
 */
static void rsi_ble_on_gatt_write_event (uint16_t event_id, rsi_ble_event_write_t *rsi_ble_write)
{
  LOG_PRINT ("gatt wr \n");
  memcpy (&app_ble_write_event, rsi_ble_write, sizeof (rsi_ble_event_write_t));
  rsi_ble_app_set_event (RSI_BLE_EVENT_GATT_WR);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_read_req_event
 * @brief      its invoked when read events are received.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_read, read event parameters.
 * @return     none.
 * @section description
 * This callback function is invoked when read events are received
 */
static void rsi_ble_on_read_req_event (uint16_t event_id, rsi_ble_read_req_t *rsi_ble_read_req)
{
  LOG_PRINT ("gatt rd \n");
  memcpy (&app_ble_read_event, rsi_ble_read_req, sizeof (rsi_ble_read_req_t));
  rsi_ble_app_set_event(RSI_BLE_EVENT_GATT_RD);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_profiles_event
 * @brief      invoked when the specific service details are received for
 *             supported specific services.
 * @param[out] p_ble_resp_profile, specific profile details
 * @return     none
 * @section description
 * This is a callback function
 */
static void rsi_ble_on_profiles_event (uint16_t resp_status, profile_descriptors_t *rsi_ble_resp_profile)
{
  att_resp_status = resp_status;
  rsi_ble_app_set_event (RSI_BLE_EVENT_GATT_PROFILE_RESP);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_char_services_event
 * @brief      invoked when response is received for characteristic services details
 * @param[out] p_ble_resp_char_services, list of characteristic services.
 * @return     none
 * @section description
 */
static void rsi_ble_on_char_services_event (uint16_t resp_status, rsi_ble_resp_char_services_t *rsi_ble_resp_char_services)
{
  att_resp_status = resp_status;
  rsi_ble_app_set_event (RSI_BLE_EVENT_GATT_CHAR_SERVICES_RESP);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_att_desc_event
 * @brief      its invoked when wr/noti/indi events are received.
 * @param[in]  event_id, it indicates wr/noti/indi event id.
 * @param[in]  rsi_ble_resp_att_desc, wr/noti/indi event parameters.
 * @return     none.
 * @section description
 * this callback function is invoked when wr/noti/indi events are received
 */
static void ble_on_att_desc_event (uint16_t resp_status, rsi_ble_resp_att_descs_t *rsi_ble_resp_att_desc)
{
  att_resp_status = resp_status;
  rsi_ble_app_set_event (RSI_BLE_EVENT_GATT_CHAR_DESC_RESP);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_request 
 * @brief      its invoked when smp request event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP request events is received(we are in Master mode)
 * Note: slave requested to start SMP request, we have to send SMP request command
 */
static void rsi_ble_on_smp_request (rsi_bt_event_smp_req_t *remote_dev_address)
{
  LOG_PRINT ("smp req\n");
  rsi_ble_app_set_event (RSI_BLE_EVENT_SMP_REQ);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_response 
 * @brief      its invoked when smp response event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP response events is received(we are in slave mode) 
 * Note: Master initiated SMP protocol, we have to send SMP response command
 */
static void rsi_ble_on_smp_response (rsi_bt_event_smp_resp_t *remote_dev_address)
{
  LOG_PRINT ("smp resp\n");
  rsi_ble_app_set_event (RSI_BLE_EVENT_SMP_RESP);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_passkey 
 * @brief      its invoked when smp passkey event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP passkey events is received
 * Note: We have to send SMP passkey command
 */
static void rsi_ble_on_smp_passkey (rsi_bt_event_smp_passkey_t *remote_dev_address)
{
  LOG_PRINT ("smp passkey\n");
  rsi_ble_app_set_event (RSI_BLE_EVENT_SMP_PASSKEY);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_passkey_display 
 * @brief      its invoked when smp passkey event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP passkey events is received
 * Note: We have to send SMP passkey command
 */
static void rsi_ble_on_smp_passkey_display (rsi_bt_event_smp_passkey_display_t *smp_passkey_display)
{
  LOG_PRINT ("smp passkey disp : %s\n", smp_passkey_display->passkey );
  rsi_ble_app_set_event (RSI_BLE_EVENT_SMP_PASSKEY_DISPLAY);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_smp_failed 
 * @brief      its invoked when smp failed event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when SMP failed events is received
 */
static void rsi_ble_on_smp_failed (uint16_t status, rsi_bt_event_smp_failed_t *remote_dev_address)
{
  LOG_PRINT ("smp failed status: %u\n", status);
  rsi_ble_app_set_event (RSI_BLE_EVENT_SMP_FAILED);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_encrypt_started 
 * @brief      its invoked when encryption started event is received.
 * @param[in]  remote_dev_address, it indicates remote bd address.
 * @return     none.
 * @section description
 * This callback function is invoked when encryption started events is received
 */
static void rsi_ble_on_encrypt_started (uint16_t resp_status, rsi_bt_event_encryption_enabled_t *enc_enabled)
{
  LOG_PRINT ("smp encrypt resp status : %u\n", resp_status);
  memcpy(&glbl_enc_enabled, enc_enabled, sizeof(rsi_bt_event_encryption_enabled_t));
  rsi_ble_app_set_event (RSI_BLE_EVENT_SMP_ENCRYPT_STARTED);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_le_ltk_req_event
 * @brief      invoked when disconnection event is received
 * @param[in]  resp_disconnect, disconnected remote device information
 * @param[in]  reason, reason for disconnection.
 * @return     none.
 * @section description
 * This callback function indicates linkkey request information
 */
static void rsi_ble_on_le_ltk_req_event(rsi_bt_event_le_ltk_request_t *le_ltk_req)
{
  LOG_PRINT ("smp ltk req\n");
  memcpy(&temp_le_ltk_req, le_ltk_req, sizeof(rsi_bt_event_le_ltk_request_t));
  rsi_ble_app_set_event (RSI_BLE_EVENT_LTK_REQ);
}

/*==============================================*/
/**
 * @fn         rsi_ble_on_mtu_event
 * @brief      its invoked when write/notify/indication events are received.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This callback function is invoked when write/notify/indication events are received
 */
static void rsi_ble_on_mtu_event (rsi_ble_event_mtu_t *rsi_ble_mtu)
{
  memcpy (&app_ble_mtu_event, rsi_ble_mtu, sizeof (rsi_ble_event_mtu_t));
  rsi_ble_app_set_event (RSI_BLE_EVENT_MTU);
}

/*==============================================*/
/**
 * @fn         rsi_gatt_add_att_to_list
 * @brief      This function is used to store characteristic service attribute.
 * @param[in]  p_hid_info, pointer to hid info structure
 * @param[in]  handle, characteristic service attribute handle.
 * @param[in]  data_len, characteristic value length
 * @param[in]  data, characteristic value pointer
 * @param[in]  uuid, characteristic value uuid
 * @return     none.
 * @section description
 * This function is used to store all attribute records  
 */
static void rsi_gatt_add_att_to_list (rsi_ble_hid_info_t *p_hid_info, uint16_t handle,
                                     uint16_t data_len, uint8_t *data, uint32_t uuid)
{
  LOG_PRINT ("gatt add att to list\n");
  if ((p_hid_info->data_ix + data_len) >= BLE_ATT_REC_SIZE)
  {
    LOG_PRINT ("no data memory for att rec values");
    return;
  }

  p_hid_info->att_rec_list[p_hid_info->att_rec_list_count].handle = handle;
  p_hid_info->att_rec_list[p_hid_info->att_rec_list_count].len = data_len;
  p_hid_info->att_rec_list[p_hid_info->att_rec_list_count].value = p_hid_info->data + p_hid_info->data_ix;
  memcpy (p_hid_info->data + p_hid_info->data_ix, data, data_len);
  p_hid_info->att_rec_list[p_hid_info->att_rec_list_count].uuid = uuid; 
  p_hid_info->att_rec_list_count++;
  p_hid_info->data_ix += data_len;

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_add_char_serv_att
 * @brief      this function is used to add characteristic service attribute..
 * @param[in]  serv_handler, service handler.
 * @param[in]  handle, characteristic service attribute handle.
 * @param[in]  val_prop, characteristic value property.
 * @param[in]  att_val_handle, characteristic value handle
 * @param[in]  att_val_uuid, characteristic value uuid
 * @return     none.
 * @section description
 * This function is used at application to add characteristic attribute
 */
static void rsi_ble_add_char_serv_att (void *serv_handler,
    uint16_t handle,
    uint8_t val_prop,
    uint16_t att_val_handle,
    uuid_t att_val_uuid)
{
  rsi_ble_req_add_att_t  new_att = {0};

  //! preparing the attribute service structure
  new_att.serv_handler = serv_handler;
  new_att.handle = handle;
  new_att.att_uuid.size = 2;
  new_att.att_uuid.val.val16 = RSI_BLE_CHAR_SERV_UUID;
  new_att.property = RSI_BLE_ATT_PROP_RD;

  //! preparing the characteristic attribute value
  new_att.data_len = 6;
  new_att.data[0] = val_prop;
  rsi_uint16_to_2bytes (&new_att.data[2], att_val_handle);
  rsi_uint16_to_2bytes (&new_att.data[4], att_val_uuid.val.val16);

  //! Add attribute to the service
  rsi_ble_add_attribute (&new_att);

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_add_char_val_att
 * @brief      this function is used to add characteristic value attribute.
 * @param[in]  serv_handler, new service handler.
 * @param[in]  handle, characteristic value attribute handle.
 * @param[in]  att_type_uuid, attribute uuid value.
 * @param[in]  val_prop, characteristic value property.
 * @param[in]  data, characteristic value data pointer.
 * @param[in]  data_len, characteristic value length.
 * @return     none.
 * @section description
 * This function is used at application to create new service.
 */
static uint32_t rsi_ble_add_char_val_att (
    rsi_ble_hid_info_t *p_hid_info,
    void *serv_handler,
    uint16_t handle,
    uuid_t   att_type_uuid,
    uint8_t  val_prop,
    uint8_t *data,
    uint16_t  data_len,
    uint8_t   auth_read)
{
  int32_t  status;
  rsi_ble_req_add_att_t  new_att = {0};


  //! preparing the attributes
  new_att.serv_handler = serv_handler;
  new_att.handle = handle;
  new_att.reserved = auth_read;
  memcpy (&new_att.att_uuid, &att_type_uuid, sizeof (uuid_t));
  new_att.property = val_prop;

  //! preparing the attribute value
  new_att.data_len = data_len;
  memcpy (new_att.data, data, RSI_MIN(sizeof (new_att.data), data_len));

  //! add attribute to the service
  status = rsi_ble_add_attribute (&new_att);
  if (status != RSI_SUCCESS) {
    return status;
  }
  
  if ((auth_read == 1) || (data_len > 20)) {
    if(!p_hid_info)
    {
      return RSI_FAILURE;
    }
    rsi_gatt_add_att_to_list (p_hid_info, handle, data_len, data, att_type_uuid.val.val32);
  }

  //! check the attribute property with notification
  if ((val_prop & RSI_BLE_ATT_PROP_NOTIFY) || (val_prop & RSI_BLE_ATT_PROP_INDICATE))
  {
    //! if notification property supports then we need to add client characteristic service.

    //! preparing the client characteristic attribute & values
    memset (&new_att, 0, sizeof(rsi_ble_req_add_att_t));
    new_att.serv_handler = serv_handler;
    new_att.handle = handle + 1;
    new_att.att_uuid.size = 2;
    new_att.att_uuid.val.val16 = RSI_BLE_CLIENT_CHAR_UUID;
    new_att.property = RSI_BLE_ATT_PROP_RD | RSI_BLE_ATT_PROP_WR | RSI_BLE_ATT_PROP_WR_NO_RESP;
    new_att.data_len = 2;

    //! add attribute to the service
    rsi_ble_add_attribute (&new_att);
  }

  return;
}

/*==============================================*/
/**
 * @fn         rsi_ble_add_hid_serv
 * @brief      this function is used to add new servcie i.e.., hids.
 * @param[in]  none.
 * @return     int32_t
 *             0  =  success
 *             !0 = failure
 * @section description
 * This function is used at application to create new service.
 */
static uint32_t rsi_ble_add_hid_serv (rsi_ble_hid_info_t *p_hid_info)
{
  uint8_t len = 0;
  uint8_t data[31] = {0};
  uuid_t  new_uuid = {0};
  uint8_t hdl_offset = 0;
  rsi_ble_resp_add_serv_t  new_serv_resp = {0};

  //! adding new service 
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_HID_SERVICE_UUID;
  rsi_ble_add_service (new_uuid, 17, 170, &new_serv_resp);
  hdl_offset += 1;


  //! Protocol mode
  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_HID_PROTOCOL_MODE_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + hdl_offset,
      RSI_BLE_ATT_PROP_RD | RSI_BLE_ATT_PROP_WR_NO_RESP,
      new_serv_resp.start_handle + hdl_offset + 1,
      new_uuid);

  //! adding characteristic value attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_HID_PROTOCOL_MODE_UUID;
  data[0] = PROTOCOL_MODE_REPORT; len = 1;
  rsi_ble_add_char_val_att (p_hid_info,
      new_serv_resp.serv_handler,
      new_serv_resp.start_handle + hdl_offset + 1,
      new_uuid,
      RSI_BLE_ATT_PROP_RD | RSI_BLE_ATT_PROP_WR_NO_RESP,
      (uint8_t *)data,
      len, 0);
  hdl_offset += 2;


  //! Keyboard input report
  //! adding characteristic service attribute to the service
  rsi_ble_hid_in_report_val_hndl = new_serv_resp.start_handle + hdl_offset + 1;
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_HID_REPORT_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + hdl_offset,
      RSI_BLE_ATT_PROP_RD | RSI_BLE_ATT_PROP_NOTIFY,
      new_serv_resp.start_handle + hdl_offset + 1,
      new_uuid);

  //! adding characteristic value attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_HID_REPORT_UUID;
  memset(data, 0, HID_KDB_IN_RPT_DATA_LEN);
  len = HID_KDB_IN_RPT_DATA_LEN;
  rsi_ble_add_char_val_att (p_hid_info,
      new_serv_resp.serv_handler,
      new_serv_resp.start_handle + hdl_offset + 1,
      new_uuid,
      RSI_BLE_ATT_PROP_RD | RSI_BLE_ATT_PROP_NOTIFY,
      (uint8_t *)data,
      len, 0);

  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_REPORT_REFERENCE_UUID;
  data[0] = 1; data[1] = 1;
  len = 2;
  rsi_ble_add_char_val_att (p_hid_info,
      new_serv_resp.serv_handler,
      new_serv_resp.start_handle + hdl_offset + 3,
      new_uuid,
      RSI_BLE_ATT_PROP_RD,
      (uint8_t *)data,
      len, 0);
  hdl_offset += 4;


  //! Keyboard output report
  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_HID_REPORT_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + hdl_offset,
      RSI_BLE_ATT_PROP_RD | RSI_BLE_ATT_PROP_WR | RSI_BLE_ATT_PROP_WR_NO_RESP,
      new_serv_resp.start_handle + hdl_offset + 1,
      new_uuid);

  //! adding characteristic value attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_HID_REPORT_UUID;
  memset(data, 0, HID_KDB_IN_RPT_DATA_LEN);
  len = HID_KDB_IN_RPT_DATA_LEN;
  rsi_ble_add_char_val_att (p_hid_info,
      new_serv_resp.serv_handler,
      new_serv_resp.start_handle + hdl_offset + 1,
      new_uuid,
      RSI_BLE_ATT_PROP_RD | RSI_BLE_ATT_PROP_WR | RSI_BLE_ATT_PROP_WR_NO_RESP,
      (uint8_t *)data,
      len, 0);

  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_REPORT_REFERENCE_UUID;
  data[0] = 1; data[1] = 2;
  len = 2;
  rsi_ble_add_char_val_att (p_hid_info,
      new_serv_resp.serv_handler,
      new_serv_resp.start_handle + hdl_offset + 3,
      new_uuid,
      RSI_BLE_ATT_PROP_RD,
      (uint8_t *)data,
      len, 0);
  hdl_offset += 4;


  //! Report map
  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_HID_REPORT_MAP_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + hdl_offset,
      RSI_BLE_ATT_PROP_RD,
      new_serv_resp.start_handle + hdl_offset + 1,
      new_uuid);

  //! adding characteristic value attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_HID_REPORT_MAP_UUID;
  len = sizeof(hid_report_map);
  rsi_ble_add_char_val_att (p_hid_info,
      new_serv_resp.serv_handler,
      new_serv_resp.start_handle + hdl_offset + 1,
      new_uuid,
      RSI_BLE_ATT_PROP_RD,
      (uint8_t *)hid_report_map,
      len, 1);
  hdl_offset += 2;


  //! HID info
  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_HID_INFO_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + hdl_offset,
      RSI_BLE_ATT_PROP_RD,
      new_serv_resp.start_handle + hdl_offset + 1,
      new_uuid);

  //! adding characteristic value attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_HID_INFO_UUID;
  data[0] = 1; data[1] = 1; data[2] = 0; data[3] = 3;
  len = 4;
  rsi_ble_add_char_val_att (p_hid_info,
      new_serv_resp.serv_handler,
      new_serv_resp.start_handle + hdl_offset + 1,
      new_uuid,
      RSI_BLE_ATT_PROP_RD,
      (uint8_t *)data,
      len, 0);
  hdl_offset += 2;


  //! control point
  //! adding characteristic service attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_HID_CONTROL_POINT_UUID;
  rsi_ble_add_char_serv_att (new_serv_resp.serv_handler,
      new_serv_resp.start_handle + hdl_offset,
      RSI_BLE_ATT_PROP_WR_NO_RESP,
      new_serv_resp.start_handle + hdl_offset + 1,
      new_uuid);

  //! adding characteristic value attribute to the service
  new_uuid.size = 2;
  new_uuid.val.val16 = RSI_BLE_HID_CONTROL_POINT_UUID;
  data[0] = 0; len = 1;
  rsi_ble_add_char_val_att (p_hid_info,
      new_serv_resp.serv_handler,
      new_serv_resp.start_handle + hdl_offset + 1,
      new_uuid,
      RSI_BLE_ATT_PROP_WR_NO_RESP,
      (uint8_t *)data,
      len, 0);
  hdl_offset += 2;

  return RSI_SUCCESS;
}

/*=============================================*/
/**
 * @fn         rsi_ble_hid_srv_gatt_wr_cb 
 * @brief      handles server side write events.
 * @param[in]  none
 * @return     none
 * @section description
 * handles server side write events and set respective flags.
 */
static void rsi_ble_hid_srv_gatt_wr_cb(void)
{
  LOG_PRINT("wr handle\n");
  if ((rsi_ble_hid_in_report_val_hndl + 1) == *((uint16_t *)app_ble_write_event.handle))
  {
    //! 0x01 for notification
    if(app_ble_write_event.att_value[0] == 0x01)
    {
      //! set a bit
      app_state |= BIT(REPORT_IN_NOTIFY_ENABLE);
    }
    else if(app_ble_write_event.att_value[0] == 0)
    {
      //! clear a bit
      app_state &= ~BIT(REPORT_IN_NOTIFY_ENABLE);
    }
    LOG_PRINT ("Input report notify val: %d\n", app_state & BIT(REPORT_IN_NOTIFY_ENABLE));
  }
}
void rsi_ble_on_sc_passkey (rsi_bt_event_sc_passkey_t *sc_passkey)
{
  rsi_6byte_dev_address_to_ascii (remote_dev_addr, sc_passkey->dev_addr);
	LOG_PRINT ("remote addr: %s, sc passkey: %06d \r\n", 
      remote_dev_addr, sc_passkey->passkey );
    numeric_value = sc_passkey->passkey;
    rsi_ble_app_set_event (RSI_BLE_SC_PASSKEY_EVENT);
}

int8_t hid_kbd_in_rpt_send(uint8_t len, uint8_t *p_data)
{
  uint8_t i = 0;
 
  rsi_ble_set_local_att_value (rsi_ble_hid_in_report_val_hndl, len, p_data);
  
  return 0;
}

/*==============================================*/
/**
 * @fn         rsi_ble_hid_gatt_application
 * @brief      this is the application of ble hid application.
 * @param[in]  event_id, it indicates write/notification event id.
 * @param[in]  rsi_ble_write, write event parameters.
 * @return     none.
 * @section description
 * This function is used to test the ble hid application.
 */
int32_t rsi_ble_hids_gatt_application (rsi_ble_hid_info_t *p_hid_info)
{
  int32_t status = 0;
  int32_t event_id,i;
  int8_t local_dev_addr[BD_ADDR_ARRAY_LEN] = {0};
  uint8_t hid_data[HID_KDB_IN_RPT_DATA_LEN] = {0};
  uint8_t rsi_app_resp_get_dev_addr[RSI_DEV_ADDR_LEN] = {0};
#if (GATT_ROLE == SERVER)
  uint8_t adv_data_len = 0;
  uint8_t adv_data[32] = {0x02, 0x01, 0x06,
                          0x03, 0x19, 0xC1, 0x03, /* Appearance */
                          0x03, 0x03, 0x12, 0x18 /*0x0F, 0x18, 0x0A, 0x18,  Service UUIDs */  
  };
	/* Name */ 
	adv_data[11] = strlen (RSI_BLE_APP_HIDS) + 1;
	adv_data[12] = 0x09;
  strcpy (&adv_data[13], RSI_BLE_APP_HIDS);
	
  adv_data_len = strlen (RSI_BLE_APP_HIDS) + 13;
#elif (GATT_ROLE == CLIENT)
  static uint8_t char_srv_index = 0;
  uuid_t service_uuid;
  profile_descriptors_t  ble_servs = {0};
  rsi_ble_resp_char_services_t char_servs = {0};
  rsi_ble_resp_att_descs_t  att_desc = {0};
#endif

#ifndef RSI_WITH_OS
  //! Driver initialization
  status = rsi_driver_init(bt_global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }

  //! RS9113 intialisation 
  status = rsi_device_init(RSI_LOAD_IMAGE_I_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif

  //! WC initialization
  status = rsi_wireless_init(0, RSI_OPERMODE_WLAN_BLE);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

#if (GATT_ROLE == SERVER)
  //! adding ble hid service
  rsi_ble_add_hid_serv (p_hid_info);
#endif

  //! registering the GAP callback functions
  rsi_ble_gap_register_callbacks(
      rsi_ble_on_adv_report_event,
      rsi_ble_on_connect_event,
      rsi_ble_on_disconnect_event,
      NULL,
      NULL,
      NULL,
      rsi_ble_on_enhance_conn_status_event,
      NULL,
      NULL);

  //! registering the GATT callback functions
  rsi_ble_gatt_register_callbacks (
      NULL,
      rsi_ble_on_profiles_event,
      rsi_ble_on_char_services_event,
      NULL,
      ble_on_att_desc_event,
      NULL,
      NULL,
      rsi_ble_on_gatt_write_event,
      NULL,
      NULL,
      rsi_ble_on_read_req_event,
      rsi_ble_on_mtu_event);

  //! registering the SMP callback functions
  rsi_ble_smp_register_callbacks(
      rsi_ble_on_smp_request,
      rsi_ble_on_smp_response,
      rsi_ble_on_smp_passkey,
      rsi_ble_on_smp_failed,
      rsi_ble_on_encrypt_started,
      rsi_ble_on_smp_passkey_display,
      rsi_ble_on_sc_passkey,
      rsi_ble_on_le_ltk_req_event,
      NULL);

  //!  initializing the application events map
  rsi_ble_app_init_events ();

  //! Set local name
  rsi_bt_set_local_name(RSI_BLE_APP_HIDS);
  
  //! get the local device address(MAC address).
  status = rsi_bt_get_local_device_address(rsi_app_resp_get_dev_addr);
  if(status != RSI_SUCCESS)
  {
    return status;
  }
  rsi_6byte_dev_address_to_ascii (local_dev_addr, rsi_app_resp_get_dev_addr);
  LOG_PRINT("local device addr: %s \n",local_dev_addr);

#if(GATT_ROLE == SERVER)
  //! set advertise data
  rsi_ble_set_advertise_data (adv_data, adv_data_len);

  LOG_PRINT ("adv start\n");
  //! set device in advertising mode.
  rsi_ble_start_advertising ();
  app_state |= BIT(ADVERTISE);
#endif

#if(GATT_ROLE == CLIENT)
  //! start scanning
  status = rsi_ble_start_scanning();
  if(status != RSI_SUCCESS)
  {
    return status;
  }
#endif

  //! waiting for events from controller.
  while (1) 
  {
    //! Application main loop
#ifndef RSI_WITH_OS
    rsi_wireless_driver_task();
#endif
    //! checking for events list
    event_id = rsi_ble_app_get_event ();

    if (event_id == -1)
    {
      if(app_state & BIT(CONNECTED))
      {
        if(app_state & BIT(REPORT_IN_NOTIFY_ENABLE))
        {
#ifdef __linux__
          usleep(3000000);
#endif
#if RSI_M4_ENABLE
          rsi_delay_ms (3000);
#endif
          hid_data[2] = 0xb;// key 'h' pressed
          rsi_ble_set_local_att_value (rsi_ble_hid_in_report_val_hndl, HID_KDB_IN_RPT_DATA_LEN, hid_data);
          hid_data[2] = 0x0;// key released
          rsi_ble_set_local_att_value (rsi_ble_hid_in_report_val_hndl, HID_KDB_IN_RPT_DATA_LEN, hid_data);

          hid_data[2] = 0x12;// key 'o' pressed
          rsi_ble_set_local_att_value (rsi_ble_hid_in_report_val_hndl, HID_KDB_IN_RPT_DATA_LEN, hid_data);
          hid_data[2] = 0x0;// key released
          rsi_ble_set_local_att_value (rsi_ble_hid_in_report_val_hndl, HID_KDB_IN_RPT_DATA_LEN, hid_data);

          hid_data[2] = 0xa;// key 'g' pressed
          rsi_ble_set_local_att_value (rsi_ble_hid_in_report_val_hndl, HID_KDB_IN_RPT_DATA_LEN, hid_data);
          hid_data[2] = 0x0;// key released
          rsi_ble_set_local_att_value (rsi_ble_hid_in_report_val_hndl, HID_KDB_IN_RPT_DATA_LEN, hid_data);

          hid_data[2] = 0x2c;// key 'space' pressed
          rsi_ble_set_local_att_value (rsi_ble_hid_in_report_val_hndl, HID_KDB_IN_RPT_DATA_LEN, hid_data);
          hid_data[2] = 0x0;// key released
          rsi_ble_set_local_att_value (rsi_ble_hid_in_report_val_hndl, HID_KDB_IN_RPT_DATA_LEN, hid_data);
        }
      }
      continue;
    }

    switch (event_id) 
    {
#if(GATT_ROLE == CLIENT)
      case RSI_APP_EVENT_ADV_REPORT:
        {
          //! advertise report event.
          
          //! clear the advertise report event.
          rsi_ble_app_clear_event (RSI_APP_EVENT_ADV_REPORT);
          status = rsi_ble_connect ((int8_t)RSI_BLE_REMOTE_BD_ADDRESS_TYPE, (int8_t *)RSI_BLE_REMOTE_BD_ADDRESS);
          if(status != RSI_SUCCESS)
          {
            LOG_PRINT ("connect status: 0x%X\r\n", status);
          }

        }
        break;
#endif
      case RSI_BLE_EVENT_CONN:
        {
          //! event invokes when connection was completed

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_EVENT_CONN);

          app_state |= BIT(CONNECTED);
          rsi_6byte_dev_address_to_ascii ((int8_t *)remote_dev_addr, (uint8_t *)conn_event_to_app.dev_addr);
#if(GATT_ROLE == CLIENT)
          rsi_ble_smp_pair_request (remote_dev_addr, RSI_BLE_SMP_IO_CAPABILITY);
#endif
        }
        break;

      case RSI_BLE_EVENT_DISCONN:
        {
          //! event invokes when disconnection was completed

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_EVENT_DISCONN);

          app_state = 0;
          app_state |= BIT(ADVERTISE);
          //! set device in advertising mode.
#if(GATT_ROLE == SERVER)
          LOG_PRINT ("adv start\n");
adv:
          status = rsi_ble_start_advertising ();
          if (status != RSI_SUCCESS) {
            goto adv;
          }
#elif(GATT_ROLE == CLIENT)
scan:
          //! start scanning
          device_found = 0;
          status = rsi_ble_start_scanning();
          if (status != RSI_SUCCESS)
          {
            goto scan;
          }
#endif
        }
        break;

      case RSI_BLE_EVENT_GATT_WR:
        {
          //! event invokes when write/notification events received

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_EVENT_GATT_WR);
        
#if(GATT_ROLE == SERVER)
          rsi_ble_hid_srv_gatt_wr_cb();
#endif
        }
        break;
      
      case RSI_BLE_EVENT_GATT_RD:
        {
          //! event invokes when read events received

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_EVENT_GATT_RD);
          for(i = 0; i < p_hid_info->att_rec_list_count; i++)
          {
            if(p_hid_info->att_rec_list[i].handle == app_ble_read_event.handle)
            {
              if(app_ble_read_event.type==0)
              {
                LOG_PRINT("read resp \n");
                rsi_ble_gatt_read_response (remote_dev_addr,
                    GATT_READ_RESP,
                    app_ble_read_event.handle,
                    GATT_READ_ZERO_OFFSET,
                    p_hid_info->att_rec_list[i].len,
                    p_hid_info->att_rec_list[i].value);
              }
              else
              {
                LOG_PRINT("read blob resp\n");
                rsi_ble_gatt_read_response (remote_dev_addr,
                    GATT_READ_BLOB_RESP,
                    app_ble_read_event.handle,
                    app_ble_read_event.offset,
                    p_hid_info->att_rec_list[i].len - app_ble_read_event.offset,
                    ((uint8_t *)p_hid_info->att_rec_list[i].value) + app_ble_read_event.offset);
              }
            }
          }
        }
        break;

      case RSI_BLE_EVENT_MTU:
        {
          //! event invokes when write/notification events received

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_EVENT_MTU);
        }
        break;

      case RSI_BLE_EVENT_SMP_REQ:
        {
          //! initiate SMP protocol as a Master

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_EVENT_SMP_REQ);
          
          //! initiating the SMP pairing process
          status = rsi_ble_smp_pair_request (remote_dev_addr, RSI_BLE_SMP_IO_CAPABILITY);
        }
        break;
      
      case RSI_BLE_EVENT_SMP_RESP:
        {
          //! initiate SMP protocol as a Master

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_EVENT_SMP_RESP);

          //! initiating the SMP pairing process
          status = rsi_ble_smp_pair_response (remote_dev_addr, RSI_BLE_SMP_IO_CAPABILITY);
        }
        break;

      case RSI_BLE_EVENT_SMP_PASSKEY:
        {
          uint32_t passkey = RSI_BLE_SMP_PASSKEY;
          //! initiate SMP protocol as a Master

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_EVENT_SMP_PASSKEY);
       
          //! initiating the SMP pairing process
          rsi_ble_smp_passkey (remote_dev_addr, passkey);
        }
        break;

      case RSI_BLE_EVENT_SMP_PASSKEY_DISPLAY:
        {
          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_EVENT_SMP_PASSKEY_DISPLAY);
        }
        break;

      case RSI_BLE_EVENT_SMP_FAILED:
        {
          //! initiate SMP protocol as a Master

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_EVENT_SMP_FAILED);
        }
        break;

      case RSI_BLE_EVENT_LTK_REQ:
        {
          //! event invokes when disconnection was completed

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_EVENT_LTK_REQ);
          rsi_6byte_dev_address_to_ascii (remote_dev_addr, temp_le_ltk_req.dev_addr);

#if (ROLE == SLAVE)
          if((temp_le_ltk_req.localediv == glbl_enc_enabled.localediv) && 
              !((memcmp(temp_le_ltk_req.localrand, glbl_enc_enabled.localrand,8)))
            )
          {
            LOG_PRINT("Positive reply\n");
            //! give le ltk req reply cmd with positive reply
            status = rsi_ble_ltk_req_reply(remote_dev_addr, 1, glbl_enc_enabled.localltk);
          }
          else
          {
            LOG_PRINT("Negative reply\n");
            //! give le ltk req reply cmd with negative reply
            status = rsi_ble_ltk_req_reply(remote_dev_addr, 0, NULL);
          }
#endif
        }
        break;

      case RSI_BLE_EVENT_SMP_ENCRYPT_STARTED:
        {
          //! start the encrypt event

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_EVENT_SMP_ENCRYPT_STARTED);
          
          app_state |= BIT(ENCRYPT_EN);
#if (GATT_ROLE == CLIENT)
          service_uuid.size = 2;
          service_uuid.val.val16 = RSI_BLE_HID_SERVICE_UUID;
          rsi_ble_get_profile_async (remote_dev_addr, service_uuid, &ble_servs);
#endif
        }
        break;

#if(GATT_ROLE == CLIENT)
      case RSI_BLE_EVENT_GATT_PROFILE_RESP:
        {
          //! event invokes when get profile response received

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_EVENT_GATT_PROFILE_RESP);

          if(att_resp_status == 0)
          {
            LOG_PRINT ("Service UUID : 0x%04x (Handle range 0x%04x - 0x%04x)",
                ble_servs.profile_uuid.val.val16,
                *(uint16_t *)ble_servs.start_handle,
                *(uint16_t *)ble_servs.end_handle);

            //! query characteristic services, with in the particular range, from the connected remote device.
            rsi_ble_get_char_services_async (remote_dev_addr,
                *(uint16_t *)ble_servs.start_handle,
                *(uint16_t *)ble_servs.end_handle,
                &char_servs);
          }
        }
        break;

      case RSI_BLE_EVENT_GATT_CHAR_SERVICES_RESP:
        {
          //! event invokes when get characteristics of the service response received

          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_EVENT_GATT_CHAR_SERVICES_RESP);

          if(att_resp_status == 0)
          {
            if(char_srv_index < char_servs.num_of_services)
            {
              LOG_PRINT ("    Charactristic UUID: 0x%04x (Handle: 0x%04x, Property: 0x%02x)",
                  char_servs.char_services[char_srv_index].char_data.char_uuid.val.val16,
                  char_servs.char_services[char_srv_index].handle,
                  char_servs.char_services[char_srv_index].char_data.char_property);

              //! query attribute descriptor, with in the particular range, from the connected remote device.
              if(char_srv_index < (char_servs.num_of_services - 1))
              {
                rsi_ble_get_att_descriptors_async (remote_dev_addr,
                    char_servs.char_services[char_srv_index].handle,
                    char_servs.char_services[char_srv_index + 1].handle - 1,
                    &att_desc);
              }
              else
              {
                rsi_ble_get_att_descriptors_async (remote_dev_addr,
                    char_servs.char_services[char_srv_index].handle,
                    *(uint16_t *)ble_servs.end_handle,
                    &att_desc);
              }
            }
            else
            {
              uint8_t temp_val = char_servs.char_services[char_srv_index - 1].handle + 1;
              ble_servs.start_handle[0] = temp_val & 0xFF;
              ble_servs.start_handle[1] = (temp_val >> 8) & 0xFF;
              char_srv_index = 0;
              //! set event for next charteristics
              rsi_ble_app_set_event (RSI_BLE_EVENT_GATT_PROFILE_RESP);
            }
          }
        }
        break;

      case RSI_BLE_EVENT_GATT_CHAR_DESC_RESP:
        {
          uint8_t temp_prop = 0;
          uint8_t char_desc_index = 0;
          
          //! clear the served event
          rsi_ble_app_clear_event (RSI_BLE_EVENT_GATT_CHAR_DESC_RESP);
        
          if(att_resp_status == 0)
          {
            for (char_desc_index = 0; char_desc_index < att_desc.num_of_att; char_desc_index++)
            {
              if(RSI_BLE_CHAR_SERV_UUID == att_desc.att_desc[char_desc_index].att_type_uuid.val.val16)
              {
                temp_prop = char_servs.char_services[char_srv_index].char_data.char_property;
              }
              else
              {
                LOG_PRINT("        Att Type UUID: 0x%04x (Handle 0x%04x)",
                    att_desc.att_desc[char_desc_index].att_type_uuid.val.val16,
                    (att_desc.att_desc[char_desc_index].handle[0] | (att_desc.att_desc[char_desc_index].handle[1] << 8))
                    );
              }

              if(RSI_BLE_CLIENT_CHAR_UUID == att_desc.att_desc[char_desc_index].att_type_uuid.val.val16)
              {
                //! check the attribute property with notification
                if (temp_prop & RSI_BLE_ATT_PROP_NOTIFY)
                {
                  uint8_t data[] = {1,0};
                  LOG_PRINT ("        CCCD, notification enable");
                  //! 0x0001 to enable notification
                  //! Set attribute value of the connected remote device.Ack will not be received from the remote device.
                  rsi_ble_set_att_value_async (remote_dev_addr, 
                      *((uint16_t *)att_desc.att_desc[char_desc_index].handle),
                      2,
                      data);
                }

                //! check the attribute property with indication
                if (temp_prop & RSI_BLE_ATT_PROP_INDICATE)
                {
                  uint8_t data[] = {2,0};
                  LOG_PRINT ("        CCCD, indication enable");
                  //! 0x0002 to enable indication
                  //! Set attribute value of the connected remote device.Ack will not be received from the remote device.
                  rsi_ble_set_att_value_async (remote_dev_addr, 
                      *((uint16_t *)att_desc.att_desc[char_desc_index].handle),
                      2,
                      data);
                }
              }
            }
          }

          char_srv_index++;
          //! set event for next charteristics
          rsi_ble_app_set_event (RSI_BLE_EVENT_GATT_CHAR_SERVICES_RESP);
          memset (&att_desc, 0, sizeof(rsi_ble_resp_att_descs_t));
        }
        break;
#endif

      case RSI_BLE_SC_PASSKEY_EVENT:
      {
        rsi_ble_app_clear_event (RSI_BLE_SC_PASSKEY_EVENT);
        rsi_ble_smp_passkey (remote_dev_addr, numeric_value);
        LOG_PRINT("\nnumeric value %d\n",numeric_value);
      }
      break;


    }
  }

  return RSI_SUCCESS;
}

/*==============================================*/
/**
 * @fn         main_loop
 * @brief      Schedules the Driver task.
 * @param[in]  none.
 * @return     none.
 * @section description
 * This function schedules the Driver task.
 */
void main_loop (void)
{
  while (1) {
    rsi_wireless_driver_task ();
  }
}

/*==============================================*/
/**
 * @fn         main
 * @brief      main function
 * @param[in]  none.
 * @return     none.
 * @section description
 * This is the main function to call Application
 */
int main (void)
{
#ifdef RSI_WITH_OS
  int32_t status;
  rsi_task_handle_t bt_task_handle = NULL;
  rsi_task_handle_t driver_task_handle = NULL;
#endif

#ifndef RSI_SAMPLE_HAL
  //! Board Initialization
  Board_init();
#endif

#ifndef RSI_WITH_OS

  //Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);
  
  //! Call hid GATT Server Application
  rsi_ble_hids_gatt_application(&hid_info_g);

  //! Application main loop
  main_loop();

  return 0;
#endif
#ifdef RSI_WITH_OS
 //! Driver initialization
  status = rsi_driver_init(bt_global_buf, BT_GLOBAL_BUFF_LEN);
  if((status < 0) || (status > BT_GLOBAL_BUFF_LEN))
  {
    return status;
  }
  //! RS9113 initialization
  status = rsi_device_init(RSI_LOAD_IMAGE_I_FW);
  if(status != RSI_SUCCESS)
  {
    return status;
  }

  //Start BT Stack
  intialize_bt_stack(STACK_BTLE_MODE);

  //! OS case
  //! Task created for BLE task
  rsi_task_create(rsi_ble_hid_gatt_application, "ble_task", RSI_BT_TASK_STACK_SIZE, NULL, RSI_BT_TASK_PRIORITY, &bt_task_handle);

  //! Task created for Driver task
  rsi_task_create(rsi_wireless_driver_task, "driver_task",RSI_DRIVER_TASK_STACK_SIZE, NULL, RSI_DRIVER_TASK_PRIORITY, &driver_task_handle);

  //! OS TAsk Start the scheduler
  rsi_start_os_scheduler();

  return status;
#endif
}
