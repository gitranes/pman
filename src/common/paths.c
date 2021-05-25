#include "common/paths.h"
#include "common/string.h"

#include <unistd.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char PATH_SEP = '/';

const char* const PATH_CONFIG_FILE = "./pman.ini";
const char* const PATH_DB_FILE_EXT = ".db";

const int PATH_FILENAME_MAX = 256;

inline static bool valid_filename_char(const char ch)
{
    return isalnum(ch) || ch == '.' || ch == '-' || ch == '_';
}

inline static bool valid_path_char(const char ch)
{
    return valid_filename_char(ch) || ch == '/';
}

char* path_concatenate(const char* left, const char* right)
{
    const size_t left_size = strlen(left);
    const size_t right_size = strlen(right) + 1;    // path + null
    const size_t path_sep_size = sizeof(PATH_SEP);
    const size_t new_path_size = left_size + path_sep_size + right_size;

    char* new_path = malloc(new_path_size);

    memcpy(new_path, left, left_size);
    new_path[left_size] = PATH_SEP;
    memcpy(new_path + left_size + path_sep_size, right, right_size - 1);
    new_path[new_path_size - 1] = '\0';

    return new_path;
}

bool path_exists(const char* path)
{
    return access(path, F_OK) == 0;
}

bool path_valid_filename(const char* filename)
{
    return string_check_chars(filename, valid_filename_char);
}

bool path_valid_path(const char* path)
{
    return string_check_chars(path, valid_path_char);
}
