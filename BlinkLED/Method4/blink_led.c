#include <stdint.h>
#include <stdio.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

void main(void)
{
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0x0;

    uint32_t pr1_pru1_gpo17 = 0x1 << 17;
    uint32_t pr1_pru1_gpo18 = 0x1 << 18;

    while (1) 
    {
        __R30 |= pr1_pru1_gpo17;
        __R30 &= ~(pr1_pru1_gpo18);

        __delay_cycles(100000000); /* 500ms @ 200MHz */

        __R30 |= pr1_pru1_gpo18;
        __R30 &= ~(pr1_pru1_gpo17);

        __delay_cycles(100000000); /* 500ms @ 200MHz */
    }
}
