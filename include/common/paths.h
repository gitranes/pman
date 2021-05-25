#pragma once

#include <stdbool.h>

extern const char PATH_SEP;

extern const char* const PATH_CONFIG_FILE;
extern const char* const PATH_DB_FILE_EXT;

// Unix has 4096, but pman has a little more sensible limit
extern const int PATH_FILENAME_MAX;

/**
 * Concatenates two paths components. The result needs to be freed.
 * @param left
 * @param right
 * @return Concatenated path
 */
char* path_concatenate(const char* restrict left, const char* restrict right);

/**
 * @param path - non-null path
 * @return T/F
 */
bool path_exists(const char* path);


/**
 * Check that the passed param is a valid filename eg. regex [\w\d_\-.]
 * @param filename
 * @return T/F
 */
bool path_valid_filename(const char* filename);

/**
 * Check that the passed param is a valid path eg. regex [\\\w\d_\-.]
 * @param path
 * @return T/F
 */
bool path_valid_path(const char* path);
