/******************************************************************************
 * (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 ******************************************************************************
 * This software, including source code, documentation and related materials
 * ("Software"), is owned by Cypress Semiconductor Corporation or one of its
 * subsidiaries ("Cypress") and is protected by and subject to worldwide patent
 * protection (United States and foreign), United States copyright laws and
 * international treaty provisions. Therefore, you may use this Software only
 * as provided in the license agreement accompanying the software package from
 * which you obtained this Software ("EULA").
 *
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software source
 * code solely for use in connection with Cypress's integrated circuit products.
 * Any reproduction, modification, translation, compilation, or representation
 * of this Software except as specified above is prohibited without the express
 * written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer of such
 * system or application assumes all risk of such use and in doing so agrees to
 * indemnify Cypress against all liability.
 *****************************************************************************/
/******************************************************************************
* File Name: bas_server_gap.h
*
* Description:
* BLE device's GAP configuration header file.
*
******************************************************************************/

#if !defined(bas_server_GAP_H)
#define bas_server_GAP_H

#include "stdint.h"
#include "wiced_bt_ble.h"
#include "bas_server_gatt_db.h"

/* Silicon generated 'Company assigned' part of device address */
#define CY_BT_SILICON_DEVICE_ADDRESS_EN                       0

/* Appearance */
#define CY_BT_APPEARANCE                                      0

/* TX Power Level */
#define CY_BT_TX_POWER                                        0

/* Interval of random address refreshing */
#define CY_BT_RPA_TIMEOUT                                     900

/* Maximum attribute length */
#define CY_BT_MAX_ATTR_LEN                                    512
/* Maximum attribute MTU size */
#define CY_BT_MTU_SIZE                                        517

/* RX PDU size */
#define CY_BT_RX_PDU_SIZE                                     517

/* Maximum connections */
#define CY_BT_SERVER_MAX_LINKS                                1
#define CY_BT_CLIENT_MAX_LINKS                                0

/* LE Filter accept list size */
#define CY_BT_FILTER_ACCEPT_LIST_SIZE                         0
/* This define is obsolete and kept for backward compatibility only. */
#define CY_BT_WHITE_LIST_SIZE                                 CY_BT_FILTER_ACCEPT_LIST_SIZE

/* L2CAP configuration */
#define CY_BT_L2CAP_MAX_LE_PSM                                1
#define CY_BT_L2CAP_MAX_LE_CHANNELS                           1
#define CY_BT_L2CAP_MAX_LE_FIXED_CHANNELS                     0
#define CY_BT_L2CAP_MAX_BR_ERTM_CHANNELS                      0
#define CY_BT_L2CAP_MAX_BR_ERTM_TX_WIN                        0
#define CY_BT_L2CAP_MTU_SIZE                                  517

/* Security level */
#define CY_BT_SECURITY_LEVEL                                  BTM_SEC_BEST_EFFORT

/* Scan configuration */
#define CY_BT_SCAN_MODE                                       BTM_BLE_SCAN_MODE_PASSIVE

#define CY_BT_HIGH_DUTY_SCAN_INTERVAL                         WICED_BT_CFG_DEFAULT_HIGH_DUTY_SCAN_INTERVAL
#define CY_BT_HIGH_DUTY_SCAN_WINDOW                           WICED_BT_CFG_DEFAULT_HIGH_DUTY_SCAN_WINDOW
#define CY_BT_HIGH_DUTY_SCAN_DURATION                         5

#define CY_BT_LOW_DUTY_SCAN_INTERVAL                          WICED_BT_CFG_DEFAULT_LOW_DUTY_SCAN_INTERVAL
#define CY_BT_LOW_DUTY_SCAN_WINDOW                            WICED_BT_CFG_DEFAULT_LOW_DUTY_SCAN_WINDOW
#define CY_BT_LOW_DUTY_SCAN_DURATION                          60

#define CY_BT_HIGH_DUTY_CONN_SCAN_INTERVAL                    WICED_BT_CFG_DEFAULT_HIGH_DUTY_CONN_SCAN_INTERVAL
#define CY_BT_HIGH_DUTY_CONN_SCAN_WINDOW                      WICED_BT_CFG_DEFAULT_HIGH_DUTY_CONN_SCAN_WINDOW
#define CY_BT_HIGH_DUTY_CONN_SCAN_DURATION                    30

#define CY_BT_LOW_DUTY_CONN_SCAN_INTERVAL                     WICED_BT_CFG_DEFAULT_LOW_DUTY_CONN_SCAN_INTERVAL
#define CY_BT_LOW_DUTY_CONN_SCAN_WINDOW                       WICED_BT_CFG_DEFAULT_LOW_DUTY_CONN_SCAN_WINDOW
#define CY_BT_LOW_DUTY_CONN_SCAN_DURATION                     30

/* Connection configuration */
#define CY_BT_CONN_MIN_INTERVAL                               WICED_BT_CFG_DEFAULT_CONN_MIN_INTERVAL
#define CY_BT_CONN_MAX_INTERVAL                               WICED_BT_CFG_DEFAULT_CONN_MAX_INTERVAL
#define CY_BT_CONN_LATENCY                                    WICED_BT_CFG_DEFAULT_CONN_LATENCY
#define CY_BT_CONN_SUPERVISION_TIMEOUT                        WICED_BT_CFG_DEFAULT_CONN_SUPERVISION_TIMEOUT

/* Advertisement settings */
#define CY_BT_CHANNEL_MAP                                     (BTM_BLE_ADVERT_CHNL_37 | BTM_BLE_ADVERT_CHNL_38 | BTM_BLE_ADVERT_CHNL_39)

#define CY_BT_HIGH_DUTY_ADV_MIN_INTERVAL                      48
#define CY_BT_HIGH_DUTY_ADV_MAX_INTERVAL                      48
#define CY_BT_HIGH_DUTY_ADV_DURATION                          60

#define CY_BT_LOW_DUTY_ADV_MIN_INTERVAL                       2048
#define CY_BT_LOW_DUTY_ADV_MAX_INTERVAL                       2048
#define CY_BT_LOW_DUTY_ADV_DURATION                           60

#define CY_BT_HIGH_DUTY_DIRECTED_ADV_MIN_INTERVAL             400
#define CY_BT_HIGH_DUTY_DIRECTED_ADV_MAX_INTERVAL             800

#define CY_BT_LOW_DUTY_DIRECTED_ADV_MIN_INTERVAL              48
#define CY_BT_LOW_DUTY_DIRECTED_ADV_MAX_INTERVAL              48
#define CY_BT_LOW_DUTY_DIRECTED_ADV_DURATION                  30

#define CY_BT_HIGH_DUTY_NONCONN_ADV_MIN_INTERVAL              160
#define CY_BT_HIGH_DUTY_NONCONN_ADV_MAX_INTERVAL              160
#define CY_BT_HIGH_DUTY_NONCONN_ADV_DURATION                  30

#define CY_BT_LOW_DUTY_NONCONN_ADV_MIN_INTERVAL               2048
#define CY_BT_LOW_DUTY_NONCONN_ADV_MAX_INTERVAL               2048
#define CY_BT_LOW_DUTY_NONCONN_ADV_DURATION                   30


/* Advertisement and scan response packets defines */
#define CY_BT_ADV_PACKET_ELEM_COUNT                           3
/* CY_BT_ADV_PACKET_DATA_SIZE is obsolete. Use CY_BT_ADV_PACKET_ELEM_COUNT instead */
#define CY_BT_ADV_PACKET_DATA_SIZE                            CY_BT_ADV_PACKET_ELEM_COUNT

/* cy_bt_device_name is obsolete. Use app_gap_device_name instead. */
#define cy_bt_device_name                                     app_gap_device_name

/* External definitions */
extern wiced_bt_device_address_t cy_bt_device_address;
extern wiced_bt_ble_advert_elem_t cy_bt_adv_packet_data[];

#endif /* bas_server_GAP_H */
