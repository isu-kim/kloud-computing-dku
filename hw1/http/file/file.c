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
        if (mkdir(ctx->web_files_dir, S_IRWXU | S_IRWXG | S_IRWXO) != 0) {
            LOG_ERROR("Unable to create web files directory: %s", strerror(errno));
            return -1;
        } else {
            return 0;
        }
    }

    return 0;
}

/**
 * Gets file size
 * @param target the target
 * @return target size in bytes, -1 if failure
 */
long get_file_size(char *target) {
    FILE* file = fopen(target, "r");
    if (file == NULL)
        return -1; // Failed to open file
    fseek(file, 0, SEEK_END);

    int size = ftell(file);
    fclose(file);
    return size;
}


/**
 * Reads file and saves it to return
 * If the size of the file exceeds HTTP_MAX_RESPONSE_LEN, will return -2
 * @param target the file to read
 * @param ret the buffer to store
 * @return -1 if any error, -2 if file size exceeds max limit, 0 if no error
 */
int read_file(char *target, char *ret) {
    FILE* file = fopen(target, "r");
    if (file == NULL) {
        LOG_ERROR("Unable to open file: %s", strerror((errno)));
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size > HTTP_MAX_RESPONSE_LEN) {
        fclose(file);
        return -2;
    }

    size_t bytes_read = fread(ret, 1, file_size, file);
    if (bytes_read != file_size) {
        fclose(file);
        LOG_ERROR("Unable to read file: %s", strerror((errno)));
        return -1;
    }

    fclose(file);
    return 0;
}

/**
 * File write from buffer
 * @param target the target file
 * @param buff the buffer
 * @param written written bytes
 * @return -1 if failure 0 if success
 */
int write_file(char *target, char *buff, long *written){
    FILE* file = fopen(target, "w+");
    if (file == NULL) {
        LOG_ERROR("Unable to open file: %s", strerror((errno)));
        return -1;
    }

    *written = fprintf(file, "%s", buff);
    if (*written < 0) {
        LOG_ERROR("Error writing to file: %s", strerror(errno));
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

/**
 * File delete
 * @param target the target file
 * @return -1 if failure, 0 if success
 */
int delete_file(char *target) {
    if (remove(target) == 0) {
        return 0;
    } else {
        LOG_ERROR("Error deleting file %s: %s", target, strerror(errno));
        return -1;
    }
}