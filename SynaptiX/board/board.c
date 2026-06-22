#include "board.h"
#include "cqueue.h"
#include "stdbool.h"
#include "logger.h"
#include <string.h>
#include "button.h"

static const char *TAG = "BSP";

#define MAX_UART_BUFF_SIZE 256


UART_HandleTypeDef *bsp_uart[BSP_UART_NUM] = {&hlpuart1,&huart1,&huart2}; 

static CQueue_t uart_queue[BSP_UART_NUM];
static uint8_t uart_buff[BSP_UART_NUM][MAX_UART_BUFF_SIZE];
static bool uart_tranfer_done[BSP_UART_NUM] = {false};
static uint8_t uart_data[BSP_UART_NUM];
static uint8_t uart_data_dma[BSP_UART_NUM][MAX_UART_BUFF_SIZE];
static timer_handle tim_handle[2] = {NULL};
static uint32_t uart_tick[BSP_UART_NUM] = {0};

#if USB_ENABLE

#include "usb_device.h"
#include "usbd_cdc_acm.h"
#include "usbd_cdc_acm_if.h"

CQueue_t usb_rx_queue[BSP_USB_NUM];
uint8_t usb_rx_buff[BSP_USB_NUM][MAX_UART_BUFF_SIZE];

void bsp_usb_init(){
    for(int i = 0;i<BSP_USB_NUM;i++)
        cqueue_init_static(&usb_rx_queue[i],usb_rx_buff[i],MAX_UART_BUFF_SIZE,sizeof(uint8_t));
}
int bsp_usb_transmit(int ch,uint8_t *buff,uint32_t len){
    return (int) CDC_Transmit(ch,buff,len);
}
int bsp_usb_receiver(int ch,uint8_t *buff,uint32_t len){
    if( 0 > ch || ch >= BSP_USB_NUM) return -1;
    int i = 0;
    for(i = 0;i<len;i++)
        if(false == cqueue_receive(&usb_rx_queue[ch],&buff[i])){
            break;
        }
    return i;
}
int bsp_usb_available(int ch){
    if( 0 > ch || ch >= BSP_USB_NUM) return -1;
    return usb_rx_queue[ch].count;
}
int bsp_usb_rx_callback(int ch,uint8_t *buff,uint32_t len){
    if( 0 > ch || ch >= BSP_USB_NUM) return -1;
    int i = 0;
    for(i = 0;i<len;i++)
        if(false == cqueue_send(&usb_rx_queue[ch],&buff[i])){
            break;
        }
    return i;  
}

#endif

#define BSP_UART_TIMEOUT_READY 2
void bsp_timer_set_handle(int timer, timer_handle handle)
{
    if(0  > timer || timer >= 2) return;
    tim_handle[timer] = handle;
}

#if BSP_IO_ENABLE && BSP_OUTPUT_NUM > 0 && BSP_INPUT_NUM > 0

uint8_t bsp_get_input(int input_num)
{
    if(input_num == 0) return (uint8_t) HAL_GPIO_ReadPin(BSP_INPUT0_PORT,BSP_INPUT0_Pin);
    else if(input_num == 1) return (uint8_t) HAL_GPIO_ReadPin(BSP_INPUT1_PORT,BSP_INPUT1_Pin);
    else if(input_num == 2) return (uint8_t) HAL_GPIO_ReadPin(BSP_INPUT2_PORT,BSP_INPUT2_Pin);
    else if(input_num == 3) return (uint8_t) HAL_GPIO_ReadPin(BSP_INPUT3_PORT,BSP_INPUT3_Pin);
    return 0;
}

uint8_t bsp_output_on(int output_num)
{
    if(output_num == 0) HAL_GPIO_WritePin(BSP_OUTPUT0_PORT,BSP_OUTPUT0_Pin,GPIO_PIN_RESET);
    else if(output_num == 1) HAL_GPIO_WritePin(BSP_OUTPUT1_PORT,BSP_OUTPUT1_Pin,GPIO_PIN_RESET);
    else if(output_num == 2) HAL_GPIO_WritePin(BSP_OUTPUT2_PORT,BSP_OUTPUT2_Pin,GPIO_PIN_RESET);
    else if(output_num == 3) HAL_GPIO_WritePin(BSP_OUTPUT3_PORT,BSP_OUTPUT3_Pin,GPIO_PIN_RESET);
    return 0;
}

uint8_t bsp_output_off(int output_num)
{
    if(output_num == 0) HAL_GPIO_WritePin(BSP_OUTPUT0_PORT,BSP_OUTPUT0_Pin,GPIO_PIN_SET);
    else if(output_num == 1) HAL_GPIO_WritePin(BSP_OUTPUT1_PORT,BSP_OUTPUT1_Pin,GPIO_PIN_SET);
    else if(output_num == 2) HAL_GPIO_WritePin(BSP_OUTPUT2_PORT,BSP_OUTPUT2_Pin,GPIO_PIN_SET);
    else if(output_num == 3) HAL_GPIO_WritePin(BSP_OUTPUT3_PORT,BSP_OUTPUT3_Pin,GPIO_PIN_SET);
    return 0;
}

uint8_t bsp_output_toggle(int output_num)
{
    if(output_num == 0) HAL_GPIO_TogglePin(BSP_OUTPUT0_PORT,BSP_OUTPUT0_Pin);
    else if(output_num == 1) HAL_GPIO_TogglePin(BSP_OUTPUT1_PORT,BSP_OUTPUT1_Pin);
    else if(output_num == 2) HAL_GPIO_TogglePin(BSP_OUTPUT2_PORT,BSP_OUTPUT2_Pin);
    else if(output_num == 3) HAL_GPIO_TogglePin(BSP_OUTPUT3_PORT,BSP_OUTPUT3_Pin);
    return 0;
}
#endif
uint32_t bsp_com_write(int com_num, uint8_t *buff, uint32_t len)
{
    switch (com_num)
    {
    case BSP_DEBUG_COM_PORT:
        /* code */
        uart_tranfer_done[BSP_DEBUG_COM_PORT] = false;
        HAL_UART_Transmit(bsp_uart[BSP_DEBUG_COM_PORT],buff,len,HAL_MAX_DELAY);
        // while(uart_tranfer_done[0] == false);
        return len;
        break;
    case BSP_RS485_COM_PORT:
        uart_tranfer_done[BSP_RS485_COM_PORT] = false;
        HAL_UART_Transmit(bsp_uart[BSP_RS485_COM_PORT],buff,len,HAL_MAX_DELAY);
        // while(uart_tranfer_done[1] == false);
        return len;
        break; 
    case BSP_RF_COM_PORT:
        uart_tranfer_done[BSP_RF_COM_PORT] = false;
        log_debug(TAG,"RF Transmit %ld bytes",len);
        HAL_UART_Transmit(bsp_uart[BSP_RF_COM_PORT],buff,len,HAL_MAX_DELAY);
        // while(uart_tranfer_done[2] == false);
        return len;
        break;
    default:
        break;
    }
    return 0;
}

uint32_t bsp_com_write_it(int com, uint8_t *buff, uint32_t len)
{
    switch (com)
    {
    case BSP_DEBUG_COM_PORT:
        /* code */
        uart_tranfer_done[0] = false;
        HAL_UART_Transmit_IT(bsp_uart[BSP_DEBUG_COM_PORT],buff,len);
        // while(uart_tranfer_done[0] == false);
        return len;
        break;
    case BSP_RS485_COM_PORT:
        uart_tranfer_done[1] = false;
        HAL_UART_Transmit_IT(bsp_uart[BSP_RS485_COM_PORT],buff,len);
        // while(uart_tranfer_done[1] == false);
        return len;
        break; 
    case BSP_RF_COM_PORT:
        uart_tranfer_done[2] = false;
        log_debug(TAG,"RF Transmit IT %ld bytes",len);
        log_print_hex(LOGGER_DEBUG,TAG,buff,len);
        HAL_UART_Transmit_IT(bsp_uart[BSP_RF_COM_PORT],buff,len);
        // while(uart_tranfer_done[2] == false);
        return len;
        break;
    default:
        break;
    }
    return 0;
}

typedef void (*com_tx_cb)(void *arg);
typedef struct BSP_COM_Tx_Callback
{
    /* data */
    com_tx_cb cb;
    void *arg;
}BSP_COM_Tx_Callback_t;

BSP_COM_Tx_Callback_t com_tx_callback[3];

void bsp_com_set_tx_callback(int com, void (*callback)(void *arg), void *arg)
{
    if(com < 0 || com > 3) return;
    com_tx_callback[com].cb = callback;
    com_tx_callback[com].arg = arg;
}

uint32_t bsp_com_read(int com_num, uint8_t *buff, uint32_t len)
{
    if(com_num < 0 || com_num >= BSP_UART_NUM)
        return 0;
    uint32_t length = 0;
    while(length < len){
        if(false == cqueue_receive(&uart_queue[com_num],&buff[length])) break;
        length++;
    }
    return length;
}

uint32_t bsp_com_available(int com_num)
{
    if(com_num < 0 || com_num >= BSP_UART_NUM)
        return 0;
    
    return uart_queue[com_num].count;
}

bool bsp_com_ready(int com)
{
    if(com < 0 || com >= BSP_UART_NUM)
        return false;
    if(uart_tick[com] > 0) uart_tick[com]--;
    return uart_tick[com] == 0 ? true : false;
}

void bsp_com_flush(int com)
{
    if(com < 0 || com >= BSP_UART_NUM)
        return;
    uart_queue[com].head = 0;
    uart_queue[com].tail = 0;
    uart_queue[com].count = 0;
}

uint32_t bsp_com_init()
{      
    #if BSP_RS485_RF_TRANSFER_MODE
    // HAL_UART_Receive_IT(&huart2,&uart_data[BSP_RS485_COM_PORT],1);
    // HAL_UART_Receive_IT(&huart3,&uart_data[BSP_RF_COM_PORT],1);
    // HAL_UARTEx_ReceiveToIdle_DMA(&huart1,uart_data_dma[0],MAX_UART_BUFF_SIZE);
    // HAL_UARTEx_ReceiveToIdle_IT(&huart2,uart_data_dma[1],MAX_UART_BUFF_SIZE);
    #else
    
    // HAL_UART_Receive_IT(&huart3,&uart_data[BSP_RF_COM_PORT],1);
    #endif
    for(int i = 0;i<BSP_UART_NUM;i++){
        if(i = BSP_RF_COM_PORT){
            HAL_UARTEx_ReceiveToIdle_DMA(bsp_uart[i],uart_data_dma[BSP_RF_COM_PORT],MAX_UART_BUFF_SIZE);
        }
        else
            HAL_UART_Receive_IT(bsp_uart[i],&uart_data[i],1);
        cqueue_init_static(&uart_queue[i],uart_buff[i],MAX_UART_BUFF_SIZE,sizeof(uint8_t));
    }
    return 0;
}

uint8_t bsp_get_address()
{
    return 1;
}

static void log_puts(const char *s){
    bsp_com_write(0,(uint8_t*)s,strlen(s));
}

void bsp_init()
{
    tim_handle[0] = NULL;
    tim_handle[1] = NULL;
    bsp_usb_init();
    bsp_com_init();
    HAL_UART_Transmit(bsp_uart[BSP_DEBUG_COM_PORT],(uint8_t*)"System Init\r\n",strlen("System Init\r\n"),HAL_MAX_DELAY);
    bsp_led_net_off();
    bsp_led_status_off();
    logger_init(BSP_LOG_LEVEL,log_puts);
    log_debug(TAG,"Modbus RS-485 address : 0x%02X",bsp_get_address());
    button_init();
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *husart){
    for (int i = 0; i < BSP_UART_NUM; i++)
        if (bsp_uart[i] == husart)
        {
            if (com_tx_callback[i].cb != NULL)
                com_tx_callback[i].cb(com_tx_callback[i].arg);
            break;
        }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < BSP_UART_NUM; i++)
    {
        if (bsp_uart[i] == huart)
        {
            cqueue_send(&uart_queue[i], &uart_data[i]);
            uart_tick[i] = BSP_UART_TIMEOUT_READY;
            HAL_UART_Receive_IT(bsp_uart[i],&uart_data[i],1);
            break;
        }
    }
}
#if BSP_RS485_RF_TRANSFER_MODE
#include "zigbee.h"
#include "app_settings.h"

extern ZigbeeMesh_t zigbee;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    if(huart == bsp_uart[BSP_RS485_COM_PORT]){
        log_debug(TAG,"RS485->RF : %ld bytes",Size);
        HAL_UART_Transmit(bsp_uart[BSP_RF_COM_PORT],uart_data_dma[BSP_RF_COM_PORT],Size,HAL_MAX_DELAY);
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1,uart_data_dma[BSP_RS485_COM_PORT],MAX_UART_BUFF_SIZE);
    }
    else if(huart == bsp_uart[BSP_RF_COM_PORT]){
        if(app_setting.app_mode == Parency_Transfer && zigbee.event.event == ZB_EVENT_NONE) {
            log_debug(TAG,"RF->RS485 : %ld bytes",Size);
            bsp_de_on();
            HAL_UART_Transmit(&huart2,uart_data_dma[BSP_RF_COM_PORT],Size,HAL_MAX_DELAY);
            bsp_de_off();
        }
        for(uint16_t i = 0;i<Size;i++)
            cqueue_send(&uart_queue[BSP_RF_COM_PORT],&uart_data_dma[BSP_RF_COM_PORT][i]);
        HAL_UARTEx_ReceiveToIdle_IT(bsp_uart[BSP_RF_COM_PORT],uart_data_dma[BSP_RF_COM_PORT],MAX_UART_BUFF_SIZE);
    }
}
#endif

void BSP_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim == &htim2){
        if(tim_handle[0] != NULL) tim_handle[0]();
    }
    if(htim == &htim3){
        if(tim_handle[1] != NULL) tim_handle[1]();
    }
}