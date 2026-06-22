#ifndef INPUT_H
#define INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "app_config.h"

#if BSP_IO_ENABLE
typedef struct Input
{
    /* data */
    enum InputEvent{
        Input_EVENT_RELEASE = 0,
        Input_EVENT_PRESS = 1,
        Input_EVENT_CLICK = 2
    } *event;

    uint32_t tick;
    uint16_t *counter;
    uint8_t clear_flag:1;
    int input_num;
    struct InputDriver{
        uint8_t (*read)(int input_num);
    } *driver;
}Input_t;

void input_init(struct Input *input, enum InputEvent *event, uint16_t *counter, struct InputDriver *driver);
void input_poll(struct Input *input, uint32_t timestamp);

void input_app_init();
void input_app_poll(uint32_t timestamp);
#endif
#ifdef __cplusplus
}
#endif  
#endif // INPUT_H