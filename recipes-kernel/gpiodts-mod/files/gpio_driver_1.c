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

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nemesis");
MODULE_DESCRIPTION("A module that does X on RPi3B+");
MODULE_VERSION("1.0.0");

static int __init gpioDriv_init(void);
static void __exit gpioDriv_exit(void);
static int gpio_open(struct inode *inode, struct file *file);
static ssize_t gpio_write(struct file *file, const char __user *buf, size_t len, loff_t *off);
int probeCBF(struct platform_device* platResource);
int removeCBF(struct platform_device* platResource);

static struct platform_driver_private_data* _prvtDrvData;

static const struct file_operations gpio_fops = {
    .owner = THIS_MODULE,
    .open = gpio_open,
    .write = gpio_write,
};

const struct platform_device_id _id_table[NUM_OF_GPIOS] = {
    [0] = { .name = "LED_AQUA" },
    [1] = { .name = "LED_CRIMSON" }
};

static int gpio_open(struct inode *inode, struct file *file) {
    return 0;
}

static ssize_t gpio_write(struct file* file, const char __user* user_buff, size_t len, loff_t* off) {

    return len;
}

int probeCBF(struct platform_device* platResource) {
    printk("am here in probe\n");
    return 0;
}

int removeCBF(struct platform_device* platResource) {
    printk("Device Removed!\n");
    return 0;
}

struct myprivateData {
    int a;
    int b;
};
struct myprivateData element;

const struct of_device_id mydeviceId = {
    .compatible = "led-aqua",
    .data = &element,
    .name = "my led device",
    .type = "character"
};

struct platform_driver _platDriver = {
    .probe = probeCBF,
    .remove = removeCBF,
    .driver = {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE,
        .of_match_table = &mydeviceId
    }
};

static int __init gpioDriv_init(void) {
    int res = 0;

    _prvtDrvData = kzalloc(sizeof(struct platform_driver_private_data), GFP_KERNEL);
    if (!_prvtDrvData)
        return -ENOMEM;

    res = alloc_chrdev_region(&_prvtDrvData->devNum, 0, NUM_OF_ENTRIES, DRIVER_NAME);
    if (res < 0)
        goto err_chrdev;

    _prvtDrvData->_class = class_create(THIS_MODULE, DRIVER_NAME);
    if (IS_ERR(_prvtDrvData->_class)) {
        res = PTR_ERR(_prvtDrvData->_class);
        goto err_class;
    }

    _prvtDrvData->gpio_base = ioremap(GPIO_BASE, GPIO_SIZE);
    if (!_prvtDrvData->gpio_base) {
        res = -ENOMEM;
        goto err_ioremap;
    }

    res = platform_driver_register(&_platDriver);
    if (res < 0)
        goto err_platform;

    return 0;

err_platform:
    iounmap(_prvtDrvData->gpio_base);
err_ioremap:
    class_destroy(_prvtDrvData->_class);
err_class:
    unregister_chrdev_region(_prvtDrvData->devNum, NUM_OF_ENTRIES);
err_chrdev:
    kfree(_prvtDrvData);
    return res;
}

static void __exit gpioDriv_exit(void) {
    platform_driver_unregister(&_platDriver);
    iounmap(_prvtDrvData->gpio_base);
    class_destroy(_prvtDrvData->_class);
    unregister_chrdev_region(_prvtDrvData->devNum, NUM_OF_ENTRIES);
    kfree(_prvtDrvData);
}

module_init(gpioDriv_init);
module_exit(gpioDriv_exit);
