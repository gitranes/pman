#pragma once

struct Logger;

struct LoggerInterface
{
    int (*init)(struct Logger* self, void* data);
    void (*close)(struct Logger* self);
    void (*info)(const struct Logger* self, const char* fmt, ...);
    void (*error)(const struct Logger* self, const char* fmt, ...);
};

struct Logger
{
    struct LoggerInterface interface;
    void* data;
};
