#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/mod_devicetable.h>
#include "gpio_driver.h"

static void platform_device_release(struct device *dev)
{
    /* Managed resources are automatically freed */
}

static struct GPIO _gpios[2] = {
    [0] = {
        .buffSize = 15,
        .permissions = PERM_RW,
        .pin_num = 21
    },
    [1] = {
        .buffSize = 15,
        .permissions = PERM_RW,
        .pin_num = 26
    }
};

static struct platform_device led_platform_devices[2] = {
    [0]= {
        .name = "LED_AQUA",
        .id = 0,
        .dev = {
            .release = platform_device_release,
            .driver_data = &_gpios[0],
        },
    },
    [1]= {
        .name = "LED_CRIMSON",
        .id = 1,
        .dev = {
            .release = platform_device_release,
            .driver_data = &_gpios[1],
        },
    }
};

static int __init led_devices_init(void)
{
    int ret;

    ret = platform_device_register(&led_platform_devices[0]);
    if (ret)
        return ret;

    ret = platform_device_register(&led_platform_devices[1]);
    if (ret) {
        platform_device_unregister(&led_platform_devices[0]);
        return ret;
    }

    return 0;
}

static void __exit led_devices_exit(void)
{
    platform_device_unregister(&led_platform_devices[0]);
    platform_device_unregister(&led_platform_devices[1]);
}

module_init(led_devices_init);
module_exit(led_devices_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nemesis");
MODULE_DESCRIPTION("Platform device definitions for GPIO LEDs on RPi3B+");

