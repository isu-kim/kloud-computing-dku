#include "server.h"

/**
 * A function that starts listening on designated port and address
 * @return listen fd
 */
int serve() {
    struct sockaddr_in server_addr;
    int ret;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ctx->address);
    server_addr.sin_port = htons(ctx->port);

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        return -1;
    }

    ret = bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret == -1) {
        LOG_ERROR("Fail to bind: %s", (const char *) strerror(errno));
    }

    ret = listen(listen_fd, MAX_CONNECTIONS);
    if (ret == -1) {
        LOG_ERROR("Listen error: %s", (const char*) strerror(errno));
        return -1;
    }

    check_files_dir();
    LOG_INFO("Listening on %s:%d", ctx->address, ctx->port);
    return 0;
}