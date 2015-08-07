#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "pthread.h"
#define BUFSIZE 8096
#define ERROR      42
#define LOG        44
#define FORBIDDEN 403
#define NOTFOUND  404



struct {
    char *ext;
    char *filetype;
} extensions [] = {
        {"gif", "image/gif" },
        {"jpg", "image/jpg" },
        {"jpeg","image/jpeg"},
        {"png", "image/png" },
        {"ico", "image/ico" },
        {"zip", "image/zip" },
        {"gz",  "image/gz"  },
        {"tar", "image/tar" },
        {"htm", "text/html" },
        {"html","text/html" },
        {"mpg","video/mpg"},
        {0,0} };

void* web(void* fd_t) {
    int fd = *((int*)fd_t);
    int file_fd, buflen;
    long i, ret, len;
    char * fstr;
    static char buffer[BUFSIZE+1];

    ret =read(fd,buffer,BUFSIZE);
    if(ret == 0 || ret == -1) {
    }
    if(ret > 0 && ret < BUFSIZE)
        buffer[ret]=0;
    else buffer[0]=0;
    for(i=0;i<ret;i++)
        if(buffer[i] == '\r' || buffer[i] == '\n')
            buffer[i]='*';
    if( strncmp(buffer,"GET ",4) && strncmp(buffer,"get ",4) ) {
    }
    for(i=4;i<BUFSIZE;i++) {
        if(buffer[i] == ' ') {
            buffer[i] = 0;
            break;
        }
    }

    buflen=strlen(buffer);
    fstr = (char *)0;
    for(i=0;extensions[i].ext != 0;i++) {
        len = strlen(extensions[i].ext);
        if( !strncmp(&buffer[buflen-len], extensions[i].ext, len)) {
            fstr =extensions[i].filetype;
            break;
        }
    }
    if(fstr == 0);

    if(( file_fd = open(&buffer[5],O_RDONLY)) == -1) {
    }

    len = (long)lseek(file_fd, (off_t)0, SEEK_END);
    (void)lseek(file_fd, (off_t)0, SEEK_SET);
    (void)sprintf(buffer,"HTTP/1.1 200 OK\nServer: thread-webserver\nContent-Length: %ld\nConnection: close\nContent-Type: %s\n\n", len, fstr);
    (void)write(fd,buffer,strlen(buffer));

    while (	(ret = read(file_fd, buffer, BUFSIZE)) > 0 ) {
        (void)write(fd,buffer,ret);
    }
    //sleep(1);
    close(fd);
    //  exit(1);
    return NULL;
}

int main(int argc, char **argv) {
    int  port, listenfd,threadLength;
    int socketfd;
    socklen_t length;
    static struct sockaddr_in cli_addr;
    static struct sockaddr_in serv_addr;

    if( argc < 7  || argc > 7 || strcmp(argv[1],"-n")!=0 || strcmp(argv[3],"-w")!=0 || strcmp(argv[5],"-p")!=0) {
        (void)printf("Ops!! la manera adecuada de correr el web server es:\n\t$ thread-webserver -n <cantidad-hilos> -w <path-www-root> -p <port>");
        exit(0);
    }
    if(chdir(argv[4]) == -1){
        (void)printf("ERROR con la ruta: %s\n",argv[4]);
        exit(4);
    }

    (void)signal(SIGCLD, SIG_IGN);
    (void)signal(SIGHUP, SIG_IGN);
//    for(i=0;i<32;i++)
    //       (void)close(i);
//    (void)setpgrp();

    printf("\nCorriendo el webserver...\n");

    if((listenfd = socket(AF_INET, SOCK_STREAM,0)) <0)
        printf("\nerror creado el socket\n");
    port = atoi(argv[6]);
    threadLength = atoi(argv[2]);
    pthread_t  threads[threadLength];
    int busyThreads[threadLength];
    int k;

    for (k=0; k< threadLength; k++) {
        busyThreads[k]=0;
//        pthread_create(&threads[k],NULL,web,(void*)&socketfd);
    }


    printf("\n %d hilos creados \n",threadLength);

    if(port < 0 || port >60000)
        printf("\nerror, puerto invalido\n");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);
    if(bind(listenfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) <0)
        printf("\nerror de bind\n");
    if( listen(listenfd,64) <0)
        printf("\nerror de listen\n");
    int available;
    int lastsocked=-2;
    for(;;) {
        sleep(1);
        length = sizeof(cli_addr);
        if((socketfd = accept(listenfd, (struct sockaddr *)&cli_addr, &length)) < 0) {
            printf("\nerror de accept\n");
        }
        else if(lastsocked != socketfd) {
            lastsocked = socketfd;
            //(void) close(listenfd);
            //       web(socketfd);
            /*
         * VERIFICAR QUE LA COLA ESTE VACIA
         *
         * */

            available = -1;
            int u;
            for (u = 0; u < threadLength && available==-1; u++) {
                if (busyThreads[u] == 0) {
                    available = u;
                    busyThreads[u]=1;
                    break;
                }            }
            if (available == -1) {
                printf("\nNo hay hilos disponibles\n");
                close(socketfd);
                exit(0);
            }
            else {
                printf("\nse correra el hilo: %d\n", available+1);
                int temp = socketfd;
                // web(socketfd);
                pthread_create(&threads[available], NULL, web, (void *) &temp);
                //pthread_join(threads[available],NULL);
            }

        }

    }
}
