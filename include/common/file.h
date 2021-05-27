#pragma once

#include "common/buffer_view.h"

#include <stdio.h>

/**
 * Creates an unlinked and unnamed temporary file
 * @param directory_path - directory to create the temp file in
 * @return
 */
FILE* file_hidden_temp(const char* directory_path, const char* mode);

/**
 * Seek from fp to end and back, and return the size.
 * @param fp
 * @return Size of the file starting from fp
 */
long file_size(FILE* fp);


/**
 * Dump all of the data from start of fp to the end of the file
 * @param fp
 * @return
 */
struct ByteView file_dump_contents(FILE* fp);
