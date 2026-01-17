#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/validator.sock"

typedef struct {
    char name[32];
    int age;
    unsigned char priority;
} Record;

int main(int argc, char **argv) {
    int N = atoi(argv[1]);

    int sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock < 0) return 1;

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    for (int i = 0; i < N; i++) {
        Record r;
        snprintf(r.name, 32, "User%d", i);
        r.age = rand() % 160 - 10;
        r.priority = i % 3;

        sendto(sock, &r, sizeof(r), 0,
               (struct sockaddr *)&addr, sizeof(addr));
    }

    close(sock);
    return 0;
}
