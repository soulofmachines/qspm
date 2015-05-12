#include "acpi.hpp"
#include <sys/socket.h>
#include <sys/un.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>

int socketConnect(const char* path) {
    int fd, sc;
    struct sockaddr_un addr;

    fd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (fd < 0)
    {
        return fd;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);
    sc = connect(fd, (const sockaddr*) &addr, sizeof(addr));
    if (sc < 0) {
        return sc;
    }

    return fd;
}

char* socketRead(int fd) {
    static char* buf;
    int buflen = 64;
    int maxbuf = 1024;
    int x = 0;
    int rd;
    bool search = true;

    while (search) {
        buf = (char*) realloc(buf, buflen);
        if (!buf) {
            std::cerr << "ACPI: alloc error" << std::endl;
        }
        memset(buf + x, 0, buflen - x);

        while (x < buflen) {
            //rd = TEMP_FAILURE_RETRY (read(fd, buf + x, 1));
            rd = read(fd, buf + x, 1);
            if (rd < 0) {
                std::cerr << "ACPI: read error" << std::endl;
                return NULL;
            } else if (rd == 0) {
                return NULL;
            } else if (rd == 1) {
                if (buf[x] == '\n') {
                    search = false;
                    buf[x] = '\0';
                    break;
                }
                x++;
            }
        }
        if (buflen >= maxbuf) {
            break;
        }
        buflen *= 2;
    }

    return buf;
}
