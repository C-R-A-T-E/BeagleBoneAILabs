#include <stdint.h>
#include <stdio.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"

static uint32_t *k_gpio3ClearDataOut = (uint32_t *)0x48057190;
static uint32_t *k_gpio3SetDataOut = (uint32_t *)0x48057194;

static const uint32_t k_gpioPin12 = (0x1 << 12);

void main(void)
{
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0x0;

    while (1) 
    {
        (*k_gpio3SetDataOut) = k_gpioPin12;
        __delay_cycles(100000000); /* 500ms @ 200MHz */


        (*k_gpio3ClearDataOut) = k_gpioPin12;
        __delay_cycles(100000000); /* 500ms @ 200MHz */
    }

    (*k_gpio3ClearDataOut) = k_gpioPin12;
	__halt();
}
