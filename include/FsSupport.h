#ifndef _FS_SUPPORT_H
#define _FS_SUPPORT_H

#include <LittleFS.h>

class FsSupport
{
public:
    static uint32_t listFiles(String dirName = "/", uint8_t depth = 0);
};

#endif