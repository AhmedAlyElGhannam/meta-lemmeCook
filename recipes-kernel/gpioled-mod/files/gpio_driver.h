#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#define GPIO_BASE   (0x3F200000UL)
#define GPIO_GPFSEL0 (0x00)
#define GPIO_GPFSEL1 (0x04)
#define GPIO_GPFSEL2 (0x08)
#define GPIO_GPSET0 (0x1C)
#define GPIO_GPCLR0 (0x28)
#define GPIO_SIZE	    (0xA0)
#define NUM_OF_GPIOS 2

#define PERM_RO 1U
#define PERM_WO 2U
#define PERM_RW 3U

#define NUM_OF_ENTRIES  5 // max num of devices this driver is able to operate

#define DRIVER_NAME "_led_gpio"


struct GPIO 
{
    u32 buffSize;
    u32 base_reg;
    void __iomem* GPSEL;
    void __iomem* GPSET;
    void __iomem* GPCLR;
    u32 permissions;
    u32 pin_num;
    void* allocatedMem;
};

struct platform_private_device 
{
    struct cdev _cdev;
    struct device* _device;
};

#endif
