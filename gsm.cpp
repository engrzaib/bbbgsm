//# dtc -O dtb -o uart1-00A0.dtbo -b 0 -@ uart1.dts
//# cp uart1-00A0.dtbo /lib/firmware/uart1-00A0.dtbo
//# echo uart1 > /sys/devices/bone_capemgr.8/slots
//Modified by ABHISHEK GOWDA

#include <stdio.h> // standard input / output functions
#include <string.h> // string function definitions
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <termios.h> // POSIX terminal control definitionss
#include <time.h>   // time calls

int open_port(void)
{
int fd; /* File descriptor for the port */
    fd = open("/dev/ttyO1", O_RDWR | O_NOCTTY | O_NDELAY );//open the tty01 folder in readwrite and nodelay
    //fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_SYNC);
    printf("text openport\n");
if (fd == -1)
{
/* Could not open the port. */
perror("open_port: Unable to open /dev/ttyO1");//throw error
}
else
{
  fcntl(fd, F_SETFL, 0);//set the flag to 0 for file descriptor
  printf("Port1 has been succesfully opened.\n");
}
return(fd);

} //open_port

int configure_port(int fd)      // configure the port
{
struct termios port_settings;      // structure to store the port settings in

cfsetispeed(&port_settings, B9600);    // set baud rates
cfsetospeed(&port_settings, B9600);

port_settings.c_cflag &= ~PARENB;    // set no parity, stop bits, data bits
port_settings.c_cflag &= ~CSTOPB;   
port_settings.c_cflag &= ~CSIZE;
port_settings.c_cflag |= CS8;
tcsetattr(fd, TCSANOW, &port_settings);    // apply the settings to the port
printf("text configport\n");
return(fd);

}

int query_modem(int fd)   // query modem with an AT command
{
int m,n,d,e;
//char buff[100];

//n=read(fd,buff,100);
//printf("Result %i\n",n);
//printf("Buf = %s\n\n\n", buff);
//sleep(1);
//memset(buff, 0, 100);
printf("AT+CMGF=1\r\n");
d = write(fd, "AT+CMGF=1\r\n", 11);
if (d < 0)
fputs("write() of 4 bytes failed!\n", stderr);
else
printf("write succeed n0 %i\n",d);

sleep(2);
//d=read(fd, buff, 100);
//printf("Buf = %s\n\n\n", buff);
//memset(buff, 0, 100);

printf("AT+CMGS=\"9019354797\"\r\n"); // \r and \n CR n LF
e = write(fd, "AT+CMGS=\"9019354797\"\rhello,hi from aj group\n\x1A", 30);

if (e < 0)
fputs("write() of 4 bytes failed!\n", stderr);
else
printf("write succeed n0 %i\n",e);
sleep(8);
//e=read(fd, buff, 100);
//printf("Buf = %s\n\n\n", buff);

return (fd);
}


int close_port(int fd)
{
close(fd);
printf("text close port\n");
}
int main(void)
{
int fd = open_port();
sleep(5);
configure_port(fd);
query_modem(fd);
close_port(fd);
return(0);
} //main
