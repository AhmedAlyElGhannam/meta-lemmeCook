#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

/* gpio base address + relevant registers offsets */
#define GPIO_BASE       (0x3F200000UL)
#define GPIO_GPFSEL0    (0x00)
#define GPIO_GPFSEL1    (0x04)
#define GPIO_GPFSEL2    (0x08)
#define GPIO_GPSET0     (0x1C)
#define GPIO_GPCLR0     (0x28)

/* size of gpio (a fancy way to say the number of registers * 4bytes) */
#define GPIO_SIZE	    (0xB4)

/* number of used gpio pins */
#define NUM_OF_GPIOS    (2)

/* read/write permissions */
#define PERM_RO 1U
#define PERM_WO 2U
#define PERM_RW 3U

/* maximum number of devices supported by this driver */
#define NUM_OF_ENTRIES  5 // max num of devices this driver is able to operate

/* name of this driver (will be used when probe is called to match device with driver) */
#define DRIVER_NAME "_led_gpio"

/* gpio pins connected to LEDs */
#define LED_AQUA_PIN    21
#define LED_CRIMSON_PIN 26

/* gpio buffer size */
#define GPIO_BUF_SIZE   15


struct GPIO 
{
    u32 buffSize; /* buffer size */
    void* __iomem GPSEL; /* function select register address */
    void* __iomem GPSET; /* value set register address */
    void* __iomem GPCLR; /* value clear register address */
    u32 permissions; /* device permissions when using this gpio pins */
    void* allocatedMem; /* pointer to buffer */
};

/* driver private data */
struct platform_driver_private_data
{
    dev_t devNum; /* major + base minor */
    struct class* _class; /* driver class in sys */
    void __iomem* gpio_base; /* base address for gpio */
    int numOfEntries; /* device count */
};  

/* device private data */
struct platform_device_private_data
{
    struct cdev _cdev;      /* logical representation of character devices */
    struct device* _device; /* physical representation of devices (in general) in Linux */
    struct GPIO _gpio;      /* hardware-level info about gpio (registers + pins) */
    u32 pin_num;            /* gpio pin */
};

#endif