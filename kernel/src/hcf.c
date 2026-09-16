#include <hcf.h>

__attribute__((noreturn))
void hcf(void)
{
    for (;;) {
        __asm__ volatile ("cli");
        __asm__ volatile ("hlt");
    }
}