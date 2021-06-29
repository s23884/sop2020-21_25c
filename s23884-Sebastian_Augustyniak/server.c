#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<signal.h>


FILE *log_file; 
int sockfd; 
void error(char *msg)
{
        perror(msg);
        exit(1);
}

 void signalHandler(int s){
     fclose(log_file);
     close(sockfd);
    exit(1);
 }

int main(int argc, char *argv[])

{

        int newsockfd, portno, clilen; 
        log_file = fopen("logs.txt","w");
        signal(SIGINT,&signalHandler);
       
        
        struct sockaddr_in serv_addr, cli_addr;
        int n;
        if (argc<2){                            
                fprintf(stderr,"Error no port\n");
                exit(1);
        }
        sockfd = socket(AF_INET, SOCK_STREAM, 0); 
        if(sockfd<0){                              
                error("error opening sokcet");
        }
        bzero((char *) &serv_addr, sizeof(serv_addr)); 
        portno = atoi(argv[1]); 
        serv_addr.sin_family = AF_INET; 
        serv_addr.sin_addr.s_addr = INADDR_ANY;  
        serv_addr.sin_port = htons(portno);   
        if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
	{  
                error("ERROR on binding");                   
        }       
        listen(sockfd,5);
        clilen = sizeof(cli_addr);

        while(1){
            
           
            char buffer[256];
            newsockfd = accept(sockfd,(struct sockaddr *)  &cli_addr, &clilen);
            
        if(newsockfd<0)
	    {
                     error("ERROR on accept");
        }
         bzero(buffer,256);
         n = read (newsockfd,buffer,255);
          
         if (n< 0 )
	 {
                  error("ERROR reading from socket"); 
        }
        buffer[strcspn(buffer, "\n")] = 0;
        FILE * fp = fopen(buffer,"r");
        
        
        
        long lSize;
        char *content;

        fseek( fp , 0L , SEEK_END);
        lSize = ftell( fp );
        rewind( fp );

        content = calloc( 1, lSize+1 );
       
        fread( content , lSize, 1 , fp);
        fclose(fp);
        char log_info[1024];
        char addr_str[512];
        inet_ntop(AF_INET, &(cli_addr),addr_str , INET_ADDRSTRLEN);
        sprintf (log_info, "%s %s", addr_str,buffer);
        fprintf(log_file,"%s",log_info);
        
        

        n = write(newsockfd,content,sizeof(content));
           if (n< 0 )
	   {
                  error("ERROR writning to socket ");
         }

        }
        
        
        

}

