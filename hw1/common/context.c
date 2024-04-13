#include "context.h"

/**
 * Initializes master context for program
 * @param ctx The target context to write to
 * @param argc The argc
 * @param argv The argv
 * @return -1 if failure, 0 if successful
 */
int init_ctx(struct context_t **ctx, int argc, char **argv) {
    // Malloc a new context.
    *ctx = (struct context_t *) malloc(sizeof(struct context_t));
    if (*ctx == NULL) {
        printf("Memory allocation failed\n");
        return -1;
    }
    memset(*ctx, 0, sizeof(struct context_t));

    // Parse arguments using getopt.
    static struct option options[] = {
            {"port", required_argument, 0, 'p'},
            {"listen", required_argument, 0, 'l'},
            {"files", required_argument, 0, 'f'},
            {"help", no_argument, 0, 'h'},
    };

    int option_index, opt;
    while ((opt = getopt_long(argc, argv, "p:l:h", options, &option_index)) != -1) {
        switch (opt) {
            case 'p': // For port option.
                (*ctx)->port = atoi(optarg);
                break;
            case 'l': // For listen option.
                memset((*ctx)->address, 0, MAX_IP_STRING);
                strncpy((*ctx)->address, optarg, MAX_IP_STRING - 1);
                break;
            case 'f': // For listen option.
                memset((*ctx)->address, 0, MAX_IP_STRING);
                strncpy((*ctx)->address, optarg, MAX_IP_STRING - 1);
                break;
            case 'h': // For help option.
                printf("Usage: http_server [options]\n"
                       "Options\n"
                       "  -h, --help           Print this help message and exit\n"
                       "  -p, --port           Specify an port to listen HTTP server\n"
                       "  -l, --listen         Specify an IPv4 address to listen HTTP server\n"
                       "  -f, --files          Specify directory of web files\n"
                );
                return -1;
            default: // Unknown options.
                printf("Check --help or -h for more information\n");
                return -1;
        }
    }

    if (strlen((*ctx)->address) == 0) {
        LOG_INFO("No listen address was specified, using default address: 0.0.0.0");
        strcpy((*ctx)->address, "0.0.0.0");
    }

    if (strlen((*ctx)->web_files_dir) == 0) {
        LOG_INFO("No web files directory was specified, using default: web/");
        strcpy((*ctx)->web_files_dir, "web/");
    }

    if ((*ctx)->port <= 0 || (*ctx)->port > 65535) {
        LOG_WARN("Invalid port was specified, using default address: 5000");
        (*ctx)->port = 5000;
    }

    return 0;
}

/**
 * Prints out ctx
 * @param ctx The context
 * @return 0
 */
int dump_ctx(struct context_t ctx) {
    printf("- port=%d\n", ctx.port);
    printf("- listen=%s\n", ctx.address);
    printf("- files_directory=%s\n", ctx.web_files_dir);
    return 0;
}
