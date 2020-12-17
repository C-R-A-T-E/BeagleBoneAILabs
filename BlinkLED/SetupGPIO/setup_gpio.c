#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

static const char k_exportFilename[] = "/sys/class/gpio/export"; 
static const char k_gpio76Directoryname[] = "/sys/class/gpio/gpio76";
static const char k_gpio76DirectionFilename[] = "/sys/class/gpio/gpio76/direction";
static const char k_gpio121Directoryname[] = "/sys/class/gpio/gpio121";
static const char k_gpio121DirectionFilename[] = "/sys/class/gpio/gpio121/direction";

static const int k_gpioNum76 = 76;
static const int k_gpioNum121 = 121;

int main(int argc, char* argv[])
{
    if(access(k_exportFilename, F_OK) < 0) 
    {
        printf("gpio export file (%s) doesn't exist\n", k_exportFilename);
        exit(1);
    }
        
    FILE* exportFilehandle = fopen (k_exportFilename, "w");

    if (exportFilehandle == NULL)
    {
        printf("Could not open %s\n", k_exportFilename);
        exit(1);
    }

    //
    // Make sure we can access /sys/class/gpio76/
    //

    DIR* gpioDirectoryhandle = opendir(k_gpio76Directoryname);

    if(gpioDirectoryhandle == NULL)
    {
        closedir(gpioDirectoryhandle);

        printf("gpio76 device doesn't exist, using export to create\n");
        
	fseek(exportFilehandle, 0, SEEK_SET);
        fprintf(exportFilehandle, "%d", k_gpioNum76);
	fflush(exportFilehandle);
    
        //  Check again, we should now have the gpio directory we need.

        gpioDirectoryhandle = opendir(k_gpio76Directoryname);

        if(gpioDirectoryhandle == NULL)
        {
            closedir(gpioDirectoryhandle);
            printf("Could not create gpio76 device\n");
            exit(1);
        } 

        closedir(gpioDirectoryhandle);
        usleep(500000);
    }

    //
    // Make sure we can access 'value' and 'direction' and make sure direction
    // is correclty set to 'out'
    //

    FILE* gpio76DirectionFilehandle = fopen(k_gpio76DirectionFilename, "r");

    if (gpio76DirectionFilehandle == NULL)
    {
        printf("could not open gpio direction control file\n");
        exit(1);
    }

    char direction[32];
    fgets(direction, sizeof(direction) -1, gpio76DirectionFilehandle);
    fclose(gpio76DirectionFilehandle);

    if (strncmp("out", direction, 3) != 0)
    {
        printf("Direction of gpio76 was not 'out', will change\n");

        gpio76DirectionFilehandle = fopen(k_gpio76DirectionFilename, "w");

        if (gpio76DirectionFilehandle == NULL)
        {
            printf("Could not open direction file for write\n");
            exit(1);
        }

        fputs("out", gpio76DirectionFilehandle);
        fclose(gpio76DirectionFilehandle);

        gpio76DirectionFilehandle = fopen(k_gpio76DirectionFilename, "r");
        fgets(direction, sizeof(direction) -1, gpio76DirectionFilehandle);
        printf("direction = %s\n", direction);
        fclose(gpio76DirectionFilehandle);
    }

    printf("/sys/class/gpio/gpio76 (gpio3_12) is setup and ready to use\n");

    //
    // Make sure we can access /sys/class/gpio121/
    //

    gpioDirectoryhandle = opendir(k_gpio121Directoryname);

    if (gpioDirectoryhandle == NULL)
    {
        closedir(gpioDirectoryhandle);

        printf("gpio121 device doesn't exist, using export to create\n");
        
	fseek(exportFilehandle, 0, SEEK_SET);
        fprintf(exportFilehandle, "%d", k_gpioNum121);
	fflush(exportFilehandle);
    
        //  Check again, we should now have the gpio directory we need.

        gpioDirectoryhandle = opendir(k_gpio121Directoryname);

        if(gpioDirectoryhandle == NULL)
        {
            closedir(gpioDirectoryhandle);
            printf("Could not create gpio121 device\n");
            exit(1);
        } 

        closedir(gpioDirectoryhandle);
        usleep(500000);
    }

    //
    // Make sure we can access 'value' and 'direction' and make sure direction
    // is correclty set to 'out'
    //

    FILE* gpio121DirectionFilehandle = fopen(k_gpio121DirectionFilename, "r");

    if (gpio121DirectionFilehandle == NULL)
    {
        printf("could not open gpio direction control file\n");
        exit(1);
    }

    fgets(direction, sizeof(direction) -1, gpio121DirectionFilehandle);
    fclose(gpio121DirectionFilehandle);

    if (strncmp("out", direction, 3) != 0)
    {
        printf("Direction of gpio121 was not 'out', will change\n");

        gpio121DirectionFilehandle = fopen(k_gpio121DirectionFilename, "w");

        if (gpio121DirectionFilehandle == NULL)
        {
            printf("Could not open direction file for write\n");
            exit(1);
        }

        fputs("out", gpio121DirectionFilehandle);
        fclose(gpio121DirectionFilehandle);

        gpio121DirectionFilehandle = fopen(k_gpio121DirectionFilename, "r");
        fgets(direction, sizeof(direction) -1, gpio121DirectionFilehandle);
        printf("direction = %s\n", direction);
        fclose(gpio121DirectionFilehandle);
    }

    printf("/sys/class/gpio/gpio121 (gpio4_25) is setup and ready to use\n");

    fclose(exportFilehandle);
}

