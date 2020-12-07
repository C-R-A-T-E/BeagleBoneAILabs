#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

static const char k_exportFilename[] = "/sys/class/gpio/export"; 
static const char k_gpioDirectoryname[] = "/sys/class/gpio/gpio76";
static const char k_gpioValueFilename[] = "/sys/class/gpio/gpio76/value";
static const char k_gpioDirectionFilename[] = "/sys/class/gpio/gpio76/direction";

static const int k_gpioNum = 76;

int main(int argc, char* argv[])
{
    //
    // Make sure we can access /sys/class/gpio76/
    //

    DIR* gpioDirectoryhandle = opendir(k_gpioDirectoryname);

    if(gpioDirectoryhandle == NULL)
    {
        closedir(gpioDirectoryhandle);

        printf("gpio76 device doesn't exist, using export to create\n");
        
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

        fprintf(exportFilehandle, "%d", k_gpioNum);
        fclose(exportFilehandle);
    
        //  Check again, we should now have the gpio directory we need.

        gpioDirectoryhandle = opendir(k_gpioDirectoryname);

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

    FILE* gpioDirectionFilehandle = fopen(k_gpioDirectionFilename, "r");

    if (gpioDirectionFilehandle == NULL)
    {
        printf("could not open gpio direction control file\n");
        exit(1);
    }

    char direction[32];
    fgets(direction, sizeof(direction) -1, gpioDirectionFilehandle);
    fclose(gpioDirectionFilehandle);

    if (strncmp("out", direction, 3) != 0)
    {
        printf("Direction of gpio76 was not 'out', will change\n");

        gpioDirectionFilehandle = fopen(k_gpioDirectionFilename, "w");

        if (gpioDirectionFilehandle == NULL)
        {
            printf("Could not open direction file for write\n");
            exit(1);
        }

        fputs("out", gpioDirectionFilehandle);
        fclose(gpioDirectionFilehandle);

        gpioDirectionFilehandle = fopen(k_gpioDirectionFilename, "r");
        fgets(direction, sizeof(direction) -1, gpioDirectionFilehandle);
        printf("direction = %s\n", direction);
        fclose(gpioDirectionFilehandle);
    }

    FILE* gpioValueFilehandle = fopen(k_gpioValueFilename, "w");

    if (gpioValueFilehandle == NULL)
    {
        printf("could not open gpio value control files\n");
        exit(1);
    }
    
    fseek(gpioValueFilehandle, 0, SEEK_SET);
    fputs("0", gpioValueFilehandle);
    fflush(gpioValueFilehandle);

    for (int i=0; i<10; i++)
    {
        fputs("1", gpioValueFilehandle);
        fflush(gpioValueFilehandle);

        usleep(500000);

        fputs("0", gpioValueFilehandle);
        fflush(gpioValueFilehandle);

        usleep(500000);
    } 
}

