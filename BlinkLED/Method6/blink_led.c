#include <stdint.h>
#include <stdio.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"

// Uses LEDs connected to:
// P8.14 - pr1_pru1_gpo9 NOT default muxmode for ball D5
// P8.16 - pr1_pru1_gpo18 default muxmode ball B4

volatile register uint32_t __R30;
volatile register uint32_t __R31;

void main(void)
{
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0x0;

    uint32_t pr1_pru1_gpo9 = 0x1 << 9;
    uint32_t pr1_pru1_gpo18 = 0x1 << 18;

    uint32_t *ctrl_core_pad_vin2a_d12 = (uint32_t *)0x4A003598;
    uint32_t muxmode_13 = 0x0000000D;
    uint32_t muxmode_default = 0x0000000E;
    uint32_t clear_muxmode = 0xFFFFFFF0;

    int i;

    while (1) 
    {
        for (i=0; i<4; i++)
        {
            __R30 |= pr1_pru1_gpo9;
            __R30 &= ~(pr1_pru1_gpo18);

            __delay_cycles(100000000); /* 500ms @ 200MHz */

            __R30 |= pr1_pru1_gpo18;
            __R30 &= ~(pr1_pru1_gpo9);

            __delay_cycles(100000000); /* 500ms @ 200MHz */
        }

	if (((*ctrl_core_pad_vin2a_d12) & muxmode_13) != muxmode_13)
        {
            (*ctrl_core_pad_vin2a_d12) = ((*ctrl_core_pad_vin2a_d12) & clear_muxmode) | muxmode_13;
        }
        else
        {
            (*ctrl_core_pad_vin2a_d12) = ((*ctrl_core_pad_vin2a_d12) & clear_muxmode) | muxmode_default;
        }
    }
}
