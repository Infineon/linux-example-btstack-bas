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
* BLE device's GATT database and device configuration.
*
******************************************************************************/

#include "bas_server_gatt_db.h"
#include "wiced_bt_uuid.h"
#include "wiced_bt_gatt.h"

/******************************************************************************
* GATT server definitions
******************************************************************************/

const uint8_t gatt_database[] = 
{
    /* Primary Service: Generic Access */
    PRIMARY_SERVICE_UUID16 (
            HDLS_GAP,
            __UUID_SERVICE_GENERIC_ACCESS),
        /* Characteristic: Device Name */
        CHARACTERISTIC_UUID16 (
                HDLC_GAP_DEVICE_NAME,
                HDLC_GAP_DEVICE_NAME_VALUE,
                __UUID_CHARACTERISTIC_DEVICE_NAME,
                GATTDB_CHAR_PROP_READ,
                GATTDB_PERM_READABLE),
        /* Characteristic: Appearance */
        CHARACTERISTIC_UUID16 (
                HDLC_GAP_APPEARANCE,
                HDLC_GAP_APPEARANCE_VALUE,
                __UUID_CHARACTERISTIC_APPEARANCE,
                GATTDB_CHAR_PROP_READ,
                GATTDB_PERM_READABLE),

    /* Primary Service: Generic Attribute */
    PRIMARY_SERVICE_UUID16 (
            HDLS_GATT,
            __UUID_SERVICE_GENERIC_ATTRIBUTE),

    /* Primary Service: Battery */
    PRIMARY_SERVICE_UUID16 (
            HDLS_BAS,
            __UUID_SERVICE_BATTERY),
        /* Characteristic: Battery Level */
        CHARACTERISTIC_UUID16 (
                HDLC_BAS_BATTERY_LEVEL,
                HDLC_BAS_BATTERY_LEVEL_VALUE,
                __UUID_CHARACTERISTIC_BATTERY_LEVEL,
                GATTDB_CHAR_PROP_READ | GATTDB_CHAR_PROP_NOTIFY,
                GATTDB_PERM_READABLE),
            /* Descriptor: Characteristic Presentation Format */
            CHAR_DESCRIPTOR_UUID16 (
                    HDLD_BAS_BATTERY_LEVEL_CHAR_PRESENTATION_FORMAT,
                    __UUID_DESCRIPTOR_CHARACTERISTIC_PRESENTATION_FORMAT,
                    GATTDB_PERM_READABLE),
            /* Descriptor: Client Characteristic Configuration */
            CHAR_DESCRIPTOR_UUID16_WRITABLE (
                    HDLD_BAS_BATTERY_LEVEL_CLIENT_CHAR_CONFIG,
                    __UUID_DESCRIPTOR_CLIENT_CHARACTERISTIC_CONFIGURATION,
                    GATTDB_PERM_READABLE | GATTDB_PERM_WRITE_REQ),
};

/* Length of the GATT database */
const uint16_t gatt_database_len = sizeof(gatt_database);

/******************************************************************************
* GATT Initial Value Arrays
******************************************************************************/
 
uint8_t app_gap_device_name[] = {'B', 'a', 't', 't', 'e', 'r', 'y', ' ', 'S', 'e', 'r', 'v',
    'e', 'r', '\0',};
uint8_t app_gap_appearance[] = {0x00, 0x00,};
uint8_t app_bas_battery_level[] = {0x00,};
uint8_t app_bas_battery_level_char_presentation_format[] = {0x00, 0x00, 0x00, 0x00, 0x01, 
    0x00,0x00,};
uint8_t app_bas_battery_level_client_char_config[] = {0x00, 0x00,};
 
/******************************************************************************
* GATT Lookup Table
******************************************************************************/
 
gatt_db_lookup_table_t app_gatt_db_ext_attr_tbl[] =
{
    {
        HDLC_GAP_DEVICE_NAME_VALUE,                      /* attribute handle */
        14,                                              /* maxlen */
        14,                                              /* curlen */
        app_gap_device_name,                             /* attribute data */
    },
    {
        HDLC_GAP_APPEARANCE_VALUE,                       /* attribute handle */
        2,                                               /* maxlen */
        2,                                               /* curlen */
        app_gap_appearance,                              /* attribute data */
    },
    {
        HDLC_BAS_BATTERY_LEVEL_VALUE,                    /* attribute handle */
        1,                                               /* maxlen */
        1,                                               /* curlen */
        app_bas_battery_level,                           /* attribute data */
    },
    {
        HDLD_BAS_BATTERY_LEVEL_CHAR_PRESENTATION_FORMAT, /* attribute handle */
        7,                                               /* maxlen */
        7,                                               /* curlen */
        app_bas_battery_level_char_presentation_format,  /* attribute data */
    },
    {
        HDLD_BAS_BATTERY_LEVEL_CLIENT_CHAR_CONFIG,       /* attribute handle */
        2,                                               /* maxlen */
        2,                                               /* curlen */
        app_bas_battery_level_client_char_config,        /* attribute data */
    },
};

/* Number of Lookup Table entries */
const uint16_t app_gatt_db_ext_attr_tbl_size = 
    (sizeof(app_gatt_db_ext_attr_tbl) / sizeof(gatt_db_lookup_table_t));

/* Number of GATT initial value arrays entries */
const uint16_t app_gap_device_name_len = 14;
const uint16_t app_gap_appearance_len = (sizeof(app_gap_appearance));
const uint16_t app_bas_battery_level_len = (sizeof(app_bas_battery_level));
const uint16_t app_bas_battery_level_char_presentation_format_len =
    (sizeof(app_bas_battery_level_char_presentation_format));
const uint16_t app_bas_battery_level_client_char_config_len =
    (sizeof(app_bas_battery_level_client_char_config));
