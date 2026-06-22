#include "spi.h"
#include "board.h"
#include <stdio.h>
#include "app_config.h"
#include "rs4852RF.h"
#include "mb.h"
#include "port.h"
#include "user_mb_app.h"

#include "app_settings.h"
#include "rf_app.h"
#include "button.h"
#include "logger.h"
#if APPLICATION_MODE_GPIO
#include "input.h"
#include "output.h"
#elif APPLICATION_MODE_ANALOG
#include "analogs.h"
#endif

static const char *TAG = "APP";

extern eModbus modbus[N_MODBUS];
extern void mb_uart_rx_task(eModbus *mb);

extern void shell_app_init();
extern void shell_app_poll();
extern void user_app();

// static void app_timer_callback()
// {
//     for (int i = 0; i < 4; i++)
//     {
//         if (input_reg[i + 20] < hoding_reg[i + 8])
//         {
//             input_reg[i + 20]++;
//         }
//         else
//             input_reg[i + 20] = 0;
//     }
// }

void app_init()
{
#if APPLICATION_MODE_GPIO
    // IO_GPIO_Init();
#elif APPLICATION_MODE_ANALOG
    MX_SPI2_Init();
#endif
    
    app_setting_load(&app_setting);
    app_setup_serial(BSP_DEBUG_COM_PORT, &app_setting.com);
    app_setup_serial(BSP_RS485_COM_PORT, &app_setting.rs485);
    app_setup_serial(BSP_RF_COM_PORT, &app_setting.rf);
    __enable_irq();
    bsp_delay(100);
    bsp_init();
    bsp_delay(500);
    // bsp_timer_set_handle(TIMER2, app_timer_callback);
    // bsp_timer_start(TIMER2);
    log_info(TAG, "Version : %d.%d.%d.%d", (uint8_t)(APP_VERSION >> 24), (uint8_t)(APP_VERSION >> 16), (uint8_t)(APP_VERSION >> 8), (uint8_t)APP_VERSION);
    uint8_t address = 0;
    input_reg[21] = address;
    app_setting_printf(&app_setting);
    shell_app_init();
    address = app_setting.id;
    if (app_setting.app_mode == Parency_Transfer)
        rs485_to_rf_init();
    else
        for (int i = 0; i < N_MODBUS; i++)
        {
            eMBInit(&modbus[i], MB_RTU, address, i + 1, 115200, MB_PAR_NONE);
            eMBEnable(&modbus[i]);
        }
    rf_app_init();
#if APPLICATION_MODE_ANALOG
    analogs_init();
    SPI_NSS_Init();
#elif APPLICATION_MODE_GPIO
    input_app_init();
    output_app_init();
#endif
#if BSP_BUTTON_EN
    button_init();
#endif
}

void app_poll()
{
    static uint32_t time_stamp = 0;
    static uint32_t time_stamp_rf = 0;
    bsp_iwdg_refresh();
    // shell_app_poll();
    rf_app_poll();
    user_app();
    if (bsp_get_tick() - time_stamp_rf > 20){
        if (app_setting.app_mode == Parency_Transfer)
        {
            if (!rf_app_busy())
            {
                rs485_to_rf_poll();
            }
        }
        else if (app_setting.app_mode == Parency_Modbus)
        {
            if (!rf_app_busy())
            {
                for (int i = 0; i < N_MODBUS; i++)
                {
                    eMBPoll(&modbus[i]);
                    mb_uart_rx_task(&modbus[i]);
                }
            }
            user_mb_app_poll();
        }
        time_stamp_rf = bsp_get_tick();
    }
    if (bsp_get_tick() - time_stamp > 10)
    {

#if APPLICATION_MODE_ANALOG
        analogs_poll(10);
#elif APPLICATION_MODE_GPIO
        input_app_poll(10);
        output_app_poll(10);
#endif
#if BSP_BUTTON_EN
        button_poll(10);
#endif
        time_stamp = bsp_get_tick();
    }
}