#ifndef OUTPUT_H
#define OUTPUT_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "app_config.h"

#if BSP_IO_ENABLE
#define OUTPUT_OFF 0
#define OUTPUT_ON 1
#define OUTPUT_TOGGLE 2
#define OUTPUT_TIME_TOGGLE_mS 1000
typedef struct Output
{
    /* data */
    uint16_t *state;
    bool isOn;
    struct OutputDriver
    {
        uint8_t (*on)(int output_num);
        uint8_t (*off)(int output_num);
        uint8_t (*toggle)(int output_num);
    } *driver;
    uint32_t tick;
    int output_num;
}Output_t;

void output_init(Output_t *output, uint16_t *state, struct OutputDriver *driver);
void output_poll(Output_t *output, uint32_t timestamp);

void output_app_init();
void output_app_poll(uint32_t timestamp);
#endif
#ifdef __cplusplus
}
#endif

#endif // OUTPUT_H