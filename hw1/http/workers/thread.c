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
    char *buff = (char *)malloc(sizeof(char) * HTTP_MAX_REQUEST_LEN);
    memset(buff, 0x00, HTTP_MAX_REQUEST_LEN);
    struct http_request_info_t req = {0};

    // Read initial buffer, shall know basic request header
    for (int read_n = 0, is_init = 1 ;
         read_n > 0 || is_init == 1 ;
         read_n = read(client_info->fd, buff, HTTP_MAX_REQUEST_LEN), is_init = 0) {
#ifdef DEBUG
        LOG_DEBUG("Received: %s", buff);
#endif

        if (read_n < 0 || parse_request_info(buff, &req) !=0) {
            LOG_ERROR("[%s:%d] Failed to parse HTTP header properly", client_info->addr_str, client_info->port);
        }

        if (req.method == http_req_unknown) {
            continue;
        }

#ifdef DEBUG
        char method_str[HTTP_MAX_METHOD_STR] = {0};
        mtoa(req.method, (char *) method_str);
        LOG_DEBUG("[Worker][%s:%d] %s %s %s",
                  client_info->addr_str, client_info->port, method_str, req.endpoint_str, req.http_version);
#endif

        int ret = 0;
        switch (req.method) {
            case http_req_get:
                http_handler_get(client_info, &req);
                break;
            case http_req_post:
#ifdef DEBUG
                char method_str[HTTP_MAX_METHOD_STR] = {0};
                mtoa(req.method, (char *) method_str);
                LOG_DEBUG("[Worker][%s:%d] %s %s %s reading extra payload",
                          client_info->addr_str, client_info->port, method_str, req.endpoint_str, req.http_version);
#endif
                memset(buff, 0x00, HTTP_MAX_REQUEST_LEN);
                ret = read(client_info->fd, buff, req.content_size > HTTP_MAX_REQUEST_LEN ? HTTP_MAX_REQUEST_LEN : req.content_size);
#ifdef DEBUG
                LOG_DEBUG("[Worker][%s:%d] %s %s %s read %s (%ld)",
                          client_info->addr_str, client_info->port, method_str, req.endpoint_str, req.http_version,
                          buff, req.content_size);
#endif

                http_handler_post(client_info, &req, buff, req.content_size);
                break;
            case http_req_update:
#ifdef DEBUG
                char method_str[HTTP_MAX_METHOD_STR] = {0};
                mtoa(req.method, (char *) method_str);
                LOG_DEBUG("[Worker][%s:%d] %s %s %s reading extra payload",
                          client_info->addr_str, client_info->port, method_str, req.endpoint_str, req.http_version);
#endif
                memset(buff, 0x00, HTTP_MAX_REQUEST_LEN);
                ret = read(client_info->fd, buff, req.content_size > HTTP_MAX_REQUEST_LEN ? HTTP_MAX_REQUEST_LEN : req.content_size);
#ifdef DEBUG
                LOG_DEBUG("[Worker][%s:%d] %s %s %s read %s (%ld)",
                          client_info->addr_str, client_info->port, method_str, req.endpoint_str, req.http_version,
                          buff, req.content_size);
#endif

                http_handler_update(client_info, &req, buff, req.content_size);
                break;
        }
    }

    free(buff);
    return NULL;
}
