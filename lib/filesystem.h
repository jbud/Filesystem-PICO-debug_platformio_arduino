#include <Arduino.h>

#include "FlashIAPBlockDevice.h"
#include "LittleFileSystem.h"
#include <mbed.h>

#ifndef FS_BASE
    #define FS_BASE "fs"
#endif

// 1MB partition, starting 1MB into the flash
FlashIAPBlockDevice bd(XIP_BASE + 0x100000, 0x100000);
mbed::LittleFileSystem fs(FS_BASE);

char filename[] = FS_BASE "/ksdm.dat";
char logname[] = FS_BASE "/ksdm.log";

typedef char kbyte;

long fileSize(const char * path);

void dumpLog();

void appendFile(const char * path, const char * message, size_t messageSize);

void writeFile(const char * path, const char * message, size_t messageSize);

kbyte readFile(const char * path);

bool fileExists(const char * path);