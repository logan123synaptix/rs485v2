/*
 * mcp320x.h
 *
 *  Created on: Jun 6, 2025
 *      Author: phamminhnhan
 */

#ifndef COMPONENTS_MCP320X_MCP320X_H_
#define COMPONENTS_MCP320X_MCP320X_H_


#include "board.h"
#include <stdint.h>

#if BSP_ANALOG_ENABLE

#define VOLTAGE_REFERENCE_mV 4096

typedef int (*mcp_transmit)(uint8_t *data,uint32_t length);
typedef int (*mcp_receive)(uint8_t *data,uint32_t length);
typedef int (*mcp_transmit_receive)(uint8_t *source,uint8_t *dest,uint32_t length);

typedef struct MCP320x_Driver{
    mcp_transmit transmit;
    mcp_receive receive;
    mcp_transmit_receive transmit_receive;
}MCP320x_Driver_t;

typedef struct MCP320x{
	MCP320x_Driver_t *driver;
	uint8_t num_channel;
	uint16_t *adc_value;
	uint16_t vref;
}MCP320x_t;

void MCP320x_init(MCP320x_t *mcp,MCP320x_Driver_t *driver,uint16_t *buff_addc,uint8_t num_channle);
uint16_t MCP320x_read(MCP320x_t *mcp,uint8_t channel);
uint16_t MCP320x_convert_mili_voltage(MCP320x_t *mcp,uint8_t channel);
#endif
#endif /* COMPONENTS_MCP320X_MCP320X_H_ */