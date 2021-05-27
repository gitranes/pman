#include "common/file.h"

#include <fcntl.h>
#include <unistd.h>

#include <stdlib.h>

FILE* file_hidden_temp(const char* directory_path, const char* mode)
{
    FILE* fp = NULL;
    const int permissions = S_IRUSR | S_IWUSR; // NOLINT(hicpp-signed-bitwise)
    const int o_flags = O_TMPFILE | O_EXCL | O_RDWR; // NOLINT(hicpp-signed-bitwise)

    // Unlinked and unnamed temporary file with 0600 permissions
    const int tmp_file_fd = open(directory_path, o_flags, permissions);
    if (tmp_file_fd == -1)
    {
        return NULL;
    }
    if (!(fp = fdopen(tmp_file_fd, mode)))
    {
        close(tmp_file_fd);
        return NULL;
    }
    return fp;
}

long file_size(FILE* fp)
{
    fseek(fp, 0, SEEK_END);
    const long file_size = ftell(fp);

    rewind(fp);
    return file_size;
}

struct ByteView file_dump_contents(FILE* fp)
{
    struct ByteView result = {0};
    const long dump_size = file_size(fp);

    result.buf = malloc(dump_size + 1);
    result.size = dump_size;
    fread(result.buf, 1, dump_size, fp);
    return result;
}
