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
    struct context_t *tmp_ctx = (struct context_t *) malloc(sizeof(struct context_t));
    memset(tmp_ctx, 0, sizeof(struct context_t));

    // Parse arguments using getopt.
    static struct option options[] = {
            {"port", optional_argument, 0, 'p'},
            {"listen", optional_argument, 0, 'l'},
            {"help", no_argument, 0, 'h'},
    };

    int option_index, opt = 0;
    while ((opt = getopt_long(argc, argv, "i:o:fs12", options, &option_index)) != -1) {
        switch (opt) {
            case 'p': // For port option.
                tmp_ctx->port = atoi(optarg);
                break;
            case 'l': // For listen option.
                memset(tmp_ctx->address, 0, MAX_IP_STRING);
                strcpy(tmp_ctx->address, optarg);
                break;
            case 'h': // For help option.
                printf("Usage: http_server [options]\n"
                       "Options\n"
                       "  -h, --help           Print this help message and exit\n"
                       "  -p, --port           Specify an port to listen HTTP server\n"
                       "  -l, --listen         Specify an IPv4 address to listen HTTP server\n"
                );
                return -1;
            default: // Unknown options.
            case '?':
                printf("Check --help or -h for more information\n");
                return -1;
        }
    }

    *ctx = tmp_ctx;
    return 0;
}

/**
 * Prints out ctx
 * @param ctx The context
 * @return 0
 */
int dump_ctx(struct context_t ctx) {
    printf("port=%d\n", ctx.port);
    printf("listen=%s\n", ctx.address);

    return 0;
}
