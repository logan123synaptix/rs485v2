#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "board.h"

#define SN_LENGTH 32
#define PRODUCT_SN "101SX485RFIO0000000000000000000"

/**
 * @def APPLICATION Version
 * @brief
 *
 * 
 */
#define APP_VERSION 0x01000002

#ifndef APPLICATION_TYPE
#define APPLICATION_TYPE
#endif

#define APPLICATION_MODE_GPIO BSP_IO_ENABLE

#define APPLICATION_MODE_ANALOG BSP_ANALOG_ENABLE

/**
 * @def BUTTON ENABLE
 * @brief Enables or disables Button functionality.
 *
 * Set to 1 to enable BUTTON features, or 0 to disable them.
 */
#define APP_BUTTON_EN 1

#if APPLICATION_MODE_ANALOG

#ifndef ANALOGS_DEBUG
#define ANALOGS_DEBUG 0
#endif

#endif

/**
 * @def ZIGBEE_ENABLE
 * @brief Enables or disables Zigbee functionality.
 * 
 * Set to 1 to enable Zigbee features in the application.
 * Set to 0 to disable Zigbee features.
 */
#define ZIGBEE_ENABLE 1

/**
 * @def LORA_ENABLE
 * @brief Enables or disables LoRa functionality.
 *
 * Set to 1 to enable LoRa features, or 0 to disable them.
 */
#define LORA_ENABLE 0

#ifdef __cplusplus
}
#endif

#endif