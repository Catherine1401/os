#include "types.h"
#include "gdt.h"
#include "interrupts.h"

void printf(const int8_t *str) {
    static uint16_t* videoMemory = (uint16_t*) 0xb8000;
    static uint8_t row = 0, column = 0;

    uint16_t lastSpace = 0;
    uint16_t lastSpaceIndex = 0;

    for (uint16_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            row++;
            column = 0;
            continue;
        }

        if (str[i] == ' ') {
            lastSpace = column;
            lastSpaceIndex = i;
        }

        if (column == 80) {
            if (lastSpace > 0) {
                for (uint16_t j = lastSpace; j < 80; j++) {
                    videoMemory[80 * row + j] = (videoMemory[80 * row + j] & 0xFF00) | ' ';
                }
                i = lastSpaceIndex;
                column = lastSpace;
            }
            row++;
            column = 0;
            lastSpace = 0;
            continue;
        }

        // Khi đến dòng 25, thực hiện cuộn màn hình lên 1 dòng
        if (row == 25) {
            for (uint16_t y = 1; y < 25; y++) {
                for (uint16_t x = 0; x < 80; x++) {
                    videoMemory[80 * (y - 1) + x] = videoMemory[80 * y + x];
                }
            }
            // Xóa dòng cuối cùng
            for (uint16_t x = 0; x < 80; x++) {
                videoMemory[80 * 24 + x] = (videoMemory[80 * 24 + x] & 0xFF00) | (' ' & 0xFF00);
            }
            row = 24;  // Ghi tiếp vào dòng cuối cùng
        }

        videoMemory[80 * row + column] = (videoMemory[80 * row + column] & 0xFF00) | (str[i] & 0xFF);
        column++;
    }
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void call_constructors() {
    for (constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

extern "C" void mainn(void *multiboot_structure, uint32_t magicnumber) {
    printf("hello world!\n hehe\nhealjfewlef\nlihwfhelf");
    printf("hehee");
    printf("On Tuesday, British Prime Minister Keir Starmer pointedly paid tribute in the House of Commons to British soldiers who were killed fighting alongside the United States in Iraq and Afghanistan. This followed a comment by Vance on Fox News that Ukraine needed better security guarantees than undertakings from “some random country that has not fought a war in 30 or 40 years.” The vice president’s remark caused great offensive in Britain. He said on X that claims he’d been talking about Britain and France were “absurdly dishonest,” but those are the only allies who have so far publicly volunteered to take part in a post-peace deal reassurance force in Ukraine.On Tuesday, British Prime Minister Keir Starmer pointedly paid tribute in the House of Commons to British soldiers who were killed fighting alongside the United States in Iraq and Afghanistan. This followed a comment by Vance on Fox News that Ukraine needed better security guarantees than undertakings from “some random country that has not fought a war in 30 or 40 years.” The vice president’s remark caused great offensive in Britain. He said on X that claims he’d been talking about Britain and France were “absurdly dishonest,” but those are the only allies who have so far publicly volunteered to take part in a post-peace deal reassurance force in Ukraine.On Tuesday, British Prime Minister Keir Starmer pointedly paid tribute in the House of Commons to British soldiers who were killed fighting alongside the United States in Iraq and Afghanistan. This followed a comment by Vance on Fox News that Ukraine needed better security guarantees than undertakings from “some random country that has not fought a war in 30 or 40 years.” The vice president’s remark caused great offensive in Britain. He said on X that claims he’d been talking about Britain and France were “absurdly dishonest,” but those are the only allies who have so far publicly volunteered to take part in a post-peace deal reassurance force in Ukraine.On Tuesday, British Prime Minister Keir Starmer pointedly paid tribute in the House of Commons to British soldiers who were killed fighting alongside the United States in Iraq and Afghanistan. This followed a comment by Vance on Fox News that Ukraine needed better security guarantees than undertakings from “some random country that has not fought a war in 30 or 40 years.” The vice president’s remark caused great offensive in Britain. He said on X that claims he’d been talking about Britain and France were “absurdly dishonest,” but those are the only allies who have so far publicly volunteered to take part in a post-peace deal reassurance force in Ukraine.On Tuesday, British Prime Minister Keir Starmer pointedly paid tribute in the House of Commons to British soldiers who were killed fighting alongside the United States in Iraq and Afghanistan. This followed a comment by Vance on Fox News that Ukraine needed better security guarantees than undertakings from “some random country that has not fought a war in 30 or 40 years.” The vice president’s remark caused great offensive in Britain. He said on X that claims he’d been talking about Britain and France were “absurdly dishonest,” but those are the only allies who have so far publicly volunteered to take part in a post-peace deal reassurance force in Ukraine.");
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(&gdt);

    interrupts.Activate();
    while (true);
}