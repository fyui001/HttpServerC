#include <stdio.h>
#include <string.h>

#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#define PORT 80
#define HTTP_BUF 4096
#define HTTP_ADD_LEN 4096


int main(){
    int sock, sock0, len;
    int y = 1;
    char buf[HTTP_BUF], add_buf[HTTP_ADD_LEN];
    struct sockaddr_in socketaddr_s, socketaddr_c;

    if((sock0 = socket(AF_INET, SOCK_STREAM, 0))  < 0) {
        perror("socket");
        return -1;
    }

    socketaddr_s.sin_family = AF_INET;
    socketaddr_s.sin_port = htons(PORT);
    socketaddr_s.sin_addr.s_addr = INADDR_ANY;


    setsockopt(sock0, SOL_SOCKET, SO_REUSEADDR, (const char *) &y, sizeof(y));

    if (bind(sock0, (struct sockaddr *) &socketaddr_s, sizeof(socketaddr_s)) != 0) {
		perror("bind");
		return -1;
	}

    if(listen(sock0, 5) != 0){
        perror("listen");
        return -1;
    }

    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(buf),
        "HTTP/1.1 200 OK\r\n"
        "Last-Modified: Sun, 26 Feb 2017 15:06:20 GMT\r\n"
        "Connection: close\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "\r\n"
        "Hello World\r\n"
    );

    while(1){
        len = sizeof(socketaddr_c);
        if(sock = accept(sock0, (struct sockaddr *) &socketaddr_c, &len) < 0){
            perror("accept");
            return -1;
        }

        memset(add_buf, 0, sizeof(add_buf));
        recv(sock, add_buf, sizeof(add_buf), 0);
        send(sock, buf, (int) strlen(buf), 0);

        close(sock);
    }

    close(sock0);
    return 0;
}
