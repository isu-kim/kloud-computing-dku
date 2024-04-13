#include "thread.h"

/**
 * Starts a new worker thread for connection
 * @return -1 if failure, 0 if success
 */
int spawn_worker_thread(void *target) {
    pthread_t wt = {0};
    pthread_create(&wt, NULL, worker_func, target);
    pthread_detach(wt);

    return 0;
}

/**
 * Takes care of the request
 * @return -1 if failure, 0 if success
 */
void* worker_func(void *target) {
    struct client_info_t *client_info = (struct client_info_t *)target;
    char buff[MAX_REQUEST_SIZE] = {0};
    struct http_request_info_t req = {0};

    // Read initial buffer, shall know basic request header
    for (int read_n = 0, is_init = 1 ;
         read_n > 0 || is_init == 1 ;
         read_n = read(client_info->fd, buff, MAX_REQUEST_SIZE), is_init = 0) {
#ifdef DEBUG
        LOG_DEBUG("Received: %s", buff);
#endif

        if (parse_request_info(buff, &req) !=0) {
            LOG_ERROR("[%s:%d] Failed to parse HTTP header properly", client_info->addr_str, client_info->port);
            return NULL;
        }

        if (req.method == http_req_unknown) {
            continue;
        }

#ifdef DEBUG
        char method_str[HTTP_MAX_METHOD_STR] = {0};
        mtoa(req.method, (char *) method_str);
        LOG_DEBUG("[Worker][%s:%d] %s %s %s", client_info->addr_str, client_info->port, method_str, req.endpoint_str, req.http_version);
#endif

        switch (req.method) {
            case http_req_get:
                http_handler_get(client_info, &req);
                break;
        }
    }

    return NULL;
}
