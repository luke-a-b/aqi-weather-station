#ifndef _MEM_CHECKER_H
#define _MEM_CHECKER_H

#include <Arduino.h>

#define UPDATE_INTERVAL 60000

typedef struct memory_stat_t {
    uint32_t freeHeap;
    uint16_t maxFreeBlock;
    uint8_t frag;
    uint32_t freeContStack;
} memory_stat_t;


class MemChecker
{
public:
    void update(uint32_t now)
    {
        if (now - lastUpdate > 60000)
        {
            lastUpdate = now;

            memory_stat_t memoryStats;
            ESP.getHeapStats(&memoryStats.freeHeap, &memoryStats.maxFreeBlock, &memoryStats.frag);

            if (memoryStats.freeHeap > memoryStatsMax.freeHeap)             memoryStatsMax.freeHeap  = memoryStats.freeHeap;
            if (memoryStats.maxFreeBlock > memoryStatsMax.maxFreeBlock)     memoryStatsMax.maxFreeBlock  = memoryStats.maxFreeBlock;
            if (memoryStats.frag > memoryStatsMax.frag)                     memoryStatsMax.frag  = memoryStats.frag;

            if (memoryStats.freeHeap < memoryStatsMin.freeHeap)             memoryStatsMin.freeHeap  = memoryStats.freeHeap;
            if (memoryStats.maxFreeBlock < memoryStatsMin.maxFreeBlock)     memoryStatsMin.maxFreeBlock  = memoryStats.maxFreeBlock;
            if (memoryStats.frag < memoryStatsMin.frag)                     memoryStatsMin.frag  = memoryStats.frag;

            uint32_t cont = ESP.getFreeContStack();
            if (cont > memoryStatsMax.freeContStack)                     memoryStatsMax.freeContStack  = cont;
            if (cont < memoryStatsMin.freeContStack)                     memoryStatsMin.freeContStack  = cont;

            Serial.println(F("Memory stats: "));
            Serial.print(F("Free heap (min/max): \t"));Serial.print(memoryStatsMin.freeHeap);Serial.print("/");Serial.println(memoryStatsMax.freeHeap);
            Serial.print(F("Max free block (min/max): \t"));Serial.print(memoryStatsMin.maxFreeBlock);Serial.print("/");Serial.println(memoryStatsMax.maxFreeBlock);
            Serial.print(F("Fragmentation (min/max): \t"));Serial.print(memoryStatsMin.frag);Serial.print("/");Serial.println(memoryStatsMax.frag);
            Serial.print(F("Free cont stack (min/max): \t"));Serial.print(memoryStatsMin.freeContStack);Serial.print("/");Serial.println(memoryStatsMax.freeContStack);
        }
    }
private:
    int32_t lastUpdate = -UPDATE_INTERVAL;
    memory_stat_t memoryStatsMin = memory_stat_t{UINT32_MAX, UINT16_MAX, UINT8_MAX, UINT32_MAX};
    memory_stat_t memoryStatsMax = memory_stat_t{0, 0, 0, 0};
};

#endif
