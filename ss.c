#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 1025

int main() {
    int fd;
    // 1. Crear socket TCP
    fd = socket(AF_INET, SOCK_STREAM, 0);
    // 2. Asignar puerto a socket
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    // 3. Poner al socket en modo escucha (bind)
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Unable to bind\n");
        close(fd);
        exit(1);
    }
    if (listen(fd, 1) == -1) {
        perror("Unable to listen\n");
    }
    // 4. Repetir
    int connfd, connfd2;
    struct sockaddr_in cli_addr;
    char *buffer = (char *)malloc(50*sizeof(char));
    char *load = (char *)malloc(6*sizeof(char));
    printf("Entro\n");
    // 4.1 Aceptar nueva conexion
    socklen_t cli_addr_len = sizeof(cli_addr);
    connfd = accept(fd, (struct sockaddr *)&cli_addr, &cli_addr_len);
    printf("IP address is: %s\n", inet_ntoa(cli_addr.sin_addr));
    connfd2 = accept(fd, (struct sockaddr *)&cli_addr, &cli_addr_len);
    printf("IP address is: %s\n", inet_ntoa(cli_addr.sin_addr));
    if (connfd == -1 || connfd2 == -1) {
        perror("Unable to connect\n");
    }
    // 4.2 Comunicacion
    read(connfd, load, 6);
    printf("conn1: %s received\n", load);
    read(connfd2, load, 6);
    printf("conn2: %s received\n", load);

    read(connfd, buffer, 50);
    printf("%s received\n", buffer);
    char *str = "OK";
    write(connfd, str, strlen(str));
    /*if (tolower(*buffer) - 'a' >= 25) {
        *buffer = 'a';
    }
    else{
        *buffer += 1;
    }*/
    char rev[1000];
    int j, count=0;
    while(buffer[count] != '\0'){
    	count++;
    }
    j = count - 1;
    //Hago el reversa
    for(int i=0; i < count; i++){
    	rev[i] = buffer[j];
    	j--;
    }
    
    write(connfd2, rev, strlen(rev));
    printf("%s sent\n", rev);

    read(connfd2, str, 50);
    printf("%s received\n", str);

    free(buffer);
    
    // 4.3 Cerrar conexion
    close(connfd);
    close(fd);
    printf("finished\n");

    return 0;
}
