#ifndef GDT_H
#define GDT_H

#include "types.h"

class GlobalDescriptorTable {
public:
    GlobalDescriptorTable();
    ~GlobalDescriptorTable();

    uint16_t CodeSegmentDescriptor();
    uint16_t DataSegmentDescriptor();

    class SegmentDescriptor {
    public:
        SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flags);
        uint32_t Base();
        uint32_t Limit();
    private:
        uint16_t limitLow;
        uint8_t flagsLimitHigh;
        uint16_t baseLow;
        uint8_t baseHigh;
        uint8_t type;
    } __attribute__((packed));

    SegmentDescriptor nullSegmentSelector;
    SegmentDescriptor unusedSegmentSelector;
    SegmentDescriptor codeSegmentSelector;
    SegmentDescriptor dataSegmentSelector;
};

#endif