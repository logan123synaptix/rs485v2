#include "rs4852RF.h"

#include "board.h"
#include "logger.h"

static const char *TAG_RF = "RF Receive";
static const char *TAG_RS485 = "RS485 Receive";

#define MAX_UART_BUFF_SIZE 256

uint8_t data_rs485[MAX_UART_BUFF_SIZE];
uint8_t data_rf[MAX_UART_BUFF_SIZE];

void rs485_to_rf_init()
{
    bsp_de_off();
    // HAL_UART_AbortReceive_IT(&huart2);
    // HAL_UART_AbortReceive_IT(&huart3);
    // HAL_UARTEx_ReceiveToIdle_IT(&huart2,data_rs485,MAX_UART_BUFF_SIZE);
    // HAL_UARTEx_ReceiveToIdle_IT(&huart3,data_rf,MAX_UART_BUFF_SIZE);
}

void rs485_to_rf_poll()
{
    static uint32_t time_poll = 0;
    if (bsp_get_tick() - time_poll > 1)
    {
        uint8_t buff[256];
        uint32_t len = bsp_com_available(BSP_RS485_COM_PORT);
        if (len > 0 && bsp_com_ready(BSP_RS485_COM_PORT))
        {
            bsp_com_read(BSP_RS485_COM_PORT, buff, len);
            bsp_com_write(BSP_RF_COM_PORT, buff, len);
        }
        time_poll = bsp_get_tick();
    }
}

// void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
//     if(huart == &huart2){
//         bsp_com_write(BSP_RF_COM_PORT, data_rs485, Size);
//         HAL_UARTEx_ReceiveToIdle_IT(&huart1,data_rs485,MAX_UART_BUFF_SIZE);
//     }
//     else if(huart == &huart3){
//         bsp_de_on();
//         bsp_com_write(BSP_RS485_COM_PORT, data_rf, Size);
//         bsp_de_off();
//         HAL_UARTEx_ReceiveToIdle_IT(&huart1,data_rf,MAX_UART_BUFF_SIZE);
//     }
// }