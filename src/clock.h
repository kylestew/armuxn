#ifndef __CLOCK_H
#define __CLOCK_H

#include "stdint.h"

void msleep(uint32_t delay);
uint32_t mtime(void);
void clock_setup(void);

#endif
