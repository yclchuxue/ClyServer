#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h> //atoi()
 
int main(int argc, char *argv[])
{	
	//1 创建tcp通信socket
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd == -1)
	{
		perror("socket failed!\n");
	}
 
	//2 连接服务器
	struct sockaddr_in server_addr = {0};//服务器的地址信息
	server_addr.sin_family = AF_INET;//IPv4协议
	server_addr.sin_port = htons(9898);//服务器端口号
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");         //设置服务器IP
	int ret = connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));       //连接服务器
	if(ret == -1)
	{
		perror("connect failed!\n");
	}
	else
	{
		printf("connect server successful!\n");
	}
 
	//char buf[1024] = "GET /index.html HTTP/1.1\r\nHost: www.chenshuo.com\r\n\r\n";
    char buf[1024] = "GET /index.html HTTP/1.1\r\n";

	write(socket_fd,buf,strlen(buf));

    sleep(10);

    char buf1[1024] = "Host: www.chenshuo.com\r\n\r\n";
    write(socket_fd, buf1, strlen(buf));

    sleep(10);

	close(socket_fd);
 
	return 0;
}
 