#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <sys/time.h>

#define SOCKET_PATH "/tmp/validator.sock"

typedef struct {
    char name[32];
    int age;
    unsigned char priority;
} Record;

/* ---------- GUID v1 ---------- */
void generate_guid(char *out) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    snprintf(out, 40, "%08lx-%04lx-1%03lx-%04x-%012lx",
             tv.tv_sec,
             tv.tv_usec & 0xffff,
             tv.tv_usec & 0x0fff,
             0x8000 | (rand() & 0x3fff),
             random());
}

/* ---------- Command ---------- */
typedef void (*Validator)(Record *, char *);

void check_name(Record *r, char *err) {
    if (strlen(r->name) == 0)
        strcat(err, "empty name; ");
}

void check_name_len(Record *r, char *err) {
    if (strlen(r->name) > 31)
        strcat(err, "name too long; ");
}

void check_age(Record *r, char *err) {
    if (r->age < 0 || r->age > 150)
        strcat(err, "invalid age; ");
}

/* ---------- Chain ---------- */
Validator chain[] = {
    check_name,
    check_name_len,
    check_age,
    NULL
};

void validate(Record *r, char *errors) {
    errors[0] = '\0';
    for (int i = 0; chain[i]; i++)
        chain[i](r, errors);
}

int main() {
    srand(time(NULL));
    unlink(SOCKET_PATH);

    int sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock < 0) return 1;

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        return 1;

    printf("Server started\n");

    while (1) {
        Record r;
        recv(sock, &r, sizeof(r), 0);

        char guid[40];
        char errors[256];

        generate_guid(guid);
        validate(&r, errors);

        printf("GUID: %s | %s %d | %s\n",
               guid,
               r.name,
               r.age,
               strlen(errors) ? errors : "OK");
    }

    close(sock);
    unlink(SOCKET_PATH);
    return 0;
}