#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int main()
{
   
    char Kbuff[]="this data is written from user to kernel";
    int fd;
    fd = open("/dev/Dev_ioctl",O_RDWR);
    if(fd<0)
    {
        perror("device is not accessible\n ");
        return(-1);
    }
    
    
    write(fd,Kbuff,sizeof(Kbuff));
    printf(" writing from user---%s \n ",Kbuff);

    return close(fd);

}