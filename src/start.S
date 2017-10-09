/*
 * start.S
 *
 *  Created on: 2017年9月28日
 *      Author: lucius
 */

#define GPIO_BASE (0xC0010000)
#define GPIOx_BASE(x) (GPIO_BASE + 0xA000 +0x1000*(x))
#define GPIOA_BASE GPIOx_BASE(0)
#define GPIOB_BASE GPIOx_BASE(1)
#define GPIOC_BASE GPIOx_BASE(2)
#define GPIOD_BASE GPIOx_BASE(3)
#define GPIOE_BASE GPIOx_BASE(4)

#define GPIOxOUT        0x0
#define GPIOxOUTENB     0x4
#define GPIOxALTFN0     0x20

#define PHY_BASEADDR_INTC_MODULE                            0xC0008000


#define PE_MODE_USER        0x10
#define PE_MODE_FIQ         0x11
#define PE_MODE_IRQ         0x12
#define PE_MODE_SVC         0x13
#define PE_MODE_MON         0x16
#define PE_MODE_ABT         0x17
#define PE_MODE_HYP         0x1A
#define PE_MODE_UND         0x1B
#define PE_MODE_SYS         0x1F


#define CPSR_IRQ            (1 << 7)
#define CPSR_FIQ            (1 << 6)

.global Vectors
Vectors:
    ldr     pc, ResetV          /* reset */
    ldr     pc, UndefV          /* undefined */
    ldr     pc, SWIV            /* software interrupt */
    ldr     pc, PAbortV         /* prefech abort */
    ldr     pc, DAbortV         /* data abort */
    ldr     pc, UnusedV         /* unused */
    ldr     pc, IRQV            /* irq */
    ldr     pc, FIQV            /* fiq */

ResetV:
    .word reset_handler
UndefV:
    .word panic
SWIV:
    .word panic
PAbortV:
    .word panic
DAbortV:
    .word panic
UnusedV:
    .word panic
IRQV:
    .word panic
FIQV:
    .word panic

DeviceReadAddr:
    .word 0x200
LOAD_SIZE:
    .word __image_size
LOAD_ADDR:
    .word 0xFFFF0000
LAUNCH_ADDR:
    .word 0xFFFF0000


reset_handler:
    msr     CPSR_c, #(PE_MODE_SVC | CPSR_IRQ | CPSR_FIQ)

    /* get clustor cpu id */
    mrc     p15, 0, r12, c0, c0, 5
    ubfx    r11, r12, #8, #8        /* get clustor id*/
    orr     r12, r12, r11           /* concat two id*/
    and     r12, r12, #0xF

    mov     r0, r12
    bl      setup_stack

    cmp     r12, #0
    bne     _sub_cpu_main

    bl      clr_bss
    mov     r0, r12
    bl      boot_main
    b       panic
    b       .

_sub_cpu_main:
    mov     r0, r12
    bl      sub_cpu_main
    b       panic
    b       .

    /* setup stack */

/* void setup_stack(uint32_t cpuid)  */
/*
high memery
cpu1 2k
cpu2 2k
...
cpu7 2k
cpu0 2k
*/
setup_stack:
    cmp     r0, #0
    moveq   r0, #7
    subne   r0, r0, #1
    mov     r0, r0, lsl #11     /* r0=r0*2k*/
    ldr     r1, _stack_end
    sub     r1, r1, r0          /* get stack base */

    mrs     r2, cpsr            /* store current mode */

    sub     r1, r1, #256        /* reserve stack space for abort, undefined */

    msr     cpsr_c, #(PE_MODE_FIQ | CPSR_IRQ | CPSR_FIQ)
    mov     sp, r1
    sub     r1, r1, #256

    msr     cpsr_c, #(PE_MODE_IRQ | CPSR_IRQ | CPSR_FIQ)
    mov     sp, r1
    sub     r1, r1, #512

    /* disable irq, fiq, change to svc mode */
    msr     cpsr_c, #(PE_MODE_SVC | CPSR_IRQ | CPSR_FIQ)
    mov     sp, r1

    msr     cpsr, r2            /* restore mode */

    mov     pc, lr
    b       . /*just for safe*/

clr_bss:
    /* clear bss */
    ldr     r0, _bss_start
    ldr     r1, _bss_end
    mov     r2, #0
clr_bss_loop:
    cmp     r0, r1
    strlo   r2, [r0], #4
    bne     clr_bss_loop

    mov     pc, lr

panic:
    /* init gpio, should only do once */
    movw    r0, #(GPIOC_BASE & 0xFFFF)
    movt    r0, #(GPIOC_BASE >> 16)

    // set GPIOC12 as ALTFN1
    ldr     r1, [r0, #GPIOxALTFN0]
    bic     r1, r1, #(3 << 24)
    orr     r1, r1, #(1 << 24)
    str     r1, [r0, #GPIOxALTFN0]
    // set GPIOC12 as output mode
    ldr     r1, [r0, #GPIOxOUTENB]
    orr     r1, r1, #(1 << 12)
    str     r1, [r0, #GPIOxOUTENB]

    // set GPIOC12 as low level (means turn on blue led on board)
    ldr     r1, [r0, #GPIOxOUT]
    bic     r1, r1, #(1 << 12)
    str     r1, [r0, #GPIOxOUT]
    b       .

stub:
    .space  0x1fc - (stub - Vectors)
signature:
    .word   (('N' << 0) | ('S' << 8) | ('I' << 16) | ('H' << 24))

.global _bss_start, _bss_end, _stack_start, _stack_end
_bss_start:
    .word   __bss_start
_bss_end:
    .word   __bss_end
_stack_start:
    .word   __stack_start
_stack_end:
    .word   __stack_end
