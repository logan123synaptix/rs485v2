#ifndef ANALOGS_H
#define ANALOGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "app_config.h"

#if APPLICATION_MODE_ANALOG

#include "mcp320x.h"

void analogs_init();
void analogs_poll(uint32_t time_stamp);

#endif

#ifdef __cplusplus
}
#endif

#endif // ANALOGS_H