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
 * File Name: bas.h
 *
 * Description: Battery server application and function prototypes.
 *
 * Related Document: See README.md
 *
 *****************************************************************************/
/******************************************************************************
 *                                INCLUDES
 *****************************************************************************/
#include "stdlib.h"
#include "app_bt_utils.h"
#include "wiced_bt_trace.h"
#include "wiced_timer.h"
#include "wiced_bt_trace.h"
#include "wiced_memory.h"
#include "wiced_bt_stack.h"
#include "app_bt_config/bas_server_gatt_db.h"
#include "app_bt_config/bas_server_cfg.h"
#include "app_bt_config/bas_server_gap.h"

/******************************************************************************
 *                                MACROS
 *****************************************************************************/
 #define BATTERY_LEVEL_CHANGE ( 2 )
 #define LOCAL_BDA_LEN ( 6 )

/******************************************************************************
 *                                EXTERNS
*****************************************************************************/
extern uint8_t bas_bd_address[];

/******************************************************************************
 *                                ENUM
*****************************************************************************/
typedef enum
{
    APP_BT_ADV_OFF_CONN_OFF,
    APP_BT_ADV_ON_CONN_OFF,
    APP_BT_ADV_OFF_CONN_ON
} app_bt_adv_conn_mode_t;
 
/*******************************************************************************
*                        FUNCTION DECLARATIONS
*******************************************************************************/
void send_notification_timer_cb( WICED_TIMER_PARAM_TYPE signo );
wiced_bt_gatt_status_t app_bt_set_value(uint16_t attr_handle, uint8_t *p_val, \
    uint16_t len);
wiced_bt_gatt_status_t app_bt_gatt_req_read_handler(uint16_t conn_id, \
    wiced_bt_gatt_opcode_t opcode, wiced_bt_gatt_read_t *p_read_req, \
    uint16_t len_requested);
wiced_bt_gatt_status_t app_bt_gatt_req_read_multi_handler(uint16_t \
    conn_id,wiced_bt_gatt_opcode_t opcode, \
    wiced_bt_gatt_read_multiple_req_t *p_read_req, \
    uint16_t len_requested);
wiced_bt_gatt_status_t app_bt_gatt_req_read_by_type_handler(uint16_t \
    conn_id,wiced_bt_gatt_opcode_t opcode, \
    wiced_bt_gatt_read_by_type_t *p_read_req,uint16_t len_requested);
wiced_bt_gatt_status_t app_bt_connect_event_handler( \
    wiced_bt_gatt_connection_status_t *p_conn_status);
wiced_bt_gatt_status_t app_bt_server_event_handler( \
    wiced_bt_gatt_event_data_t *p_data);
wiced_bt_gatt_status_t app_bt_gatt_event_callback( \
    wiced_bt_gatt_evt_t event,wiced_bt_gatt_event_data_t *p_event_data);
/* Callback function for Bluetooth stack management type events */
wiced_bt_dev_status_t  app_bt_management_callback( \
    wiced_bt_management_evt_t event, \
    wiced_bt_management_evt_data_t *p_event_data); 
wiced_bt_gatt_status_t app_bt_write_handler( \
    wiced_bt_gatt_event_data_t *p_data);
void app_bt_adv_led_update(void);
void app_bt_init(void);
void app_bt_batt_level_init(void);
/* [] END OF FILE */

