#ifndef PORT_H
#define PORT_H

#include "types.h"

class Port {
    protected:
        uint16_t portNumber;
    
    public:
        Port(uint16_t portNum) : portNumber(portNum) {}
        virtual ~Port() {}
    };
    
    // Template cho cổng I/O với kích thước bất kỳ
    template <typename T>
    class PortT : public Port {
    public:
        PortT(uint16_t portNum) : Port(portNum) {}
        ~PortT() {}
    
        void Write(T data) {
            if constexpr (sizeof(T) == 1)
                __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(portNumber));
            else if constexpr (sizeof(T) == 2)
                __asm__ volatile("outw %0, %1" : : "a"(data), "Nd"(portNumber));
            else if constexpr (sizeof(T) == 4)
                __asm__ volatile("outl %0, %1" : : "a"(data), "Nd"(portNumber));
        }
    
        T Read() {
            T result;
            if constexpr (sizeof(T) == 1)
                __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(portNumber));
            else if constexpr (sizeof(T) == 2)
                __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(portNumber));
            else if constexpr (sizeof(T) == 4)
                __asm__ volatile("inl %1, %0" : "=a"(result) : "Nd"(portNumber));
            return result;
        }
    };
    
    // Định nghĩa loại cổng cụ thể
    using Port8Bit = PortT<uint8_t>;
    using Port16Bit = PortT<uint16_t>;
    using Port32Bit = PortT<uint32_t>;
    

#endif