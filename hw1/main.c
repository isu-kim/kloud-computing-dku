#include "common/log.h"
#include "common/context.h"
#include "http/server.h"

// @todo implement main

struct context_t *ctx;

int main(int argc, char* argv[]) {
    LOG_INFO("Initializing commandline arguments");
    if (init_ctx(&ctx, argc, argv) == -1) {
        return -1;
    }

    LOG_INFO("Initialized arguments");
    dump_ctx(*ctx);

    LOG_INFO("Starting server...");
    serve();

	return 0;
}
