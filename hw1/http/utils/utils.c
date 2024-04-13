#include "utils.h"

/**
 * parses first HTTP header str
 * @param header_str the raw string
 * @param ret parsed info
 * @return -1 if failure, 0 if successful
 */
int parse_request_info(char *header_str, struct http_request_info_t *ret) {
    // Here comes painful string parsing in C
    // I literally have no idea of this string thing, this got me stuck for like two more hours
    char *header_copy_1 = strdup(header_str);
    char *header_copy_3 = strdup(header_str);
    if (header_copy_1 == NULL || header_copy_3 == NULL) {
        return -1;
    }

    char *basic_request = NULL;
    basic_request = strtok(header_copy_1, "\n");
    if (basic_request == NULL) {
        return 0;
    }

#ifdef DEBUG
    LOG_DEBUG("parse_request_info: parsed \"%s\"", basic_request);
#endif

    char method_str[HTTP_MAX_METHOD_STR] = {0};
    char endpoint_str[HTTP_MAX_ENDPOINT_LEN] = {0};
    char http_version_str[HTTP_MAX_VERSION_STR] = {0};

    if (basic_request != NULL && strlen(basic_request) != 0) {
        sscanf(basic_request, "%s %s %s", method_str, endpoint_str, http_version_str);
    }

    // convert string job
    ret->method = atom(method_str);
    strcpy(ret->endpoint_str, endpoint_str);
    strcpy(ret->http_version, http_version_str);
    if (ret->method == 0 || strlen(ret->endpoint_str) == 0 || strlen(ret->http_version) == 0) {
        return -1;
    }

    // parse content length
    char *content_length_str = strstr(header_copy_3, "Content-Length:");
    if (content_length_str != NULL && strlen(content_length_str) != 0) {
        sscanf(content_length_str, "Content-Length: %ld", &(ret->content_size));
    }

#ifdef DEBUG
    char d_method_str[HTTP_MAX_METHOD_STR] = {0};
    mtoa(ret->method, (char *) d_method_str);
    LOG_DEBUG("Header info: %s, %s, %s, %ld", d_method_str, ret->endpoint_str, ret->http_version, ret->content_size);
#endif

    return 0;
}

/**
 * Converts string to method
 * @return converted method, 0 if unknown
 */
uint16_t atom(char *type) {
    char lowered[HTTP_MAX_METHOD_STR] = {0};
    for (int i = 0 ; type[i] != 0 && i < HTTP_MAX_METHOD_STR ; i++) {
        lowered[i] = tolower(type[i]);
    }

    // yes, this is very stupid
    if (!strcmp(lowered, "get")) {
        return http_req_get;
    }

    if (!strcmp(lowered, "post")) {
        return http_req_post;
    }

    if (!strcmp(lowered, "update")) {
        return http_req_update;
    }

    if (!strcmp(lowered, "delete")) {
        return http_req_del;
    }

    return 0;
}

/**
 * Converts method to string
 * @param type request type
 * @param ret return string
 * @return -1 if failure 0 if success
 */
int mtoa(uint16_t type, char *ret) {
    switch (type) {
        case http_req_get:
            strcpy(ret, "GET");
            return 0;
        case http_req_post:
            strcpy(ret, "POST");
            return 0;
        case http_req_update:
            strcpy(ret, "UPDATE");
            return 0;
        case http_req_del:
            strcpy(ret, "DELETE");
            return 0;
        default:
            strcpy(ret, "UNKNOWN");
            return -1;
    }
}