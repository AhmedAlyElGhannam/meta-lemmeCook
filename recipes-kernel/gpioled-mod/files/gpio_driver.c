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

static const struct file_operations gpio_fops = 
{
    .owner = THIS_MODULE,
    .open = gpio_open,
    .write = gpio_write,
};

struct platform_driver_private_data
{
    dev_t devNum;
    struct class* _class;
    int numOfEntries;
    void __iomem* gpio_base;
};

struct platform_device_private_data
{
    struct cdev _cdev;
    struct device* _device;
    struct GPIO _gpio;
};

static struct platform_driver_private_data _prvtDrvData;

const struct platform_device_id _id_table[NUM_OF_GPIOS] = 
{
    [0] = { .name = "LED_AQUA" },
    [1] = { .name = "LED_CRIMSON" }
};

static int gpio_open(struct inode *inode, struct file *file) 
{
    struct platform_device_private_data* priv = container_of(inode->i_cdev, struct platform_device_private_data, _cdev);
    file->private_data = priv;
    printk("Open CBF for LED GPIO device/file number %d\n", priv->_gpio.pin_num);
    return 0;
}

static ssize_t gpio_write(struct file *file, const char __user *user, size_t size, loff_t *offst) {
    struct platform_device_private_data *priv = file->private_data;
    char buff[10] = {0};
    int ret = 0;

    printk(KERN_INFO "Write callback entered for GPIO%d\n", priv->_gpio.pin_num);

    if (!priv) {
        printk(KERN_ERR "No private data!\n");
        return -EFAULT;
    }

    if (size == 0 || size > sizeof(buff) - 1)
        size = sizeof(buff) - 1;

    if (copy_from_user(buff, user, size)) {
        printk(KERN_ERR "Failed to copy from user\n");
        return -EFAULT;
    }

    if (buff[size - 1] == '\n')
        buff[size - 1] = '\0';

    printk(KERN_INFO "Command received: %c for GPIO%d\n", buff[0], priv->_gpio.pin_num);

    if (buff[0] == '1') {
        printk(KERN_INFO "Setting GPIO%d\n", priv->_gpio.pin_num);
        iowrite32(1 << priv->_gpio.pin_num, priv->_gpio.GPSET);
    } else if (buff[0] == '0') {
        printk(KERN_INFO "Clearing GPIO%d\n", priv->_gpio.pin_num);
        iowrite32(1 << priv->_gpio.pin_num, priv->_gpio.GPCLR);
    } else {
        printk(KERN_WARNING "Invalid command: %c\n", buff[0]);
        ret = -EINVAL;
    }

    return ret ? ret : size;
}

int probeCBF (struct platform_device* platResource)
{
    int res = 0;
    uint32_t val;
    uint32_t mask_shift;
    struct platform_device_private_data* priv_dev;

    printk("Device Detected! \n");

    priv_dev = devm_kzalloc(&platResource->dev, sizeof(*priv_dev), GFP_KERNEL);
    if (!priv_dev) {
        res = -ENOMEM;
        goto out;
    }

    platform_set_drvdata(platResource, priv_dev);

    priv_dev->_gpio = *(struct GPIO*)platResource->dev.driver_data;
    priv_dev->_gpio.allocatedMem = kzalloc(priv_dev->_gpio.buffSize, GFP_KERNEL);
    if (!(priv_dev->_gpio.allocatedMem)) {
        res = -ENOMEM;
        goto out;
    }

    priv_dev->_gpio.GPSEL = _prvtDrvData.gpio_base + GPIO_GPFSEL0;
    priv_dev->_gpio.GPSET = _prvtDrvData.gpio_base + GPIO_GPSET0;
    priv_dev->_gpio.GPCLR = _prvtDrvData.gpio_base + GPIO_GPCLR0;

    if (priv_dev->_gpio.pin_num == 21)
        mask_shift = 3;
    else if (priv_dev->_gpio.pin_num == 26)
        mask_shift = 18;
    else
        mask_shift = 0;

    val = ioread32(priv_dev->_gpio.GPSEL);
    val &= ~(0b111 << mask_shift);
    val |= (0b001 << mask_shift);
    iowrite32(val, priv_dev->_gpio.GPSEL);

    cdev_init(&priv_dev->_cdev, &gpio_fops);
    res = cdev_add(&priv_dev->_cdev, _prvtDrvData.devNum + platResource->id, 1);
    if (res < 0)
        goto out;

    priv_dev->_device = device_create(_prvtDrvData._class, NULL, _prvtDrvData.devNum + platResource->id, NULL, "gpio%d", platResource->id);
    if (IS_ERR(priv_dev->_device)) {
        res = PTR_ERR(priv_dev->_device);
        cdev_del(&priv_dev->_cdev);
        goto out;
    }

out:
    return res;
}

int removeCBF (struct platform_device* platResource)
{
    struct platform_device_private_data* priv_dev = platform_get_drvdata(platResource);

    device_destroy(_prvtDrvData._class, _prvtDrvData.devNum + platResource->id);
    cdev_del(&priv_dev->_cdev);
    kfree(priv_dev->_gpio.allocatedMem);

    printk("Device Removed! \n");

    return 0;
}

struct platform_driver _platDriver = 
{
    .probe = probeCBF,
    .remove = removeCBF,
    .id_table = _id_table,
    .driver = {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE
    }
};

static int __init gpioDriv_init(void)
{
    int res = 0;

    res = alloc_chrdev_region(&_prvtDrvData.devNum, 0, NUM_OF_ENTRIES, DRIVER_NAME); 
    if (res < 0)
        goto out;

    _prvtDrvData._class = class_create(THIS_MODULE, "device");
    if (IS_ERR(_prvtDrvData._class)) {
        res = PTR_ERR(_prvtDrvData._class);
        goto err_class;
    }

    _prvtDrvData.gpio_base = ioremap(GPIO_BASE, GPIO_SIZE);
    if (!_prvtDrvData.gpio_base) {
        res = -ENOMEM;
        goto err_iomap;
    }

    res = platform_driver_register(&_platDriver);
    if (res < 0)
        goto err_driver;

    return 0;

err_driver:
    iounmap(_prvtDrvData.gpio_base);
err_iomap:
    class_destroy(_prvtDrvData._class);
err_class:
    unregister_chrdev_region(_prvtDrvData.devNum, NUM_OF_ENTRIES);
out:
    return res;
}

static void __exit gpioDriv_exit(void)
{
    platform_driver_unregister(&_platDriver);
    iounmap(_prvtDrvData.gpio_base);
    class_destroy(_prvtDrvData._class);
    unregister_chrdev_region(_prvtDrvData.devNum, NUM_OF_ENTRIES);
}

module_init(gpioDriv_init); 
module_exit(gpioDriv_exit);

