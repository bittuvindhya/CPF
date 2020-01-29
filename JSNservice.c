#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#define PORT 1234
void* multithread(void *arg)
{
	int newsock_fd=(int)arg;
        FILE *fptr;
        char *buffer1;
   	buffer1=(char *)malloc(1024 *sizeof(char)); 
	memset(buffer1,0,sizeof(buffer1));
	fptr=fopen("ifconfig.txt","w+");
	while(recv(newsock_fd,buffer1,1024,0)!=NULL)
	{	
	fwrite(buffer1,sizeof(char),strlen(buffer1),fptr);
	fscanf(fptr,"[^\n]",buffer1);
        printf("%s",buffer1);	
	}
	fclose(fptr);
//	while(fgets(fptr,"%[^\n]",buffer1)!=EOF)
//	{
//		printf("%s",buffer1);
//		fclose(fptr);
//	}

   if(fptr == NULL)
   {
      printf("Error!");   
      exit(1);             
   }
  // fprintf(fptr,"%s",buffer2);
//fclose(fptr);
close(newsock_fd);
}
int main()
{
	int sock_fd,newsock_fd,cliaddrlen;
	
	char *buffer;
	struct sockaddr_in servaddr,cliaddr;
	pthread_t serverThread;
	sock_fd=socket(AF_INET,SOCK_STREAM,0);
        printf("\nSocket created");
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(PORT);
	bind(sock_fd,(struct sockaddr*)&servaddr,sizeof(servaddr));
        printf("\nBinding");	
        listen(sock_fd,5);
        printf("\nlistening");
	while(1)
	{
		printf("\nSERVER WAITING FOR NEW CONNECTION\n");
		cliaddrlen=sizeof(cliaddr);
		newsock_fd=accept(sock_fd,(struct sockaddr*)&cliaddr,&cliaddrlen);
		printf("CONNECTED TO CLIENT\n");
		pthread_create(&serverThread,NULL,&multithread,(void *)newsock_fd);	
       	}
	return 0;
}


