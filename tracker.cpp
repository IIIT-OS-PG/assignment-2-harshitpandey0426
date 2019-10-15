#include<bits/stdc++.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>


#define BUFFER_SIZE 1024



using namespace std;
char store[100];
string search(char buffer[]){
       
        int i;
        string str,final;
       ifstream in_file (store);
        while(!in_file.eof()){
            getline (in_file,str);
            if(strcmp(buffer,str.c_str())==0){
               getline (in_file,str);
               final=final+str;
               final=final+":";
           }
        }
        return final;

}
char tracker_port[10];
int PORT;
void* operations(void* ns)
{
	int newsocket=*((int*)ns);
	int size=1024;
	char serbuff[1000]; char buffer[1000];
	memset(&buffer,'\0',sizeof(buffer));
    recv(newsocket,&size,sizeof(size),0);
    string type;
    string result;
    int i,j;
    for(i=0;i<strlen(buffer);i++){
        if(buffer[i]=='.')break;
        type=type+buffer[i];
    }


    recv(newsocket,buffer,size,0);
	
		if(type=="search"){
        
        int k=0;
        memset(&serbuff,'\0',sizeof(serbuff));
        for(j=i+1;j<strlen(buffer);j++){
           serbuff[k]=buffer[j];
           k++;
        }
        serbuff[20]='\0';
        result=search(serbuff);
    }
}
void* tracker(void* j)
{
	cout<<tracker_port<<"+++";
    PORT=atoi(tracker_port);
    int socketfd;
    struct sockaddr_in serveraddr;
    struct sockaddr_in newAddr;
    socklen_t addr_size;
    int newsocket;

    socketfd=socket(PF_INET,SOCK_STREAM,0);
    memset(&serveraddr,'\0',sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(PORT);
    serveraddr.sin_addr.s_addr=INADDR_ANY;
    cout<<"Tracker Started"<<endl;
  
  
    bind(socketfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    listen(socketfd,5);

    addr_size=sizeof(newAddr);
    while(1)
    {
    newsocket= accept(socketfd,(struct sockaddr*)&newAddr,&addr_size);
    pthread_t new_thread;
	pthread_create(&new_thread,NULL,&operations,NULL);
	pthread_join(new_thread,NULL);

	}
   // pthread_t thread_id;

     //myfunc(newsocket);
    close(socketfd);
}

int main(int argc, char *argv[])
{
	if(argc!=2)
    {
        cout<<"insufficent arguements"; 
        exit(0);
    } 
    char tracker_port[10],mytrackerIP[100];
    int i,argvsize,portpos=0;
    
   
    argvsize=strlen(argv[1]);
    
     for(i=1;i<argvsize;i++){

       tracker_port[portpos]=argv[1][i];
       portpos++;
    }
    tracker_port[portpos]='\0';
    cout<<tracker_port;

    pthread_t new_tr;
	pthread_create(&new_tr,NULL,&tracker,NULL);
	pthread_join(new_tr,NULL);
    
    return 0;


}
