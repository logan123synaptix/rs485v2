#include "rf_app.h"
#include "board.h"
#include "logger.h"
#include "app_config.h"
#include "indicator.h"

static const char *TAG_Driver = "RFDriver";
static const char *TAG = "RFApp";

Indicator_t indicator;

#if LORA_ENABLE

LoRaMesh_t lora;
LoRaDriver_t lora_driver;

static void lora_connect_cb(LoRaMesh_t *lora, uint8_t isSuccess,void *arg){
    if (isSuccess == LORA_RES_SUCCESS)
    {
        log_info(TAG, "Connect to Lora module success");
        log_info(TAG, "Lora FW Version : V%d", lora->fw_version);
        indicator_set_net_status(&indicator,EVENT_NET_CONNECTED);
    }
    else if (isSuccess == LORA_RES_FAIL)
    {
        log_error(TAG, "Connect to Lora module fail");
        indicator_set_net_status(&indicator,EVENT_NET_DISCONNECTED);
    }
    else
    {
        log_error(TAG, "Connect to Lora module timeout");
        indicator_set_net_status(&indicator,EVENT_NET_DISCONNECTED);
    }
}
static void lora_read_cb(LoRaMesh_t *lora, uint8_t isSuccess,void *arg){
    if (isSuccess == LORA_RES_SUCCESS)
    {
        LoraParameter_t *param = &lora->param;
        log_info(TAG, "Read Lora module param success");
        log_info(TAG, "PAN ID : 0x%04X", param->PAN_ID);
        log_info(TAG, "Channel : %d", param->Channel);
        log_info(TAG, "User Address : 0x%04X", param->userAddress);
        log_info(TAG, "Point Type : %d", param->pointType);
        log_info(TAG, "Transfer Model : %d", param->transferModel);
        log_info(TAG, "Uart Baudrate : %d", param->uartBraudRate);
        log_info(TAG, "Uart Data bits : %d", param->uartDataBits);
        log_info(TAG, "Uart Stop bits : %d", param->uartStopBits);
        log_info(TAG, "Uart Parity : %d", param->uartParity);
        log_info(TAG, "Antenna Select : %d", param->antennaSelect);
        log_info(TAG, "Is Security : %d", param->isSecurity);
        log_info(TAG, "Security Code : 0x%02X 0x%02X 0x%02X 0x%02X", param->securityCode[0], param->securityCode[1], param->securityCode[2], param->securityCode[3]);
        log_info(TAG, "Short Address : 0x%04X", param->shortAddress);
        log_info(TAG, "Mac Address : 0x%02X%02X%02X%02X%02X%02X%02X%02X", param->macAddress[0], param->macAddress[1], param->macAddress[2], param->macAddress[3], param->macAddress[4], param->macAddress[5], param->macAddress[6], param->macAddress[7]);
    }
    else if(isSuccess == LORA_RES_FAIL)
    {
        log_error(TAG, "Read Lora module param fail");
    }
    else
    {
        log_error(TAG, "Read Lora module param timeout");
    }
}

#endif

#if ZIGBEE_ENABLE
ZigbeeMesh_t zigbee;
ZigbeeDriver_t zigbee_driver;
static void zigbee_connect_cb(ZigbeeMesh_t *zigbee, uint8_t isSuccess,void *arg);
static void zigbee_read_cb(ZigbeeMesh_t *zigbee, uint8_t isSuccess, void *arg);
static void zigbee_reset_cb(ZigbeeMesh_t *zigbee, uint8_t isSuccess, void *arg);
static void zigbee_connect_cb(ZigbeeMesh_t *zigbee, uint8_t isSuccess,void *arg)
{
  if (isSuccess == ZIGBEE_RES_SUCCESS)
  {
    log_info(TAG, "Connect to Zigbee module success");
    log_info(TAG, "Zigbee FW Version : V%d.%d", zigbee->fw_version[0], zigbee->fw_version[1]);
    // zigbee_read_module(&zigbee, zigbee_read_cb);
    indicator_set_net_status(&indicator,EVENT_NET_CONNECTED);
  }
  else if (isSuccess == ZIGBEE_RES_FAIL)
  {
    log_error(TAG, "Connect to Zigbee module fail");
    indicator_set_net_status(&indicator,EVENT_NET_DISCONNECTED);
  }
  else
  {
    log_error(TAG, "Connect to Zigbee module timeout");
    indicator_set_net_status(&indicator,EVENT_NET_DISCONNECTED);
  }
}

static void zigbee_read_cb(ZigbeeMesh_t *zigbee, uint8_t isSuccess,void *arg)
{
  if (isSuccess == ZIGBEE_RES_SUCCESS)
  {
    ZigbeeParameter_t *param = &zigbee->param;
    log_info(TAG, "Read Zigbee module param success");
    log_info(TAG, "PAN ID : 0x%04X", param->PAN_ID);
    log_info(TAG, "Channel : %d", param->Channel);
    log_info(TAG, "User Address : 0x%04X", param->userAddress);
    log_info(TAG, "Point Type : %d", param->pointType);
    log_info(TAG, "Transfer Model : %d", param->transferModel);
    log_info(TAG, "Uart Baudrate : %d", param->uartBraudRate);
    log_info(TAG, "Uart Data bits : %d", param->uartDataBits);
    log_info(TAG, "Uart Stop bits : %d", param->uartStopBits);
    log_info(TAG, "Uart Parity : %d", param->uartParity);
    log_info(TAG, "Antenna Select : %d", param->antennaSelect);
    log_info(TAG, "Is Security : %d", param->isSecurity);
    log_info(TAG, "Security Code : 0x%02X 0x%02X 0x%02X 0x%02X", param->securityCode[0], param->securityCode[1], param->securityCode[2], param->securityCode[3]);
    log_info(TAG, "Short Address : 0x%04X", param->shortAddress);
    log_info(TAG, "Mac Address : 0x%02X%02X%02X%02X%02X%02X%02X%02X", param->macAddress[0], param->macAddress[1], param->macAddress[2], param->macAddress[3], param->macAddress[4], param->macAddress[5], param->macAddress[6], param->macAddress[7]);
    indicator_set_net_status(&indicator,EVENT_NET_CONNECTED);
  }
  else if(isSuccess == ZIGBEE_RES_FAIL)
  {
    log_error(TAG, "Read Zigbee module param fail");
    indicator_set_net_status(&indicator,EVENT_NET_DISCONNECTED);
  }
  else
  {
    log_error(TAG, "Read Zigbee module param timeout");
    indicator_set_net_status(&indicator,EVENT_NET_DISCONNECTED);
  }
}
static void zigbee_reset_cb(ZigbeeMesh_t *zigbee, uint8_t isSuccess, void *arg){
  if (isSuccess == ZIGBEE_RES_SUCCESS)
  {
    log_info(TAG, "Reset to Zigbee module success");
    // zigbee_read_module(&zigbee, zigbee_read_cb);
    indicator_set_net_status(&indicator,EVENT_NET_CONNECTED);
  }
  else if (isSuccess == ZIGBEE_RES_FAIL)
  {
    log_error(TAG, "Reset to Zigbee module fail");
    indicator_set_net_status(&indicator,EVENT_NET_DISCONNECTED);
  }
  else
  {
    log_error(TAG, "Reset to Zigbee module timeout");
    indicator_set_net_status(&indicator,EVENT_NET_DISCONNECTED);
  }
}
#endif
uint32_t rf_time_stamp = 0;

static uint32_t rf_get_available()
{
    return bsp_com_available(BSP_RF_COM_PORT);
}
static void rf_write(uint8_t *buff, uint32_t len)
{
    log_debug(TAG_Driver, "RF Write : %u", len);
    log_print_hex(LOGGER_DEBUG, TAG_Driver, buff, len);
    bsp_com_write(BSP_RF_COM_PORT, buff, len);
}
static void rf_read(uint8_t *buff, uint32_t len){
    uint32_t length = bsp_com_read(BSP_RF_COM_PORT,buff,len);
    if(length > 0){
        log_debug(TAG_Driver, "RF Read : %u", len);
        log_print_hex(LOGGER_INFO, TAG_Driver, buff, len);
    }
}
void rf_app_init()
{
    indicator_init(&indicator);
#if LORA_ENABLE
    lora_driver.write = rf_write;
    lora_driver.read = rf_read;
    lora_driver.available = rf_get_available;
    lora_init(&lora, &lora_driver);
    lora_connect(&lora, lora_connect_cb,NULL);
    lora_read_module(&lora, lora_read_cb,NULL);
#endif
#if ZIGBEE_ENABLE
    zigbee_driver.write = rf_write;
    zigbee_driver.read = rf_read;
    zigbee_driver.available = rf_get_available;
    zigbee_init(&zigbee, &zigbee_driver);
    zigbee_connect(&zigbee, zigbee_connect_cb,NULL);
    zigbee_read_module(&zigbee, zigbee_read_cb,NULL);
    zigbee_reset_module(&zigbee,zigbee_reset_cb,NULL);
#endif
    rf_time_stamp = bsp_get_tick();
}

void rf_app_poll()
{
    if(bsp_get_tick() - rf_time_stamp >= 1){
        #if LORA_ENABLE
        lora_poll(&lora, 1);
        #endif
        #if ZIGBEE_ENABLE
        zigbee_poll(&zigbee, 1);
        #endif
        indicator_poll(&indicator, 1);
        rf_time_stamp = bsp_get_tick();
    }
    // lora_poll(&lora, timestamp);
}

bool rf_app_busy()
{
#if ZIGBEE_ENABLE
    return zigbee.event.event != ZB_EVENT_NONE ? true : false;
#endif
#if LORA_ENABLE
    return lora.event.event != LR_EVENT_NONE ? true : false;
#endif
}
