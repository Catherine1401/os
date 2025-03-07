#include "interrupts.h"

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

void printf(const char* str);

void InterruptManager::setInterruptDescriptorTableEntry(uint8_t interruptNumber, uint16_t codeSegmentSelectorOffset,
    void (*handler)(), uint8_t descriptorPriviLegeLever, uint8_t descriptorType) {
        const uint8_t IDT_DESC_PRESENT = 0x00;

        interruptDescriptorTable[interruptNumber].handlerAdressLowBits = ((uint32_t)handler) & 0xFFFF;
        interruptDescriptorTable[interruptNumber].handlerAdressHighBits = (((uint32_t)handler) >> 16) & 0xFFFF;
        interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
        interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | descriptorType | ((descriptorPriviLegeLever & 3) << 5);
        interruptDescriptorTable[interruptNumber].reseved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt) 
: picMasterCommand(0x20), picMasterData(0x21), picSlaveCommand(0xA0), picSlaveData(0xA1) {
    uint16_t CodeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for (uint16_t i = 0; i < 256; i++)
        setInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    
    setInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

    picMasterCommand.Write(0x11);
    picSlaveCommand.Write(0x11);

    picMasterData.Write(0x20);
    picSlaveData.Write(0x28);

    picMasterData.Write(0x04);
    picSlaveData.Write(0x02);

    picMasterData.Write(0x01);
    picSlaveData.Write(0x01);

    picMasterData.Write(0x00);
    picSlaveData.Write(0x00);

    InterruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0": : "m" (idt));

}

InterruptManager::~InterruptManager() {}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp) {
    printf(" INTERRUPT");
    return esp;
}

void InterruptManager::Activate() {
    asm("sti");
}

void InterruptManager::IgnoreInterruptRequest() {
    // Chỉ đơn giản bỏ qua interrupt
}

extern "C" void InterruptManager::HandleInterruptRequest0x00() {
    handleInterrupt(0x20, 0);
}

extern "C" void InterruptManager::HandleInterruptRequest0x01() {
    handleInterrupt(0x21, 0);
}
