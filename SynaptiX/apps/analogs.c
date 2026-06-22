#include "analogs.h"

#if APPLICATION_MODE_ANALOG
#include "user_mb_app.h"
#include "board.h"
#if ANALOGS_DEBUG
#include "logger.h"
static const char *TAG = "Analogs";
#endif
MCP320x_t mcp;
// uint16_t adc_value[BSP_NUM_ADC_CHANNEL];
static int bsp_transmit_receive(uint8_t *source,uint8_t *dest,uint32_t len){
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
    int res = HAL_SPI_TransmitReceive(&hspi2,source,dest,len,HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
    return res;
}
 
#define R_Sense 1 // 1 Ohm
#define GAIN 200
static uint16_t convert_current(uint16_t mV){
	uint16_t current = (mV * 100) / (R_Sense * GAIN); // I = U/R 
}

MCP320x_Driver_t driver = {.receive = NULL,.transmit = NULL,.transmit_receive = bsp_transmit_receive};

void analogs_init(){
    MCP320x_init(&mcp,&driver,input_reg,BSP_NUM_ADC_CHANNEL);
}
void analogs_poll(uint32_t time_stamp){
	static uint8_t i = 0;
	static uint32_t tick = 0;
	tick+=time_stamp;
	uint16_t voltage[BSP_NUM_ADC_CHANNEL];
	uint16_t current[BSP_NUM_ADC_CHANNEL];
	if (tick % 500 == 0) {
		for (i = 0; i < BSP_NUM_ADC_CHANNEL; i++) {
			MCP320x_read(&mcp, i);
			voltage[i] = mcp.adc_value[i];
			current[i] = (voltage[i] * 100) / 200;
		}
		#if ANALOGS_DEBUG
		log_debug(TAG, "%02d.%02d,%02d.%02d,%02d.%02d,%02d.%02d,%02d.%02d mA", current[0] / 100, current[0] % 100,
														   current[1] / 100, current[1] % 100,
														   current[2] / 100, current[2] % 100,
														   current[3] / 100, current[3] % 100,
														   current[4] / 100, current[4] % 100);
		log_debug(TAG,"%d,%d,%d%d,%d,%d",voltage[0],voltage[1],voltage[2],voltage[3],voltage[4]);
		#endif
	}
}


#endif