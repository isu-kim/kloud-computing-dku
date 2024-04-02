#include "utils/log/log.h"
#include "utils/cli/cli.h"
#include "common/types.h"
#include "common/context.h"
#include "http/server.h"

// @todo implement main

struct context_t *ctx;

int main(int argc, char* argv[]) {
    // Initialize context, parse command-line arguments.
    if (init_ctx(&ctx, argc, argv) == -1) {
        return -1;
    }

    dump_ctx(*ctx);

	return 0;
}
