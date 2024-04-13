#include "utils.h"

/**
 * parses first HTTP header str
 * @param header_str the raw string
 * @param ret parsed info
 * @return -1 if failure, 0 if successful
 */
int parse_request_info(char *header_str, struct http_request_info_t *ret) {
    // Here comes painful string parsing in C
    char *basic_request = NULL;
    basic_request = strtok(header_str, "\n");
    if (basic_request == NULL) {
        return 0;
    }

#ifdef DEBUG
    LOG_DEBUG("parse_request_info: parsed %s", basic_request);
#endif

    char method_str[HTTP_MAX_METHOD_STR] = {0};
    char endpoint_str[HTTP_MAX_ENDPOINT_LEN] = {0};
    char http_version_str[HTTP_MAX_VERSION_STR] = {0};
    char *tks[3] = {method_str, endpoint_str, http_version_str};

    // split tokens, if this is not done properly, this will get us SEGFAULTs
    int tkn_idx = 0;
    for (char *tkn = strtok(basic_request, " ");
         tkn != NULL || tkn_idx < 2;
         tkn = strtok(NULL, " "), tkn_idx++) {

        if (strlen(tkn) != 0) {
            strcpy(tks[tkn_idx], tkn);
        }
    }

    // convert
    ret->method = atom(method_str);
    strcpy(ret->endpoint_str, endpoint_str);
    strcpy(ret->http_version, http_version_str);

#ifdef DEBUG
    char d_method_str[HTTP_MAX_METHOD_STR] = {0};
    mtoa(ret->method, (char *) d_method_str);
    LOG_DEBUG("%s, %s, %s", d_method_str, ret->endpoint_str, ret->http_version);
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