/*
 * mcp320x.c
 *
 *  Created on: Jun 6, 2025
 *      Author: phamminhnhan
 */




#include "mcp320x.h"
#include <assert.h>

#if BSP_ANALOG_ENABLE

#define RESOLUTION_MCP320X 12

void MCP320x_init(MCP320x_t *mcp,MCP320x_Driver_t *driver,uint16_t *buff_addc,uint8_t num_channle){
	assert(driver != NULL);
	assert(buff_addc != NULL);
	assert(num_channle <= 8);
	mcp->driver = driver;
	mcp->adc_value = buff_addc;
	mcp->num_channel = num_channle;
	mcp->vref = VOLTAGE_REFERENCE_mV;
}



uint16_t MCP320x_read(MCP320x_t *mcp,uint8_t channel){
	if(channel > mcp->num_channel) return 0;

	uint8_t tx_buff[3];
	uint8_t rx_buff[3];

	uint8_t addr = ( 0x18 |channel) << 2;

	tx_buff[0] = addr;
	tx_buff[1] = 0;
	tx_buff[2] = 0;
    // if(mcp->driver->transmit_receive == NULL) return -1;
	if(mcp->driver->transmit_receive(tx_buff, rx_buff, 3) != 0){
		return 0;
	}

	mcp->adc_value[channel] = (((uint16_t)rx_buff[1] << 4) | (rx_buff[2] >> 4)) ;
	return mcp->adc_value[channel];
}
uint16_t MCP320x_convert_mili_voltage(MCP320x_t *mcp,uint8_t channel){
	if(channel > mcp->num_channel) return 0;

	uint16_t Vin = (uint16_t) (mcp->adc_value[channel] * mcp->vref) / 4096;

	return Vin;
}
#endif