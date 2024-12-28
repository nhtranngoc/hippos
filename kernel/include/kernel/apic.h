#ifndef KERNEL_APIC_H
#define KERNEL_APIC_H

// Used to enable/disable PIC
#define PIC_MASTER		            0x20		/* IO base address for master PIC */
#define PIC_SLAVE		            0xA0		/* IO base address for slave PIC */
#define PIC_MASTER_COMMAND	        PIC_MASTER
#define PIC_MASTER_DATA	            (PIC_MASTER + 1)
#define PIC_SLAVE_COMMAND	        PIC_SLAVE
#define PIC_SLAVE_DATA	            (PIC_SLAVE + 1)
#define PIC_ICW1                    0x11
#define PIC_ICW2_MASTER             0x20
#define PIC_ICW2_SLAVE              0x28
#define PIC_ICW3_MASTER             0x2
#define PIC_ICW3_SLAVE              0x4
#define PIC_ICW4                    0x01        // 8086 Mode

// MSR APIC Specifics
#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE 0x800

void apic_initialize(void);

#endif // KERNEL_APIC_H