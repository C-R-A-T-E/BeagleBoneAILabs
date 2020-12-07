#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <fcntl.h>

// Just hardcode the addresses of the two registers we need.  
// Could clearly build a nice headerfile for getting to 
// all gpio banks and registers.

static const uint32_t k_gpio3ModuleAddr = 0x48057000;
static const uint32_t k_gpioModuleSize = 408;

static const uint32_t k_clearDataOutOffset = 0x190 / 4;
static const uint32_t k_setDataOutOffset = 0x194 / 4;

static const uint32_t k_gpioPin12 = (0x1 << 12);

int main(int argc, char* argv[])
{
    int memoryhandle;

    volatile unsigned int *gpio3Module = NULL;
    volatile unsigned int *clearDataOut = NULL;
    volatile unsigned int *setDataOut = NULL;

	memoryhandle = open("/dev/mem", O_RDWR);
    
    if (memoryhandle < 0)
    {
        printf("Could not open /dev/mem\n");
        exit(1);
    }

    //  Memory map the GPIO3 registers

	gpio3Module = mmap(0, k_gpioModuleSize, 
        PROT_WRITE, MAP_SHARED, 
        memoryhandle, k_gpio3ModuleAddr);

    clearDataOut = gpio3Module + k_clearDataOutOffset;
    setDataOut = gpio3Module + k_setDataOutOffset;

	if (clearDataOut == MAP_FAILED || setDataOut == MAP_FAILED)
    {
		printf("Could not memory map the gpio clear and set data registers\n");
		exit(1);
	}

    for (int i=0; i<5; i++)
    {
        (*setDataOut) = k_gpioPin12;
        usleep(500000);

        (*clearDataOut) = k_gpioPin12;

        usleep(500000);
    } 
}

