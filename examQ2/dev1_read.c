#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int main()
{
    char Ubuff[50];
    char Kbuff[]="this data is written from user to kernel";
    int fd;
    fd = open("/dev/Dev_1",O_RDWR);
    if(fd<0)
    {
        perror("device is not accessible\n ");
        return(-1);
    }
    printf("device accessed successfully from user space\n");
    read(fd,Ubuff,sizeof(Ubuff));
    printf("reading data in user buff through kernel\n ");
    printf("Data read ---%s\n",Ubuff);
    

    return close(fd);

}