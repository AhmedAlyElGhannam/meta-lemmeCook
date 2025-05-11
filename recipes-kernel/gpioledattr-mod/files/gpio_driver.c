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

/************************************************************** */

#define DIR_ATTR "dir" 
#define VALUE_ATTR "val"
static ssize_t dir_showFunction(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t dir_storeFunction(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
static ssize_t val_showFunction(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t val_storeFunction(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);

// static DEVICE_ATTR(val, S_IRUGO | S_IWUSR, val_showFunction, val_storeFunction);
struct device_attribute dir_attr = {
    .attr = {
        .mode = S_IRWXU | S_IRWXG | S_IRWXO,
        .name = DIR_ATTR,
    },
    .show = dir_showFunction,
    .store = dir_storeFunction
};
struct device_attribute val_attr = {
    .attr = {
        .mode = S_IRWXU | S_IRWXG | S_IRWXO,
        .name = VALUE_ATTR,
    },
    .show = val_showFunction,
    .store = val_storeFunction
};
/****************************************************************** */


static const struct file_operations gpio_fops = {
    .owner = THIS_MODULE,
    .open = gpio_open,
    .write = gpio_write,
};

const struct platform_device_id _id_table[NUM_OF_GPIOS] = {
    [0] = { .name = "LED_AQUA" },
    [1] = { .name = "LED_CRIMSON" }
};

/**************************** */
static ssize_t dir_showFunction(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct prv_data_device *dev_data = dev_get_drvdata(dev);
    u32 reg = ioread32(dev_data->gpio.gpfsels);
    int dir = (reg >> (dev_data->led_pin * 3)) & 0x7;
    return sprintf(buf, "%s\n", (dir == 1) ? "out" : "in");
}

static ssize_t dir_storeFunction(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    struct prv_data_device *dev_data = dev_get_drvdata(dev);
    u32 reg;
    
    if (sysfs_streq(buf, "out")) {
        reg = ioread32(dev_data->gpio.gpfsels);
        reg &= ~(7 << (dev_data->led_pin * 3));
        reg |= (1 << (dev_data->led_pin * 3));
        iowrite32(reg, dev_data->gpio.gpfsels);
        return count;
    }
    else if (sysfs_streq(buf, "in")) {
        reg = ioread32(dev_data->gpio.gpfsels);
        reg &= ~(7 << (dev_data->led_pin * 3));
        iowrite32(reg, dev_data->gpio.gpfsels);
        return count;
    }
    
    return -EINVAL;
}

static ssize_t val_showFunction(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct prv_data_device *dev_data = dev_get_drvdata(dev);
    u32 reg = ioread32(dev_data->gpio.gpset);
    int state = (reg >> dev_data->led_pin) & 1;
    return sprintf(buf, "%d\n", state);
}

static ssize_t val_storeFunction(struct device *dev, struct device_attribute *attr, 
                               const char *buf, size_t count)
{
    struct prv_data_device *dev_data = dev_get_drvdata(dev);
    unsigned long val;
    int ret;
    
    ret = kstrtoul(buf, 10, &val);
    if (ret)
        return ret;
        
    if (val) {
        iowrite32(1 << dev_data->led_pin, dev_data->gpio.gpset);
    } else {
        iowrite32(1 << dev_data->led_pin, dev_data->gpio.gpclr);
    }
    
    return count;
}

/********************** */

static int gpio_open(struct inode *inode, struct file *file) {
    struct platform_device_private_data* dev_data = container_of(inode->i_cdev, struct platform_device_private_data, _cdev);
    file->private_data = dev_data;
    printk("Open CBF for LED GPIO device/file number %d\n", dev_data->pin_num);
    return 0;
}

static ssize_t gpio_write(struct file* file, const char __user* user_buff, size_t len, loff_t* off) {
    struct platform_device_private_data* dev_data = file->private_data;
    char buffer[GPIO_BUF_SIZE] = {0};
    ssize_t res = 0;
    u32 not_copied;

    if (!dev_data)
        return -ENODATA;

    not_copied = copy_from_user(buffer, user_buff, len);
    if (not_copied)
        return -EFAULT;

    if (buffer[0] == '0') {
        printk("Turning LED off\n");
        iowrite32(1 << dev_data->pin_num, dev_data->_gpio.GPCLR);
    } else if (buffer[0] == '1') {
        printk("Turning LED on\n");
        iowrite32(1 << dev_data->pin_num, dev_data->_gpio.GPSET);
    } else {
        printk("Invalid input! Use '0' to turn off, '1' to turn on\n");
    }

    return len;
}

int probeCBF(struct platform_device* platResource) {
    int res = 0;
    uint32_t val;
    uint32_t mask_shift;
    struct GPIO* _lc_gpio;

    struct platform_device_private_data* _prvDevData = devm_kzalloc(&platResource->dev, sizeof(*_prvDevData), GFP_KERNEL);
    if (!_prvDevData)
        return -ENOMEM;

    platform_set_drvdata(platResource, _prvDevData);
    _lc_gpio = &_prvDevData->_gpio;

    _lc_gpio->GPSEL = _prvtDrvData->gpio_base + GPIO_GPFSEL2;
    _lc_gpio->GPSET = _prvtDrvData->gpio_base + GPIO_GPSET0;
    _lc_gpio->GPCLR = _prvtDrvData->gpio_base + GPIO_GPCLR0;

    switch (platResource->id) {
        case 0: _prvDevData->pin_num = LED_AQUA_PIN; mask_shift = 3; break;
        case 1: _prvDevData->pin_num = LED_CRIMSON_PIN; mask_shift = 18; break;
        default: return -EINVAL;
    }

    val = ioread32(_lc_gpio->GPSEL);
    val &= ~(0b111 << mask_shift);
    val |= (0b001 << mask_shift);
    iowrite32(val, _lc_gpio->GPSEL);

    cdev_init(&_prvDevData->_cdev, &gpio_fops);
    res = cdev_add(&_prvDevData->_cdev, _prvtDrvData->devNum + platResource->id, 1);
    if (res < 0)
        return res;

    _prvDevData->_device = device_create(_prvtDrvData->_class, NULL, _prvtDrvData->devNum + platResource->id, NULL, "gpio-led%d", platResource->id);
    if (IS_ERR(_prvDevData->_device)) {
        res = PTR_ERR(_prvDevData->_device);
        cdev_del(&_prvDevData->_cdev);
        return res;
    }

    /************************************ */
    res = device_create_file(_prvDevData->_device, &dir_attr);
    if (res) 
    {
        dev_err(&platResource->dev, "Failed to create dir attribute\n");
    }
    
    ret = device_create_file(dev_data->_device, &val_attr);
    if (res) 
    {
        dev_err(&platResource->dev, "Failed to create val attribute\n");
    }
    /******************************** */

    return 0;
}

int removeCBF(struct platform_device* platResource) {
    struct platform_device_private_data* _prvDevData = platform_get_drvdata(platResource);

    /*********************************** */
    device_remove_file(_prvDevData->_device, &dir_attr);
    device_remove_file(_prvDevData->_device, &val_attr);

    /*********************************** */

    device_destroy(_prvtDrvData->_class, _prvtDrvData->devNum + platResource->id);
    cdev_del(&_prvDevData->_cdev);

    printk("Device Removed!\n");
    return 0;
}

struct platform_driver _platDriver = {
    .probe = probeCBF,
    .remove = removeCBF,
    .id_table = _id_table,
    .driver = {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE
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
