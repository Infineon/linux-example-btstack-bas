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
* File Name: bas_server_gatt_db.c
*
* Description:
* BLE device's GATT database and device configuration header file.
*
******************************************************************************/

#if !defined(bas_server_GATT_DB_H)
#define bas_server_GATT_DB_H

#include "stdint.h"

#define __UUID_SERVICE_GENERIC_ACCESS                      0x1800
#define __UUID_CHARACTERISTIC_DEVICE_NAME                  0x2A00
#define __UUID_CHARACTERISTIC_APPEARANCE                   0x2A01
#define __UUID_SERVICE_GENERIC_ATTRIBUTE                   0x1801
#define __UUID_SERVICE_BATTERY                             0x180F
#define __UUID_CHARACTERISTIC_BATTERY_LEVEL                0x2A19
#define __UUID_DESCRIPTOR_CHARACTERISTIC_PRESENTATION_FORMAT    0x2904
#define __UUID_DESCRIPTOR_CLIENT_CHARACTERISTIC_CONFIGURATION    0x2902
#define __UUID_CHARACTERISTIC_SERVICE_CHANGED              0x2A05

/* Service Generic Access */
#define HDLS_GAP                                           0x0001
/* Characteristic Device Name */
#define HDLC_GAP_DEVICE_NAME                               0x0002
#define HDLC_GAP_DEVICE_NAME_VALUE                         0x0003
/* Characteristic Appearance */
#define HDLC_GAP_APPEARANCE                                0x0004
#define HDLC_GAP_APPEARANCE_VALUE                          0x0005

/* Service Generic Attribute */
#define HDLS_GATT                                          0x0006

/* Service Battery */
#define HDLS_BAS                                           0x0007
/* Characteristic Battery Level */
#define HDLC_BAS_BATTERY_LEVEL                             0x0008
#define HDLC_BAS_BATTERY_LEVEL_VALUE                       0x0009
/* Descriptor Characteristic Presentation Format */
#define HDLD_BAS_BATTERY_LEVEL_CHAR_PRESENTATION_FORMAT    0x000A
/* Descriptor Client Characteristic Configuration */
#define HDLD_BAS_BATTERY_LEVEL_CLIENT_CHAR_CONFIG          0x000B


/* External Lookup Table Entry */
typedef struct
{
    uint16_t handle;
    uint16_t max_len;
    uint16_t cur_len;
    uint8_t  *p_data;
} gatt_db_lookup_table_t;

/* External definitions */
extern const uint8_t  gatt_database[];
extern const uint16_t gatt_database_len;
extern gatt_db_lookup_table_t app_gatt_db_ext_attr_tbl[];
extern const uint16_t app_gatt_db_ext_attr_tbl_size;
extern uint8_t app_gap_device_name[];
extern const uint16_t app_gap_device_name_len;
extern uint8_t app_gap_appearance[];
extern const uint16_t app_gap_appearance_len;
extern uint8_t app_bas_battery_level[];
extern const uint16_t app_bas_battery_level_len;
extern uint8_t app_bas_battery_level_char_presentation_format[];
extern const uint16_t app_bas_battery_level_char_presentation_format_len;
extern uint8_t app_bas_battery_level_client_char_config[];
extern const uint16_t app_bas_battery_level_client_char_config_len;

#endif /* bas_server_GATT_DB_H */
