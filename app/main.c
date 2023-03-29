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
 * File Name: main.c
 *
 * Description: Entry file for Battery Server application.
 *
 * Related Document: See README.md
 *
 *****************************************************************************/
/******************************************************************************
 *                                INCLUDES
 *****************************************************************************/
#include "utils_arg_parser.h"
#include "bas.h"

/******************************************************************************
 *                                MACROS
 *****************************************************************************/
#define MAX_PATH ( 256 )
#define IP_ADDR_LEN ( 16 )
#define PARSE_ERROR ( -1 )
#define BT_STACK_HEAP_SIZE (0xF000)

/******************************************************************************
 *                                GLOBAL VARIABLE
 *****************************************************************************/
uint8_t bas_bd_address[LOCAL_BDA_LEN] = {0x11, 0x12, 0x13, 0x31, 0x32, 0x33};

/*******************************************************************************
*        Function Prototypes
*******************************************************************************/
void APPLICATION_START(void);

/******************************************************************************
*                               FUNCTION DEFINITIONS
******************************************************************************/

/******************************************************************************
 * Function Name: hci_control_proc_rx_cmd()
 ******************************************************************************
 * Summary:
 *   Function to handle HCI receive
 *
 * Parameters:
 *   uint8_t* p_buffer    : rx buffer
 *   uint32_t length      : rx buffer length
 *
 * Return:
 *  status code
 *
 *****************************************************************************/
uint32_t hci_control_proc_rx_cmd( uint8_t *p_buffer, uint32_t length )
{
    return 0;
}

/*******************************************************************************
* Function Name: APPLICATION_START()
********************************************************************************
* Summary:
*   BT stack initialization function wrapper
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void APPLICATION_START(void)
{
    wiced_result_t wiced_result;
    wiced_bt_heap_t *p_default_heap   = NULL;

    WICED_BT_TRACE("************* BAS Application Start *******************\n");

    /* Register call back and configuration with stack */
    wiced_result = wiced_bt_stack_init (app_bt_management_callback, \
    &wiced_bt_cfg_settings);

    /* Check if stack initialization was successful */
    if( WICED_BT_SUCCESS == wiced_result)
    {
        WICED_BT_TRACE("Bluetooth Stack Initialization Successful \n");
        /* Create default heap */
        p_default_heap = wiced_bt_create_heap("default_heap", NULL, 
        BT_STACK_HEAP_SIZE, NULL, WICED_TRUE);
        if (p_default_heap == NULL)
        {
            WICED_BT_TRACE_CRIT("create default heap error: size %d\n", 
            BT_STACK_HEAP_SIZE);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
       WICED_BT_TRACE_CRIT("Bluetooth Stack Initialization failed!! \n");
       exit(EXIT_FAILURE);
    }
}

/******************************************************************************
 * Function Name: main()
 ******************************************************************************
 * Summary:
 *  Application entry function
 *
 * Parameters:
 *  int argc            : argument count
 *  char *argv[]        : list of arguments
 *
 * Return:
 *  None
 *
 *****************************************************************************/
int main( int argc, char *argv[] )
{
    char fw_patch_file[MAX_PATH];
    char hci_port[MAX_PATH];
    char peer_ip_addr[IP_ADDR_LEN] = "000.000.000.000";
    uint32_t hci_baudrate = 0;
    uint32_t patch_baudrate = 0;
    int btspy_inst = 0;
    uint8_t btspy_is_tcp_socket = 0;
    /* Audobaud configuration GPIO bank and pin */
    cybt_controller_autobaud_config_t autobaud;


    memset( fw_patch_file, 0, MAX_PATH );
    memset( hci_port, 0, MAX_PATH );
    if ( PARSE_ERROR == arg_parser_get_args( argc,
                                           argv,
                                           hci_port,
                                           bas_bd_address,
                                           &hci_baudrate,
                                           &btspy_inst,
                                           peer_ip_addr,
                                           &btspy_is_tcp_socket,
                                           fw_patch_file,
                                           &patch_baudrate,
                                           &autobaud ) )
    {
        return EXIT_FAILURE;
    }

    cy_platform_bluetooth_init( fw_patch_file, hci_port, hci_baudrate,
    patch_baudrate, &autobaud );

    WICED_BT_TRACE("Linux CE BAS project initialization complete....\n");
    for(;;);

    return EXIT_SUCCESS;
}
/* [] END OF FILE */
