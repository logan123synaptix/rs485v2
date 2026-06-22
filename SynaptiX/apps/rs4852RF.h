#ifndef RS4852RF_H
#define RS4852RF_H

#if __cplusplus
extern "C" {
#endif

#include "app_config.h"


/**
 * @brief Initializes the RS485 to RF communication module.
 *
 * This function sets up all necessary hardware and software resources
 * required to enable communication from RS485 to RF.
 *
 * Call this function before using any RS485 to RF features.
 */
void rs485_to_rf_init();
/**
 * @brief Polls the RS485 interface and forwards received data to the RF module.
 *
 * This function should be called periodically to check for incoming data on the RS485 bus.
 * Any received data will be processed and transmitted over the RF interface.
 */
void rs485_to_rf_poll();


#if __cplusplus
}
#endif

#endif // RS4852RF

