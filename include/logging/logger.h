#pragma once

struct Logger;

struct LoggerInterface
{
    int (*init)(struct Logger* self, void* data);
    void (*close)(struct Logger* self);
    void (*info)(struct Logger* self, const char* fmt, ...);
    void (*error)(struct Logger* self, const char* fmt, ...);
};

struct Logger
{
    struct LoggerInterface interface;
    void* data;
};
