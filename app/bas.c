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
 * File Name: bas.c
 *
 * Description: Battery Server functions.
 *
 * Related Document: See README.md
 *
 *****************************************************************************/
/******************************************************************************
 *                                INCLUDES
 *****************************************************************************/
#include "bas.h"

/******************************************************************************
 *                                GLOBAL VARIABLE
 *****************************************************************************/
uint16_t bt_conn_id;
wiced_timer_t notification_timer;
static app_bt_adv_conn_mode_t app_bt_adv_conn_state = APP_BT_ADV_OFF_CONN_OFF;
typedef void (*pfn_free_buffer_t)(uint8_t *);
extern wiced_bt_ble_advert_elem_t cy_bt_adv_packet_data[];

/******************************************************************************
 *                               FUNCTION DEFINITIONS
 ******************************************************************************/
/******************************************************************************
 * Function Name: app_bt_alloc_buffer()
 ******************************************************************************
 * Summary:
 *   Function to allocate memory from default heap.
 *
 * Parameters:
 *   uint16_t len       : Required buffer length
 *
 * Return:
 *  uint8_t *: Allocated buffer
 *
 *****************************************************************************/
uint8_t *app_bt_alloc_buffer(uint16_t len)
{
    uint8_t *p = (uint8_t *)malloc(len);
    WICED_BT_TRACE("%s() len %d alloc %p \r\n", __FUNCTION__, len, p);
    return p;
}

/******************************************************************************
 * Function Name: app_bt_free_buffer()
 ******************************************************************************
 * Summary:
 *   Function to free allocated buffer memory.
 *
 * Parameters:
 *   uint8_t *p_data             : Buffer pointer
 *
 * Return:
 *  None
 *
 *****************************************************************************/
void app_bt_free_buffer(uint8_t *p_data)
{
    if (p_data != NULL)
    {
        WICED_BT_TRACE("%s()        free:%p \r\n", __FUNCTION__, p_data);
        free(p_data);
    }
}

/******************************************************************************
 * Function Name: app_timer_init()
 *******************************************************************************
 * Summary:
 *   Function to initialize application specific timer
 *
 * Parameters:
 *   None
 *
 * Return:
 *  None
 *
 ******************************************************************************/
void app_timer_init(void)
{
    wiced_init_timer(&notification_timer, send_notification_timer_cb, 0,
                     WICED_SECONDS_PERIODIC_TIMER);
}

/*******************************************************************************
 * Function Name: send_notification_timer_cb()
 *******************************************************************************
 *
 * Summary:
 *   One millisecond timer callback.
 *   Send GATT notifications if enabled by the GATT Client.
 *
 * Parameters:
 *   WICED_TIMER_PARAM_TYPE signo   :   The argument parameter is
 *                                used in this callback as a signal
 *
 * Return:
 *   None
 *
 ******************************************************************************/
void send_notification_timer_cb(WICED_TIMER_PARAM_TYPE signo)
{
    WICED_BT_TRACE("[%s] \n", __FUNCTION__);
    if (0 == app_bas_battery_level[0])
    {
        app_bas_battery_level[0] = 100;
    }
    else
    {
        app_bas_battery_level[0] = app_bas_battery_level[0] -
                                   BATTERY_LEVEL_CHANGE;
    }
    if (bt_conn_id)
    {
        if (app_bas_battery_level_client_char_config[0] &
            GATT_CLIENT_CONFIG_NOTIFICATION)
        {
            WICED_BT_TRACE("size of buffer: [%d] \n", app_bas_battery_level_len);
            wiced_bt_gatt_server_send_notification(bt_conn_id,
                    HDLC_BAS_BATTERY_LEVEL_VALUE, app_bas_battery_level_len,
                    app_bas_battery_level, NULL);
            WICED_BT_TRACE("Sending Notification: Battery level: [%u] \n",
                           app_bas_battery_level[0]);
        }
    }
}

/******************************************************************************
 * Function Name: app_bt_set_value()
 ******************************************************************************
 * Summary:
 *   The function is invoked by app_bt_write_handler to set a value
 *         to GATT DB.
 *
 * Parameters:
 *  param attr_handle   :   GATT attribute handle
 *  param p_val         :   Pointer to BLE GATT write request value
 *  param len           :   length of GATT write request
 *
 * Return:
 *  wiced_bt_gatt_status_t  BLE GATT status
 *
 *****************************************************************************/

wiced_bt_gatt_status_t app_bt_set_value(uint16_t attr_handle, uint8_t *p_val,
                                        uint16_t len)
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_INVALID_HANDLE;

    WICED_BT_TRACE("%s() handle : 0x%x (%d)\r\n", __func__,
                   attr_handle, attr_handle);

    for (int i = 0; i < app_gatt_db_ext_attr_tbl_size; i++)
    {
        /* Check for a matching handle entry */
        if (app_gatt_db_ext_attr_tbl[i].handle == attr_handle)
        {
            /* Detected a matching handle in the external lookup table */
            if (app_gatt_db_ext_attr_tbl[i].max_len >= len)
            {
                /* Value fits within the supplied buffer; copy over the value */
                app_gatt_db_ext_attr_tbl[i].cur_len = len;
                memset(app_gatt_db_ext_attr_tbl[i].p_data, 0x00,
                       app_gatt_db_ext_attr_tbl[i].max_len);
                memcpy(app_gatt_db_ext_attr_tbl[i].p_data, p_val,
                       app_gatt_db_ext_attr_tbl[i].cur_len);

                if (memcmp(app_gatt_db_ext_attr_tbl[i].p_data, p_val,
                           app_gatt_db_ext_attr_tbl[i].cur_len) == 0)
                {
                    status = WICED_BT_GATT_SUCCESS;
                }

                if (app_gatt_db_ext_attr_tbl[i].handle ==
                    HDLD_BAS_BATTERY_LEVEL_CLIENT_CHAR_CONFIG)
                {
                    if (GATT_CLIENT_CONFIG_NOTIFICATION ==
                        app_bas_battery_level_client_char_config[0])
                    {
                        WICED_BT_TRACE("Battery Server Notifications \
                        Enabled \r\n");
                        wiced_start_timer(&notification_timer, 1);
                    }
                    else
                    {
                        WICED_BT_TRACE("Battery Server Notifications Disabled \
                        \r\n");
                        wiced_stop_timer(&notification_timer);
                    }
                }
            }
            else
            {
                /* Value to write will not fit within the table */
                status = WICED_BT_GATT_INVALID_ATTR_LEN;
                WICED_BT_TRACE("Invalid attribute length\r\n");
            }
            break;
        }
    }
    if (WICED_BT_GATT_SUCCESS != status)
    {
        WICED_BT_TRACE_CRIT("%s() FAILED %d \r\n", __func__, status);
    }
    return status;
}

/******************************************************************************
 * Function Name: app_bt_management_callback()
 ******************************************************************************
 * Summary:
 *   This is a Bluetooth stack event handler function to receive management
 *   events from the BLE stack and process as per the application.
 *
 * Parameters:
 *   wiced_bt_management_evt_t event : BLE event code of one byte length
 *   wiced_bt_management_evt_data_t *p_event_data: Pointer to BLE management
 *   event structures
 *
 * Return:
 *  wiced_result_t: Error code from WICED_RESULT_LIST or BT_RESULT_LIST
 *
 *****************************************************************************/
wiced_result_t app_bt_management_callback(wiced_bt_management_evt_t event,
                                wiced_bt_management_evt_data_t *p_event_data)
{
    wiced_bt_device_address_t bda = {0}; /* Device BD address */
    wiced_bt_ble_advert_mode_t *p_adv_mode = NULL;

    switch (event)
    {
    case BTM_ENABLED_EVT:
        /* Bluetooth Controller and Host Stack Enabled */
        if (WICED_BT_SUCCESS == p_event_data->enabled.status)
        {
            wiced_bt_set_local_bdaddr(bas_bd_address,
                                      BLE_ADDR_PUBLIC);
            wiced_bt_dev_read_local_addr(bda);
            WICED_BT_TRACE("Local Bluetooth Address: ");
            print_bd_address(bda);

            /* Perform application-specific initialization */
            app_bt_init();
        }
        else
        {
            WICED_BT_TRACE("Bluetooth Enabling Failed \n");
        }

        break;

    case BTM_DISABLED_EVT:
        WICED_BT_TRACE("Bluetooth Disabled \n");
        break;

    case BTM_BLE_ADVERT_STATE_CHANGED_EVT:

        /* Advertisement State Changed */
        p_adv_mode = &p_event_data->ble_advert_state_changed;
        WICED_BT_TRACE("Advertisement State Change: %s\n",
                       get_bt_advert_mode_name(*p_adv_mode));

        if (BTM_BLE_ADVERT_OFF == *p_adv_mode)
        {
            /* Advertisement Stopped */
            WICED_BT_TRACE("Advertisement stopped\n");

            /* Check connection status after advertisement stops */
            if (0 == bt_conn_id)
            {
                app_bt_adv_conn_state = APP_BT_ADV_OFF_CONN_OFF;
            }
            else
            {
                app_bt_adv_conn_state = APP_BT_ADV_OFF_CONN_ON;
            }
        }
        else
        {
            /* Advertisement Started */
            WICED_BT_TRACE("Advertisement started\n");
            app_bt_adv_conn_state = APP_BT_ADV_ON_CONN_OFF;
        }

        break;

    case BTM_BLE_CONNECTION_PARAM_UPDATE:
        WICED_BT_TRACE("Connection parameter update status:%d, \
        Connection Interval: %d, \
        Connection Latency: %d, Connection Timeout: %d\n",
                       p_event_data->ble_connection_param_update.status,
                       p_event_data->ble_connection_param_update.conn_interval,
                       p_event_data->ble_connection_param_update.conn_latency,
                       p_event_data->ble_connection_param_update.supervision_timeout);
        break;
    default:
        WICED_BT_TRACE("Unhandled Bluetooth Management Event: 0x%x %s\n",
                       event, get_bt_event_name(event));
        break;
    }
    return WICED_BT_SUCCESS;
}

/******************************************************************************
 * Function Name: app_bt_init()
 ******************************************************************************
 * Summary:
 *   This function handles application level initialization tasks and is
 *   called from the BT management callback once the BLE stack enabled
 *   event (BTM_ENABLED_EVT) is triggered
 *   This function is executed in the BTM_ENABLED_EVT management callback.
 *
 * Parameters:
 *   None
 *
 * Return:
 *   None
 *
 *****************************************************************************/
void app_bt_init(void)
{
    wiced_result_t wiced_result = WICED_BT_SUCCESS;
    wiced_bt_gatt_status_t gatt_status = WICED_BT_GATT_SUCCESS;

    WICED_BT_TRACE("\n***********************************************\n");
    WICED_BT_TRACE("**Discover device with \"Battery Server\" name*\n");
    WICED_BT_TRACE("***********************************************\n\n");

    wiced_bt_set_pairable_mode(FALSE, FALSE);

    /* Set Advertisement Data */
    wiced_bt_ble_set_raw_advertisement_data(CY_BT_ADV_PACKET_DATA_SIZE,
                                            cy_bt_adv_packet_data);

    /* Register with BT stack to receive GATT callback */
    gatt_status = wiced_bt_gatt_register(app_bt_gatt_event_callback);
    WICED_BT_TRACE("GATT event Handler registration status: %s \n",
                   get_bt_gatt_status_name(gatt_status));

    /* Initialize GATT Database */
    gatt_status = wiced_bt_gatt_db_init(gatt_database, gatt_database_len, NULL);
    WICED_BT_TRACE("GATT database initialization status: %s \n",
                   get_bt_gatt_status_name(gatt_status));

    /* Start Undirected LE Advertisements on device startup.
     * The corresponding parameters are contained in 'app_bt_cfg.c' */
    wiced_result = wiced_bt_start_advertisements(
        BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL);

    /* Failed to start advertisement. Stop program execution */
    if (WICED_BT_SUCCESS != wiced_result)
    {
        WICED_BT_TRACE_CRIT("failed to start advertisement! \n");
        exit(0);
    }
    app_timer_init();
}

/******************************************************************************
 * Function Name: app_bt_gatt_event_callback()
 ******************************************************************************
 * Summary:
 *   This function handles GATT events from the BT stack.
 *
 * Parameters:
 *   wiced_bt_gatt_evt_t event : BLE GATT event code of one byte length
 *   wiced_bt_gatt_event_data_t *p_event_data : Pointer to
 *       BLE GATT event structures
 *
 * Return:
 *  wiced_bt_gatt_status_t: See possible status codes in
 *       wiced_bt_gatt_status_e in wiced_bt_gatt.h
 *
 *****************************************************************************/
wiced_bt_gatt_status_t app_bt_gatt_event_callback(
    wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data)
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_ERROR;

    /* Call the appropriate callback function based on the GATT event type,
     * and pass the relevant event
     * parameters to the callback function */
    switch (event)
    {
    case GATT_CONNECTION_STATUS_EVT:
        status = app_bt_connect_event_handler(
            &p_event_data->connection_status);
        break;

    case GATT_ATTRIBUTE_REQUEST_EVT:
        status = app_bt_server_event_handler(p_event_data);
        break;
        /* GATT buffer request, typically sized to max of bearer mtu - 1 */
    case GATT_GET_RESPONSE_BUFFER_EVT:
        WICED_BT_TRACE("%s() GATT_GET_RESPONSE_BUFFER_EVT\r\n", __func__);
        p_event_data->buffer_request.buffer.p_app_rsp_buffer =
            app_bt_alloc_buffer(p_event_data->buffer_request.len_requested);
        p_event_data->buffer_request.buffer.p_app_ctxt = (void *)
            app_bt_free_buffer;
        status = WICED_BT_GATT_SUCCESS;
        break;
        /* GATT buffer transmitted event,  check \ref
        wiced_bt_gatt_buffer_transmitted_t*/
    case GATT_APP_BUFFER_TRANSMITTED_EVT:
        WICED_BT_TRACE("\r\n\n%s() GATT_APP_BUFFER_TRANSMITTED_EVT.\r\n",
                       __func__);
        {
            pfn_free_buffer_t pfn_free = (pfn_free_buffer_t)
                                    p_event_data->buffer_xmitted.p_app_ctxt;

            /* If the buffer is dynamic, the context will point to a function to
            free it. */
            if (pfn_free)
                pfn_free(p_event_data->buffer_xmitted.p_app_data);

            status = WICED_BT_GATT_SUCCESS;
        }
        break;

    default:
        WICED_BT_TRACE(" Unhandled GATT Event \r\n");
        status = WICED_BT_GATT_SUCCESS;
        break;
    }

    return status;
}

/******************************************************************************
 * Function Name: app_bt_connect_event_handler()
 ******************************************************************************
 * Summary:
 *   This callback function handles connection status changes.
 *
 * Parameters:
 *   wiced_bt_gatt_connection_status_t *p_conn_status  : Pointer to data
 *                                           that has connection details
 *
 * Return:
 *  wiced_bt_gatt_status_t: See possible status codes in
 *                          wiced_bt_gatt_status_e in wiced_bt_gatt.h
 *
 *****************************************************************************/
wiced_bt_gatt_status_t app_bt_connect_event_handler(
    wiced_bt_gatt_connection_status_t *p_conn_status)
{
    wiced_bt_gatt_status_t gatt_status = WICED_BT_GATT_SUCCESS;

    if (NULL != p_conn_status)
    {
        if (p_conn_status->connected)
        {
            /* Device has connected */
            WICED_BT_TRACE("Connected : BDA ");
            print_bd_address(p_conn_status->bd_addr);
            WICED_BT_TRACE("Connection ID '%d' \n", p_conn_status->conn_id);

            /* Store the connection ID */
            bt_conn_id = p_conn_status->conn_id;

            /* Update the adv/conn state */
            app_bt_adv_conn_state = APP_BT_ADV_OFF_CONN_ON;
        }
        else
        {
            /* Device has disconnected */
            WICED_BT_TRACE("Disconnected : BDA ");
            print_bd_address(p_conn_status->bd_addr);
            WICED_BT_TRACE("Connection ID '%d', Reason '%s'\n",
                           p_conn_status->conn_id,
                           get_bt_gatt_disconn_reason_name(p_conn_status->reason));

            /* Set the connection id to zero to indicate disconnected state */
            bt_conn_id = 0;

            /* Restart the advertisements */
            wiced_bt_start_advertisements(
                BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL);

            /* Update the adv/conn state */
            app_bt_adv_conn_state = APP_BT_ADV_ON_CONN_OFF;
        }
    }
    else
    {
        gatt_status = WICED_BT_GATT_ERROR;
    }
    return gatt_status;
}

/*******************************************************************************
 * Function Name: app_bt_server_event_handler
 ********************************************************************************
 * Summary:
 *   This function handles GATT server events from the BT stack.
 *
 * Parameters:
 *  p_data     Pointer to LE GATT connection status
 *
 * Return:
 *  wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e
 *  in wiced_bt_gatt.h
 *
 *******************************************************************************/
wiced_bt_gatt_status_t app_bt_server_event_handler(
    wiced_bt_gatt_event_data_t *p_data)
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_ERROR;
    wiced_bt_gatt_attribute_request_t *p_att_req = &p_data->attribute_request;

    switch (p_att_req->opcode)
    {
    /* Attribute read notification (attribute value internally read from GATT
    database) */
    case GATT_REQ_READ:
    case GATT_REQ_READ_BLOB:
        WICED_BT_TRACE("  %s() GATTS_REQ_TYPE_READ\r\n", __func__);
        status = app_bt_gatt_req_read_handler(p_att_req->conn_id,
                    p_att_req->opcode, &p_att_req->data.read_req, 
                    p_att_req->len_requested);
        break;

    case GATT_REQ_READ_BY_TYPE:
        status = app_bt_gatt_req_read_by_type_handler(
            p_att_req->conn_id, p_att_req->opcode, &p_att_req->data.read_by_type,
            p_att_req->len_requested);
        break;

    case GATT_REQ_READ_MULTI:
    case GATT_REQ_READ_MULTI_VAR_LENGTH:
        status = app_bt_gatt_req_read_multi_handler(
            p_att_req->conn_id, p_att_req->opcode,
            &p_att_req->data.read_multiple_req,
            p_att_req->len_requested);
        break;

    case GATT_REQ_WRITE:
    case GATT_CMD_WRITE:
    case GATT_CMD_SIGNED_WRITE:
        WICED_BT_TRACE("  %s() GATTS_REQ_WRITE\r\n", __func__);
        status = app_bt_write_handler(p_data);
        if ((p_att_req->opcode == GATT_REQ_WRITE) && (status ==
                                                      WICED_BT_GATT_SUCCESS))
        {
            wiced_bt_gatt_write_req_t *p_write_request =
                &p_att_req->data.write_req;
            wiced_bt_gatt_server_send_write_rsp(p_att_req->conn_id,
                                p_att_req->opcode, p_write_request->handle);
        }
        break;

    case GATT_REQ_PREPARE_WRITE:
        WICED_BT_TRACE("  %s() GATT_REQ_PREPARE_WRITE\r\n", __func__);
        status = WICED_BT_GATT_SUCCESS;
        break;

    case GATT_REQ_EXECUTE_WRITE:
        WICED_BT_TRACE("  %s() GATTS_REQ_TYPE_WRITE_EXEC - nothing to do \
        here.\r\n",
                       __func__);
        wiced_bt_gatt_server_send_execute_write_rsp(
            p_att_req->conn_id, p_att_req->opcode);
        status = WICED_BT_GATT_SUCCESS;
        break;

    case GATT_REQ_MTU:
        /* Application calls wiced_bt_gatt_server_send_mtu_rsp() with the
        desired mtu */
        WICED_BT_TRACE("  %s() GATTS_REQ_TYPE_MTU\r\n", __func__);
        status = wiced_bt_gatt_server_send_mtu_rsp(p_att_req->conn_id,
                        p_att_req->data.remote_mtu,
                        wiced_bt_cfg_settings.p_ble_cfg->ble_max_rx_pdu_size);
        WICED_BT_TRACE("    Set MTU size to: %d  status: 0x%d\r\n",
                       p_att_req->data.remote_mtu, status);
        break;

    case GATT_HANDLE_VALUE_CONF: /* Value confirmation */
        WICED_BT_TRACE("  %s() GATTS_REQ_TYPE_CONF\r\n", __func__);
        status = WICED_BT_GATT_SUCCESS;
        break;

    case GATT_HANDLE_VALUE_NOTIF:
        WICED_BT_TRACE("  %s() GATT_HANDLE_VALUE_NOTIF - Client received our \
        notification\r\n",
                       __func__);
        status = WICED_BT_GATT_SUCCESS;
        break;

    default:
        WICED_BT_TRACE("  %s() Unhandled Event opcode: %d\r\n",
                       __func__, p_att_req->opcode);
        break;
    }
    return status;
}

/******************************************************************************
 * Function Name: app_bt_write_handler()
 ******************************************************************************
 * Summary:
 *  The function is invoked when GATTS_REQ_TYPE_WRITE is received from
 *  the client device and is invoked GATT Server Event Callback function. This
 *  handles "Write Requests" received from Client device.
 *
 * Parameters:
 *   wiced_bt_gatt_event_data_t *p_data    Pointer to BLE GATT write request
 *
 * Return:
 *  wiced_bt_gatt_status_t  BLE GATT status
 *
 *****************************************************************************/
wiced_bt_gatt_status_t app_bt_write_handler(
    wiced_bt_gatt_event_data_t *p_data)
{
    wiced_bt_gatt_write_req_t *p_write_req =
        &p_data->attribute_request.data.write_req;
    if ((NULL == p_data) && (NULL == p_write_req))
    {
        WICED_BT_TRACE("Invalid data received in write request handler\n");
        return WICED_BT_GATT_INVALID_HANDLE;
    }

    return app_bt_set_value(p_write_req->handle,
                            p_write_req->p_val, p_write_req->val_len);
}

/******************************************************************************
 * Function Name: app_bt_find_by_handle()
 ******************************************************************************
 * Summary:
 *  Find attribute description by handle
 *
 * Parameters:
 *   uint16_t handle    handle to look up
 *
 * Return:
 *  gatt_db_lookup_table_t   pointer containing handle data
 *
 *****************************************************************************/
gatt_db_lookup_table_t *app_bt_find_by_handle(uint16_t handle)
{
    int i;
    for (i = 0; i < app_gatt_db_ext_attr_tbl_size; i++)
    {
        if (app_gatt_db_ext_attr_tbl[i].handle == handle)
        {
            return (&app_gatt_db_ext_attr_tbl[i]);
        }
    }
    return NULL;
}

/******************************************************************************
 * Function Name: app_bt_gatt_req_read_handler()
 ******************************************************************************
 * Summary:
 *  This Function Process read request from peer device
 *
 * Parameters:
 *  param conn_id       Connection ID
 *  param opcode        BLE GATT request type opcode
 *  param p_read_req    Pointer to read request containing the handle to read
 *  param len_requested length of data requested
 *
 * Return:
 *  wiced_bt_gatt_status_t  BLE GATT status
 *
 *****************************************************************************/
wiced_bt_gatt_status_t app_bt_gatt_req_read_handler(uint16_t conn_id,
            wiced_bt_gatt_opcode_t opcode, wiced_bt_gatt_read_t *p_read_req,
            uint16_t len_requested)
{
    gatt_db_lookup_table_t *puAttribute;
    uint16_t attr_len_to_copy, to_send;
    uint8_t *from;

    if ((puAttribute = app_bt_find_by_handle(p_read_req->handle)) == NULL)
    {
        WICED_BT_TRACE("%s()  Attribute not found, Handle: 0x%04x\r\n",
                       __func__, p_read_req->handle);
        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, p_read_req->handle,
                                            WICED_BT_GATT_INVALID_HANDLE);
        return WICED_BT_GATT_INVALID_HANDLE;
    }

    attr_len_to_copy = puAttribute->cur_len;
    WICED_BT_TRACE("%s() conn_id: %d handle:0x%04x offset:%d len:%d\r\n",
                    __func__, conn_id, p_read_req->handle, p_read_req->offset, 
                    attr_len_to_copy);

    if (p_read_req->offset >= puAttribute->cur_len)
    {
        WICED_BT_TRACE("%s() offset:%d larger than attribute length:%d\r\n",
                       __func__, p_read_req->offset, puAttribute->cur_len);

        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, p_read_req->handle,
                                            WICED_BT_GATT_INVALID_OFFSET);
        return WICED_BT_GATT_INVALID_OFFSET;
    }

    to_send = MIN(len_requested, attr_len_to_copy - p_read_req->offset);
    from = puAttribute->p_data + p_read_req->offset;

    /* No need for context argument as buff not allocated */
    return wiced_bt_gatt_server_send_read_handle_rsp(conn_id, opcode, to_send,
                                                     from, NULL); 
}

/******************************************************************************
 * Function Name: app_bt_gatt_req_read_by_type_handler()
 ******************************************************************************
 * Summary:
 *  Process read-by-type request from peer device
 *
 * Parameters:
 *  param conn_id       Connection ID
 *  param opcode        BLE GATT request type opcode
 *  param p_read_req    Pointer to read request containing the handle to read
 *  param len_requested length of data requested
 *
 * Return:
 *  wiced_bt_gatt_status_t  BLE GATT status
 *
 *****************************************************************************/
wiced_bt_gatt_status_t app_bt_gatt_req_read_by_type_handler(
    uint16_t conn_id, wiced_bt_gatt_opcode_t opcode,
    wiced_bt_gatt_read_by_type_t *p_read_req, uint16_t len_requested)
{
    gatt_db_lookup_table_t *puAttribute;
    uint16_t last_handle = 0;
    uint16_t attr_handle = p_read_req->s_handle;
    uint8_t *p_rsp = app_bt_alloc_buffer(len_requested);
    uint8_t pair_len = 0;
    int used = 0;

    if (p_rsp == NULL)
    {
        WICED_BT_TRACE("%s() No memory, len_requested: %d!!\r\n",
                       __func__, len_requested);

        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, attr_handle,
                                            WICED_BT_GATT_INSUF_RESOURCE);
        return WICED_BT_GATT_INSUF_RESOURCE;
    }

    /* Read by type returns all attributes of the specified type, between the
    start and end handles */
    while (WICED_TRUE)
    {
        last_handle = attr_handle;
        attr_handle = wiced_bt_gatt_find_handle_by_type(
            attr_handle, p_read_req->e_handle, &p_read_req->uuid);

        if (attr_handle == 0)
            break;

        if ((puAttribute = app_bt_find_by_handle(attr_handle)) == NULL)
        {
            WICED_BT_TRACE("%s()  found type but no attribute for %d \r\n",
                           __func__, last_handle);
            wiced_bt_gatt_server_send_error_rsp(conn_id, opcode,
                                                p_read_req->s_handle, WICED_BT_GATT_ERR_UNLIKELY);
            app_bt_free_buffer(p_rsp);
            return WICED_BT_GATT_INVALID_HANDLE;
        }

        {
            int filled = wiced_bt_gatt_put_read_by_type_rsp_in_stream(p_rsp +
                            used, len_requested - used, &pair_len, attr_handle,
                                    puAttribute->cur_len, puAttribute->p_data);
            if (filled == 0)
            {
                break;
            }
            used += filled;
        }

        /* Increment starting handle for next search to one past current */
        attr_handle++;
    }

    if (used == 0)
    {
        WICED_BT_TRACE("%s()  attr not found  start_handle: 0x%04x  "
                       "end_handle: 0x%04x  Type: 0x%04x\r\n",
                       __func__, p_read_req->s_handle, p_read_req->e_handle,
                       p_read_req->uuid.uu.uuid16);

        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode,
                        p_read_req->s_handle, WICED_BT_GATT_INVALID_HANDLE);
        app_bt_free_buffer(p_rsp);
        return WICED_BT_GATT_INVALID_HANDLE;
    }

    /* Send the response */
    wiced_bt_gatt_server_send_read_by_type_rsp(conn_id, opcode, pair_len,
                                    used, p_rsp, (void *)app_bt_free_buffer);

    return WICED_BT_GATT_SUCCESS;
}

/******************************************************************************
 * Function Name: app_bt_gatt_req_read_multi_handler()
 ******************************************************************************
 * Summary:
 *  Process write read multi request from peer device
 *
 * Parameters:
 *  conn_id       Connection ID
 *  opcode        BLE GATT request type opcode
 *  p_read_req    Pointer to read request containing the handle to read
 *  len_requested length of data requested
 *
 * Return:
 *  wiced_bt_gatt_status_t  BLE GATT status
 *
 *****************************************************************************/
wiced_bt_gatt_status_t app_bt_gatt_req_read_multi_handler(
    uint16_t conn_id, wiced_bt_gatt_opcode_t opcode,
    wiced_bt_gatt_read_multiple_req_t *p_read_req, uint16_t len_requested)
{
    gatt_db_lookup_table_t *puAttribute;
    uint8_t *p_rsp = app_bt_alloc_buffer(len_requested);
    int used = 0;
    int xx;
    uint16_t handle = wiced_bt_gatt_get_handle_from_stream(
        p_read_req->p_handle_stream, 0);

    if (p_rsp == NULL)
    {
        WICED_BT_TRACE("line = %d fun = %s\n", __LINE__, __func__);
        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode, handle,
                                            WICED_BT_GATT_INSUF_RESOURCE);
        return WICED_BT_GATT_INVALID_HANDLE;
    }

    /* Read by type returns all attributes of the specified type, between the
     * start and end handles */
    for (xx = 0; xx < p_read_req->num_handles; xx++)
    {
        handle = wiced_bt_gatt_get_handle_from_stream(
            p_read_req->p_handle_stream, xx);
        if ((puAttribute = app_bt_find_by_handle(handle)) == NULL)
        {
            WICED_BT_TRACE("%s()  no handle 0x%04x\r\n",
                           __func__, handle);
            wiced_bt_gatt_server_send_error_rsp(conn_id, opcode,
                    *p_read_req->p_handle_stream, WICED_BT_GATT_ERR_UNLIKELY);
            app_bt_free_buffer(p_rsp);
            return WICED_BT_GATT_ERR_UNLIKELY;
        }

        {
            int filled = wiced_bt_gatt_put_read_multi_rsp_in_stream(opcode,
                            p_rsp + used, len_requested - used, puAttribute->handle,
                                        puAttribute->cur_len, puAttribute->p_data);
            if (!filled)
            {
                break;
            }
            used += filled;
        }
    }

    if (used == 0)
    {
        WICED_BT_TRACE("%s() no attr found\r\n", __func__);

        wiced_bt_gatt_server_send_error_rsp(conn_id, opcode,
                                            *p_read_req->p_handle_stream,
                                            WICED_BT_GATT_INVALID_HANDLE);
        return WICED_BT_GATT_INVALID_HANDLE;
    }

    /* Send the response */
    wiced_bt_gatt_server_send_read_multiple_rsp(conn_id, opcode, used, p_rsp,
                                                (void *)app_bt_free_buffer);

    return WICED_BT_GATT_SUCCESS;
}