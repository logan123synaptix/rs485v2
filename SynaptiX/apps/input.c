#include "input.h"
#include "board.h"
#include <stdio.h>
#include "user_mb_app.h"
#define MAX_TIME_DEBOUNCE 50
#if BSP_IO_ENABLE
void input_init(struct Input *input,enum InputEvent *event, uint16_t *counter,struct InputDriver *driver)
{
    input->event = event;
    input->counter = counter;
    input->driver = driver;
    *(input->event) = Input_EVENT_RELEASE;
    *(input->counter) = 0;
    input->tick = 0;
}

void input_poll(struct Input *input, uint32_t timestamp)
{
    if(input->driver->read(input->input_num)){
        // *input->event = 1;
        input_coils[0] |= (0x01 << input->input_num);
        if(input->tick == 0) *input->counter += 1;
        input->tick = MAX_TIME_DEBOUNCE;
    }else{
        // *input->event = 0;
        input_coils[0] &= ~(0x01 << input->input_num);
        if(input->tick > 0){
            input->tick-= timestamp;
        }
    }
    if(output_coils[1] & (0x01 << input->input_num)){
        *(input->counter) = 0;
        output_coils[1] &= ~(0x01 << input->input_num);
    }
}

#include "user_mb_app.h"

Input_t input[BSP_INPUT_NUM];
static struct InputDriver driver = {
        .read = bsp_get_input
    };

void input_app_init(){  
    for(int i = 0;i < BSP_INPUT_NUM;i++){
        input_init(&input[i],input_coils,input_reg+i+16,&driver);
        input[i].input_num = i;
    }
}
void input_app_poll(uint32_t timestamp){
    for(int i = 0;i < BSP_INPUT_NUM;i++){
        input_poll(&input[i],timestamp);
    }
}
#endif