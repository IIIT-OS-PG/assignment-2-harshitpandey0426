#include<bits/stdc++.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>
#define BUFF_SIZE 1024
using namespace std;
char mycport[100];
//int port;

void* ft_server(void* sock)
{

	FILE *fp = fopen ( "dead_copy.mp4"  , "wb" );
	char Buffer [ BUFF_SIZE] ; 
	int file_size;
	int sockfd=*((int*)sock);

	recv(sockfd, &file_size, sizeof(file_size), 0);


	int n;
	while ( ( n = recv( sockfd , Buffer, BUFF_SIZE, 0) ) > 0  &&  file_size > 0){
		//cout<<n<<"++";
		fwrite (Buffer , sizeof (char), n, fp);
		memset ( Buffer , '\0', BUFF_SIZE);
		file_size = file_size - n;
	} 
	close( sockfd);
	//close( server_fd);
	fclose ( fp );

}
void* server(void *p)
{
	// int portno;
	// cout<<"enter port no server_side";
	// cin>>portno;
	
	int portno=atoi(mycport);
	int server_fd = socket (AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in  addr;

	addr.sin_family = AF_INET;
	//portno = atoi(argv[1]);
	addr.sin_port = htons( portno );
	addr.sin_addr.s_addr=INADDR_ANY;

	cout<<"\nport no - "<<addr.sin_port<<"\n";

	int addrlen = sizeof(sockaddr);

	bind (server_fd  , (struct sockaddr *)&addr , sizeof ( addr ));
	
	listen (server_fd, 3);
	cout<<"\nListening";
	int sockfd;
	// char buffer[10000];
	// bzero(buffer,256);
 //        fgets(buffer,255,stdin);
 //        n = write(sockfd,buffer,strlen(buffer));
 //        if (n < 0) 
 //             error("ERROR writing to socket");
 //        bzero(buffer,256);
 //        n = read(sockfd,buffer,255);
 //        if (n < 0) 
 //             error("ERROR reading from socket");
 //        printf("Server : %s\n",buffer);
 //        //int i = strncmp("Bye" , buffer , 3);
        


	while(1)
	{
	
	sockfd = accept ( server_fd , (struct sockaddr *)&addr , (socklen_t*)&addrlen);
	cout<<"\nport no - "<<addr.sin_port<<"\n";
	cout<<"\nConnection Established";

	pthread_t thread_id;
	pthread_create(&thread_id,NULL,&ft_server,&sockfd);
	pthread_join(thread_id,NULL);
	cout<<"end of thread";

	}
	close(sockfd);
	
	return NULL;
	
}
void *ft_cli(void* sock)
{
	int sockfd=*((int*)sock);
	cout<<"enter file to download\n";
	char str[256];
	cin>>str;
	// int k=str.size();
	// char arr[k];
	// int i=0 ;
	// for(int i=0;i<k;i++)
	// {
	// 	arr[i]=str[i];
	// }
	//arr[k]='\0';

	FILE *fp = fopen ( str , "rb" ); 

	fseek ( fp , 0 , SEEK_END); 
	int size = ftell ( fp );
	rewind ( fp );


	send ( sockfd , &size, sizeof(size), 0);

	char Buffer [ BUFF_SIZE] ; 
	int n;

	while ( ( n = fread( Buffer , sizeof(char) , BUFF_SIZE , fp ) ) > 0  && size > 0 )
	{
			//cout<<n<<"--";
			send (sockfd , Buffer, n, 0 );
	   	 	memset ( Buffer , '\0', BUFF_SIZE);
			size = size - n;
	}

	cout<<"\n Sending file complete";
	fclose ( fp );
	
}

void* client(void* a)
{
	sleep(5);
	
	
	while(1)
	{

	int sockfd = socket( AF_INET, SOCK_STREAM, 0);
	int portno;
	cout<<"enter port";
	cin>>portno;
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	//portno = atoi(argv[1]);
	serv_addr.sin_port = htons( portno );
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");


	cout<<"\nport no - "<<serv_addr.sin_port<<"\n";

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return NULL; 
    } 

    pthread_t thread_id;
    pthread_create(&thread_id,NULL, &ft_cli, &sockfd);

    pthread_join(thread_id,NULL);
    cout<<"client thread end";
    close( sockfd);

	}


return NULL;
}

int main(int argc,char*argv[])
{
	//cin>>port;
	int argvsize;
	int i;
	if (argc!=2)
	{
		cout<<"insufficient argument";
		return 1;
	}
	argvsize=strlen(argv[1]);
	for(i=0;i<argvsize;i++){
        if(argv[1][i]==':'){
            mycport[i]='\0';
            i++;
            break;
        }
        mycport[i]=argv[1][i];
    }
    cout<<mycport;
	pthread_t new_c,new_s;
	pthread_create(&new_c,NULL,&server,NULL);
	pthread_create(&new_s,NULL,&client,NULL);
	pthread_join(new_s,NULL);
	
}