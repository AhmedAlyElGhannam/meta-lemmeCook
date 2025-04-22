#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include "gpio_driver.h"

MODULE_LICENSE("GPL"); // has to be specified to allow using GPL-licensed code in kernel
MODULE_AUTHOR("Nemesis"); // this is my gaming alias
MODULE_DESCRIPTION("Platform device for LEDs connected to GPIO (Raspberry Pi 3B+)"); // module description has to be clear and meme-free
MODULE_VERSION("1.0.0"); // module version based on development

/* all function prototypes */
static int __init gpioDev_init(void);
static void __exit gpioDev_exit(void);
void releaseCBF(struct device* dev);

struct platform_device _platdevice[NUM_OF_GPIOS] = 
{
    [0] = {
        .name = "LED_AQUA",
        .id = 0,
        .dev = { .release = releaseCBF }
    },
    [1] = {
        .name = "LED_CRIMSON",
        .id = 1,
        .dev =  { .release = releaseCBF }
    }
};

void releaseCBF(struct device* dev)
{

}

static int __init gpioDev_init(void)
{
    u32 iter;
    u16 res = 0;
    for (iter = 0; iter < NUM_OF_GPIOS; iter++)
    {
        res = platform_device_register(&_platdevice[iter]);
        if (res < 0)
        {
            printk("Failed to register plaform device number %d\n", iter);
            break;
        }
    }

    return res;
}

static void __exit gpioDev_exit(void)
{
    u32 iter;
    for (iter = 0; iter < NUM_OF_GPIOS; iter++)
    {
        platform_device_unregister(&_platdevice[iter]);
    }
}

module_init(gpioDev_init); 
module_exit(gpioDev_exit); 