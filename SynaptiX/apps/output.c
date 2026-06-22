#include "output.h"
#include "board.h"

#if BSP_IO_ENABLE
#include "user_mb_app.h"
#include "logger.h" 

static const char *TAG = "Output";

void output_init(Output_t *output, uint16_t *state,struct OutputDriver *driver)
{
    output->state = state;
    output->driver = driver;
    *(output->state) = 0;
    output->tick = 0;
    *output->state = OUTPUT_OFF;
    output->isOn = false;
}

void output_poll(Output_t *output, uint32_t timestamp)
{
    if(output_coils[0] >> output->output_num & 0x01){
        if(!output->isOn){
            output->driver->on(output->output_num);
            *(output->state) = OUTPUT_ON;
            output->isOn = true;
            // log_info(TAG, "Output %d ON", output->output_num);
        }
    }else{
        if(output->isOn){
            output->driver->off(output->output_num);
            *(output->state) = OUTPUT_OFF;
            output->isOn = false;
            // log_info(TAG, "Output %d OFF", output->output_num);
        }
    }
}

Output_t output[BSP_OUTPUT_NUM];
static struct OutputDriver driver = {
        .on = bsp_output_on,
        .off = bsp_output_off,
        .toggle = bsp_output_toggle
};

void output_app_init(){
    memset(output_coils,0,sizeof(output_coils));
    for(int i = 0;i < BSP_OUTPUT_NUM;i++){
        output_init(&output[i],hoding_reg+i,&driver);
        output[i].output_num = i;
        output->driver->off(output[i].output_num);
    }
}
void output_app_poll(uint32_t timestamp){
    for(int i = 0;i < BSP_OUTPUT_NUM;i++){
        output_poll(&output[i],timestamp);
    }
}
#endif