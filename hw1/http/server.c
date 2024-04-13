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
        return -1;
    }

    ret = listen(listen_fd, MAX_CONNECTIONS);
    if (ret == -1) {
        LOG_ERROR("Listen error: %s", (const char*) strerror(errno));
        return -1;
    }

    check_files_dir();
    LOG_INFO("Listening on %s:%d", ctx->address, ctx->port);

    struct sockaddr_in client_addr = {0};
    char ip_str[MAX_IP_STRING] = {0};

    while(1) {
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN);

        if(client_fd == -1) {
            LOG_WARN("Unable to accept connection: %s", strerror(errno));
            continue;
        } else {
#ifdef DEBUG
            LOG_DEBUG("[%s:%d] New connection, assigning a new worker process", ip_str, client_addr.sin_port);
#endif
            // Create client struct
            struct client_info_t *client = NULL;
            client = (struct client_info_t *)malloc(sizeof(struct client_info_t));
            client->fd = client_fd;
            client->port = client_addr.sin_port;
            memcpy(client->addr_str, ip_str, MAX_IP_STRING);

            if (spawn_worker_thread(client) != 0) {
                LOG_ERROR("[%s:%d] Unable to handle connection properly: %s",
                          client->addr_str, client->port, strerror(errno));
                continue;
            }
        }
    }

    return 0;
}