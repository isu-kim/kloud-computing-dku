#ifndef HW1_DEFINES_H
#define HW1_DEFINES_H
#pragma once

#define MAX_IP_STRING 16
#define MAX_CONNECTIONS 256
#define MAX_STRING 1024
#define MAX_FILE_NAME_STR_LEN 2048
#define MAX_EPOLL_EVENTS 32

#define HTTP_MAX_ENDPOINT_LEN 1024
#define HTTP_MAX_VERSION_STR 16
#define HTTP_MAX_METHOD_STR 10
#define HTTP_MAX_RESPONSE_LEN 2 << 22 // 4MB
#define HTTP_MAX_REQUEST_LEN 2 << 20 // 1MB

enum http_method_types {
    http_req_unknown = 0,
    http_req_get = 1,
    http_req_post = 2,
    http_req_del = 3,
    http_req_update = 4,
};

#endif //HW1_DEFINES_H
