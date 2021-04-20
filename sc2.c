#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

/*#define PORT 80*/
#define PORT 1025
/*#define IP "216.58.193.4" // google.com*/
#define IP "127.0.0.1"

int main() {
    int fd;
    // 1. Crear socket TCP
    fd = socket(AF_INET, SOCK_STREAM, 0);
    // 2. Establecer conexion
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &addr.sin_addr);
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Unable to connect\n");
        close(fd);
        exit(1);
    }
    // 3. Comunicacion
    /*char *str = "GET / HTTP/1.1\r\n\r\n";*/
    char *str = "hola 2";
    write(fd, str, strlen(str));
    printf("%s sent\n", str);
    
    int tam = strlen(str);
    char *buffer = malloc(tam*sizeof(char));
    read(fd, buffer, tam);
    printf("%s received\n", buffer);
    str = "OK";
    write(fd, str, strlen(str));
    free(buffer);
    // 4. Cerrar conexion
    close(fd);

    return 0;
}
