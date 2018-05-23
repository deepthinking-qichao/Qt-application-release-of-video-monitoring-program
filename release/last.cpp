#include <stdio.h> 
#include <string.h> 
#include <sys/types.h> 
#include <errno.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <termios.h> 
#include <stdlib.h> 

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop) 
{ 
    struct termios newtio,oldtio; 
    if ( tcgetattr( fd,&oldtio) != 0) { 
        perror("SetupSerial 1"); 
        return -1; 
    } 


    bzero( &newtio, sizeof( newtio ) ); 
    newtio.c_cflag |= CLOCAL | CREAD; 
    newtio.c_cflag &= ~CSIZE; 
    switch( nBits ) 
    { 
	case 7: 
	newtio.c_cflag |= CS7; 
	break; 


	case 8: 
	newtio.c_cflag |= CS8; 
	break; 
    } 


    switch( nEvent ) 
    { 
	case 'O': 
	newtio.c_cflag |= PARENB; 
	newtio.c_cflag |= PARODD; 
	newtio.c_iflag |= (INPCK | ISTRIP); 
	break; 


	case 'E': 
	newtio.c_iflag |= (INPCK | ISTRIP); 
	newtio.c_cflag |= PARENB; 
	newtio.c_cflag &= ~PARODD; 
	break; 


	case 'N': 
	newtio.c_cflag &= ~PARENB; 
	break; 
    } 


    switch( nSpeed ) 
    { 
	case 2400: 
	cfsetispeed(&newtio, B2400); 
	cfsetospeed(&newtio, B2400); 
	break; 


	case 4800: 
	cfsetispeed(&newtio, B4800); 
	cfsetospeed(&newtio, B4800); 
	break; 


	case 9600: 
	cfsetispeed(&newtio, B9600); 
	cfsetospeed(&newtio, B9600); 
	break; 


	case 115200: 
	cfsetispeed(&newtio, B115200); 
	cfsetospeed(&newtio, B115200); 
	break; 


	default: 
	cfsetispeed(&newtio, B9600); 
	cfsetospeed(&newtio, B9600); 
	break; 
    } 


    if( nStop == 1 ) 
        newtio.c_cflag &= ~CSTOPB; 
    else if ( nStop == 2 ) 
	newtio.c_cflag |= CSTOPB; 
    newtio.c_cc[VTIME] = 0; 
    newtio.c_cc[VMIN] = 0; 
    tcflush(fd,TCIFLUSH); 
    if((tcsetattr(fd,TCSANOW,&newtio))!=0) 
    { 
	perror("com set error"); 
	return -1; 
    } 


    printf("set done!\n"); 
    return 0; 
} 


int main(void) 
{ 
    sleep(2);

    //*********************set tty***************************//
    int fd; 
    int nread; 

    fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY);
    if(fd<0)
    {
        perror("open error"); 
        return 0;
    }
    if( (set_opt(fd,115200,8,'N',1))<0 )
    { 
        perror("set_opt error"); 
        return 0; 
    } 

    //*********************set FIFO*************************//
    const char *fifo_name = "/tmp/my_fifo";
    int pipe_fd;
    char buff[10]; 
    char buff_rs232[10];
    memset(buff,'\0',sizeof(buff));

    pipe_fd = open(fifo_name,O_RDONLY | O_NDELAY);
    if( pipe_fd < 0)
    {
        perror("open error"); 
        return 0;
    }

    
    int cnt=0;
    int num=8;
    while(1)
    {
        num=0;
        while(num!=8)
        {
           nread=read(fd,buff_rs232+num,8-num);
           num+=nread;
        }
       
        printf("adc%d\n",num); 
        write(fd,buff,8);
        int pipe_num=0;
        if( (pipe_num = read(pipe_fd,buff,8)) != 0 )
        {
           printf("edf%d\n",pipe_num);
           /*while(pipe_num!=8)
           {
              int res=read(pipe_fd,buff+pipe_num,8-pipe_num);
              printf("lsv%d\n",num);
              pipe_num +=res;
           }*/
          
        }
    
        /*cnt++
        if(cnt==10)
        {
             buff[8]='\0';
             printf("nread=%d,%s\n",8,buff); 
             cnt=0;
        }*/               
    }
    close(pipe_fd);
    close(fd);      
    return 0; 
} 
