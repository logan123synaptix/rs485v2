#include "cli_shell.h"
#include "board.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define SHELL_TX_TIME_OUT_mS 10

#define SHELL_USB_CH 0

TaskHandle_t shell_tx_task = NULL;
TaskHandle_t shell_rx_task = NULL;

QueueHandle_t shell_queue;
uint32_t shell_tx_timeout = 0;

ShellContext_t shell;

static int shell_send_char(void *arg,char c){
    (void) arg;
   bsp_usb_transmit(SHELL_USB_CH,&c,1);
   vTaskDelay(10);
   return 0;
}
static int shell_send_str(void *arg,char *c){
    (void) arg;
   bsp_usb_transmit(SHELL_USB_CH,c,strlen(c));
   vTaskDelay(10);
   return 0;
}


static void shell_receive_task(void *arg){
    char c = 0;
    while(1){
        uint32_t rx_len = bsp_usb_available(SHELL_USB_CH);
        if(rx_len > 0){
            for(uint32_t i = 0;i<rx_len;i++){
                bsp_usb_receiver(SHELL_USB_CH,(uint8_t*)&c,1);
                cli_shell_receive_char(&shell,c);
            }
        }
        vTaskDelay(1);
    }
}


// sCliShellImpl_t shell_impl = {.arg = NULL,.send_char = shell_send_char};
void shell_app_init();
void shell_app_poll();
void shell_app_init(){
    shell.impl.arg = NULL;
    shell.impl.send_char = shell_send_char;
    shell.impl.send_str = shell_send_str;
    cli_shell_boot(&shell);
    xTaskCreate(shell_receive_task,"shell-rx",1024,NULL,configMAX_PRIORITIES-1,&shell_rx_task);
}

void shell_app_poll(){
    char c = 0;
    if(bsp_com_read(BSP_DEBUG_COM_PORT,(uint8_t*)&c,1) == 1){
        cli_shell_receive_char(&shell,c);
    }
}