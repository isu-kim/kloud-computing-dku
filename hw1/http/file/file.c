#include "file.h"

/**
 * Check web files directory
 * @return -1 if target directory is not available
 */
int check_files_dir() {
    LOG_INFO("Checking web files directory");

    if (IS_EXISTING_FILE(ctx->web_files_dir)) {
        LOG_INFO("Web files directory exists, skipping...");
    } else {
        LOG_INFO("Web files directory does not exist, creating one...");
        if (mkdir(ctx->web_files_dir, 777) != 0) {
            LOG_ERROR("Unable to create web files directory: %s", strerror(errno));
            return -1;
        } else {
            return 0;
        }
    }

    return 0;
}

/**
 * Reads a target file and sends to that stream
 * @param client_fd the client socket fd
 * @param target the target file
 * @param header the header string
 * @return -1 if failure 0 if success
 */
int send_file(int client_fd, char* header, char* target) {

}