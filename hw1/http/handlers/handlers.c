#include "handlers.h"

/**
 * Sends response message with HTTP header
 * @param client_fd the client fd
 * @param response_code HTTP response code to return
 * @param payload full buffer of payload to return
 * @param type the content type
 * @return -1 if failure, 0 if success
 */
int send_response_message(int client_fd, int response_code, char *payload, char *type) {
    // yes, this might be painfully slow since we need to control this
    char *response = (char *)malloc(sizeof(char) * HTTP_MAX_RESPONSE_LEN);
    memset(response, 0x00, HTTP_MAX_RESPONSE_LEN);

    generate_http_header(response_code, strlen(payload), response, type);
    strcat(response, payload);
#ifdef DEBUG
    LOG_DEBUG("Response: %s", response);
#endif

    ssize_t bytes = write(client_fd, response, strlen(response));
    free(response); // I wish we had defer here :(

    return bytes >= 0;
}

/**
 * HTTP handler for GET
 * @param req the request
 * @return -1 if failure, 0 if success
 */
int http_handler_get(struct client_info_t *client, struct http_request_info_t *req) {
    int response_code = 0;

    if (strcmp(req->endpoint_str, "/") == 0) { // shall be 200
        response_code = 200;
        char payload[MAX_STRING] = {0};
        strcat(payload, title_msg);
        strcat(payload, "\n\n    It works, engineX is running! :)   ");
        send_response_message(client->fd, response_code, payload, "text");
    } else {
        char target_file[MAX_FILE_NAME_STR_LEN] = {0};
        strcpy(target_file, ctx->web_files_dir);
        strcat(target_file, req->endpoint_str);
#ifdef DEBUG
        LOG_DEBUG("Opening file %s", target_file);
#endif
        if (!IS_EXISTING_FILE(target_file)) { // shall be 404
            response_code = 404;
#ifdef DEBUG
            LOG_DEBUG("File %s does not exist: %s", target_file, strerror(errno));
#endif
            char msg[] =  "Not found, we don't have that file :b";
            send_response_message(client->fd, response_code, msg, "text");
        } else if (!IS_AVAILABLE_FILE(target_file)) { // shall be 403
            response_code = 403;
            char msg[] = "Forbidden, guessing you don't have permissions :(";
            send_response_message(client->fd, response_code, msg, "text");
        } else { // read files
            long size = get_file_size(target_file);
            char *file_content = (char *)malloc(sizeof(char) * size);

            // @todo calculate header offset as well, this will NOT consider header size in here
            // meaning that potential buffer overflow might occur, think size == HTTP_MAX_RESPONSE_LEN
            int ret = read_file(target_file, file_content);
            LOG_INFO("[Worker][%s:%d] Sending file %s (%ld bytes)", client->addr_str, client->port, target_file, size);

            // for html files, return HTML as the file type for plain/html
            char file_type[5] = {0};
            if (strcasecmp(target_file + strlen(target_file) - 5, ".html") == 0) {
                strcpy(file_type, "html");
            } else {
                strcpy(file_type, "text");
            }

            // check stats
            if (ret == 0) { // success
                response_code = 200;
                send_response_message(client->fd, response_code, file_content, file_type);
            } else if (ret == -2) { // too big
                response_code = 413;
                sprintf(file_content, "File size too big! Max size=%d bytes", HTTP_MAX_RESPONSE_LEN);
                send_response_message(client->fd, response_code, file_content, file_type);
            } else { // file open failed
                response_code = 503;
                sprintf(file_content, "Failed to open file %s properly: %s", target_file, strerror(errno));
                send_response_message(client->fd, response_code, file_content, file_type);
            }
        }
    }

    // generate method str
    char method_str[HTTP_MAX_METHOD_STR] = {0};
    mtoa(req->method, (char *) method_str);

    // log accordingly
    if (response_code / 100 == 4) { // 4xx error
        LOG_WARN("[Worker][%s:%d][%d] %s %s %s", client->addr_str, client->port, response_code,
                 method_str, req->endpoint_str, req->http_version);
    } else if (response_code / 100 == 5) { // 5xx error
        LOG_ERROR("[Worker][%s:%d][%d] %s %s %s", client->addr_str, client->port, response_code,
                 method_str, req->endpoint_str, req->http_version);
    } else { // 200 ok
        LOG_INFO("[Worker][%s:%d][%d] %s %s %s", client->addr_str, client->port, response_code,
                method_str, req->endpoint_str, req->http_version);
    }

    return 0;
}

/**
 * Generate basic HTTP header
 * @param response_code the response code
 * @param size the response size
 * @param ret the return value
 * @param type the content type
 * @return 0 always
 */
int generate_http_header(int response_code, unsigned long size, char *ret, char *type) {
    sprintf(ret,
            "HTTP/1.1 %d\r\n"
            "Content-Type: text/%s\r\n"
            "Content-Length: %lu\r\n"
            "\r\n",
            response_code, type, size);

    return 0;
}