#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"
#include "port.h"
#include "gdt.h"

class InterruptManager {
protected:
    struct GateDescriptor {
        uint16_t handlerAdressLowBits;
        uint16_t gdt_codeSegmentSelector;
        uint8_t reseved;
        uint8_t access;
        uint16_t handlerAdressHighBits;
    } __attribute__((packed));

    static GateDescriptor interruptDescriptorTable[256];

    struct InterruptDescriptorTablePointer {
        uint16_t size;
        uint16_t base;
    }__attribute__((packed));

    static void setInterruptDescriptorTableEntry(uint8_t interruptNumber, uint16_t codeSegmentSelectorOffset,
    void (*handler)(), uint8_t DescriptorPriviLegeLever, uint8_t DescriptorType);

    Port8BitSlow picMasterCommand;
    Port8BitSlow picMasterData;
    Port8BitSlow picSlaveCommand;
    Port8BitSlow picSlaveData;

public:
    InterruptManager(GlobalDescriptorTable* gdt);
    ~InterruptManager();

    void Activate();

    static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);

    static void IgnoreInterruptRequest();
    static void HandleInterruptRequest0x00();
    static void HandleInterruptRequest0x01();

};

#endif