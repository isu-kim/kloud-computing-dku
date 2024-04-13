#include "handlers.h"

/**
 * HTTP handler for GET
 * @param req the request
 * @return -1 if failure, 0 if success
 */
int http_handler_get(struct client_info_t *client, struct http_request_info_t *req) {
    int response_code = 0;

    if (strcmp(req->endpoint_str, "/") == 0) {
        char response[MAX_STRING] = {0};
        char msg[MAX_STRING] = {0};
        strcpy(msg, title_msg);

        strcat(msg, "\n\n It Works!!! engineX Web Server is Running!\n");
        response_code = 200;

        // generate header and send content
        generate_http_header(req, response_code, strlen(msg), response);
        strcat(response, msg);

        (void) write(client->fd, response, strlen(response));
    } else {
        char target_file[MAX_FILE_NAME_STR_LEN] = {0};
        strcpy(target_file, ctx->web_files_dir);
        strcat(target_file, req->endpoint_str);

#ifdef DEBUG
        LOG_DEBUG("Opening file %s", target_file);
#endif

        if (!IS_EXISTING_FILE(target_file)) { // shall be 404
            char response[MAX_STRING] = {0};
            char msg[] = "Not found";
            response_code = 404;

            generate_http_header(req, response_code, strlen(msg), response);
            strcat(response, msg);
            (void) write(client->fd, response, strlen(response));
        }
    }

    char method_str[HTTP_MAX_METHOD_STR] = {0};
    mtoa(req->method, (char *) method_str);
    LOG_INFO("[Worker][%s:%d][%d] %s %s %s", client->addr_str, client->port, response_code,
             method_str, req->endpoint_str, req->http_version);

    return 0;
}

/**
 * Generate basic HTTP header
 * @param req the request
 * @param response_code the response code
 * @param size the response size
 * @param ret the return value
 * @return 0 always
 */
int generate_http_header(struct http_request_info_t *req, int response_code, unsigned long size, char *ret) {
    sprintf(ret,
            "HTTP/1.1 %d\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: %lu\r\n"
            "\r\n",
            response_code, size);

    return 0;
}