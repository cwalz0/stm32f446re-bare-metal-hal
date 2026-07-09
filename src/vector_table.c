#include <stdint.h>

extern uint32_t _etext;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _sdata;
extern uint32_t _edata;

#define SRAM_START ( 0x20000000 )
#define SRAM_SIZE ( 128 * 1024 )
#define SRAM_END ( SRAM_START + SRAM_SIZE )

extern int main(void);

void Reset_Handler(void) {
    uint32_t *p_source = &_etext;
    uint32_t *p_dest = &_sdata;

    while (p_dest < &_edata){
        *p_dest = *p_source;
        p_dest++;
        p_source++;
    }
    
    uint32_t *p_bss = &_sbss;
    while (p_bss < &_ebss) {
        *p_bss = 0;
        p_bss++;
    }
    main();
}

void Default_Handler(void) {
    while (1) {
        //infinite loop for debugger trapping
    }
}

void NMI_Handler(void)          __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)      __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void SVCall_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void DebugMonitor_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)        __attribute__((weak, alias("Default_Handler")));

void *vector_table[] __attribute__((section(".isr_vector"))) = {
    (void *) SRAM_END,                  // 0
    (void *) &Reset_Handler,            // 1
    (void *) &NMI_Handler,              // 2
    (void *) 0,                         // 3
    (void *) 0,                         // 4
    (void *) 0,                         // 5
    (void *) 0,                         // 6
    (void *) &HardFault_Handler,        // 7
    (void *) &MemManage_Handler,        // 8
    (void *) &BusFault_Handler,         // 9
    (void *) &UsageFault_Handler,       // 10
    (void *) 0,                         // 11
    (void *) &SVCall_Handler,           // 12
    (void *) &DebugMonitor_Handler,     // 13
    (void *) 0,                         // 14
    (void *) &PendSV_Handler,           // 15
};

