#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <fcntl.h>

// Just hardcode the addresses of the two GPIO registers we need.  
// Could clearly build a nice headerfile for getting to 
// all gpio banks and registers.

// program uses: gpio4_25

static const uint32_t k_gpio4ModuleAddr = 0x48059000;
static const uint32_t k_gpioModuleSize = 408;

static const uint32_t k_clearDataOutOffset = 0x0190 / 4;
static const uint32_t k_setDataOutOffset = 0x0194 / 4;

static const uint32_t k_gpioPin25 = (0x1 << 25);

// Also need to hardcode the address of the CTRL_CORE_PAD register

static const uint32_t k_ctrlCorePadAddr = 0x4A002000;
static const uint32_t k_ctrlCorePadSize = 0x1888;;

static const uint32_t k_gpio4_24Offset = 0x15AC / 4;

static const uint32_t k_mode14 = 0x000E;

int main(int argc, char* argv[])
{
    int memoryhandle;

    volatile unsigned int *gpio4Module = NULL;
    volatile unsigned int *clearDataOut = NULL;
    volatile unsigned int *setDataOut = NULL;

    volatile unsigned int *ctrlCorePad = NULL;
    volatile unsigned int *ctrlCorePadGpio4_24 = NULL;

    memoryhandle = open("/dev/mem", O_RDWR);
    
    if (memoryhandle < 0)
    {
        printf("Could not open /dev/mem\n");
        exit(1);
    }

    //  Memory map the GPIO4 registers
    gpio4Module = mmap(0, k_gpioModuleSize, 
        PROT_WRITE, MAP_SHARED, 
        memoryhandle, k_gpio4ModuleAddr);

    clearDataOut = gpio4Module + k_clearDataOutOffset;
    setDataOut = gpio4Module + k_setDataOutOffset;

    if (gpio4Module == MAP_FAILED)
    {
        printf("Could not memory map the gpio clear and set data registers\n");
        exit(1);
    }
    //
    //  Memory map the GPIO3 registers

    ctrlCorePad = mmap(0, k_ctrlCorePadSize, 
        PROT_WRITE, MAP_SHARED, 
        memoryhandle, k_ctrlCorePadAddr);

    ctrlCorePadGpio4_24 = ctrlCorePad + k_gpio4_24Offset;

    if (ctrlCorePad == MAP_FAILED)
    {
        printf("Could not memory map the ctrl_core_pad gpio4_24 register\n");
        exit(1);
    }

    //	First, DON'T set the mux mode to show it will not blink

    uint32_t defaultMuxMode = (*ctrlCorePadGpio4_24);
    printf("Blink without setting muxmode\n");

    for (int i=0; i<5; i++)
    {
        (*setDataOut) = k_gpioPin25;
        usleep(500000);

        (*clearDataOut) = k_gpioPin25;

        usleep(500000);
    }

    //	set the mux mode for pad to gpio4_24

    printf("Blink after setting muxmode\n");
    (*ctrlCorePadGpio4_24) = k_mode14;

    for (int i=0; i<5; i++)
    {
        (*setDataOut) = k_gpioPin25;
        usleep(500000);

        (*clearDataOut) = k_gpioPin25;

        usleep(500000);
    } 

    //  finally, reset the muxmode

    (*ctrlCorePadGpio4_24) = defaultMuxMode;
    printf("reset muxmode\n");

    for (int i=0; i<5; i++)
    {
        (*setDataOut) = k_gpioPin25;
        usleep(500000);

        (*clearDataOut) = k_gpioPin25;

        usleep(500000);
    }
}

