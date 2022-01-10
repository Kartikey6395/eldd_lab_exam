#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include "ioctl.h"



//structure for ALL operatons accessiblility
struct stats
{
    int size;
    char buff[20];
    int r_w;
};


int main()
{
    int fd,retval;
    struct stats *user_ptr; 
    
    unsigned int GetBaudRate;
    fd = open("/dev/Dev_ioctl",O_RDWR,0777);
    if(fd<0)
    {
        perror("device is not accessible\n");
        return(-1);
    }
    printf("device accessed successfully from user space\n");

    if((retval = ioctl(fd,GETSTATS,user_ptr))<0)
    printf("Struct stats--error ");

    printf("getstats from kernel --\n");
    printf("size of buff %d\n",user_ptr->size);
    printf("data of buff %s\n",user_ptr->buff);
    printf("R_W =  %d\n",user_ptr->r_w);
    

    return close(fd);

}