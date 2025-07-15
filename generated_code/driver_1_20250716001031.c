Here is a complete Linux char driver in C that supports read/write operations with a 1KB internal buffer:
```
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple char driver with a 1KB internal buffer");

#define BUFFER_SIZE 1024
static char buffer[BUFFER_SIZE];

static int my_char_driver_open(struct inode *inode, struct file *file)
{
    return 0; /* always succeed */
}

static int my_char_driver_release(struct inode *inode, struct file *file)
{
    return 0; /* always succeed */
}

static ssize_t my_char_driver_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    int bytes_to_copy;
    int bytes_copied = 0;

    if (*ppos >= BUFFER_SIZE)
        return 0; /* EOF */

    bytes_to_copy = min(count, BUFFER_SIZE - *ppos);
    if (copy_to_user(buf, buffer + *ppos, bytes_to_copy))
        return -EFAULT;

    bytes_copied = bytes_to_copy;
    *ppos += bytes_copied;

    return bytes_copied;
}

static ssize_t my_char_driver_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    int bytes_to_copy;
    int bytes_copied = 0;

    if (*ppos >= BUFFER_SIZE)
        return -ENOSPC; /* no space left */

    bytes_to_copy = min(count, BUFFER_SIZE - *ppos);
    if (copy_from_user(buffer + *ppos, buf, bytes_to_copy))
        return -EFAULT;

    bytes_copied = bytes_to_copy;
    *ppos += bytes_copied;

    return bytes_copied;
}

static const struct file_operations my_char_driver_fops = {
    .owner = THIS_MODULE,
    .open = my_char_driver_open,
    .release = my_char_driver_release,
    .read = my_char_driver_read,
    .write = my_char_driver_write,
};

static int __init my_char_driver_init(void)
{
    int ret;

    ret = register_chrdev(0, "my_char_driver", &my_char_driver_fops);
    if (ret < 0) {
        printk(KERN_ERR "Failed to register char driver\n");
        return ret;
    }

    printk(KERN_INFO "my_char_driver: registered with major number %d\n", ret);

    return 0;
}

static void __exit my_char_driver_exit(void)
{
    unregister_chrdev(0, "my_char_driver");
    printk(KERN_INFO "my_char_driver: unregistered\n");
}

module_init(my_char_driver_init);
module_exit(my_char_driver_exit);
```
This driver provides a simple character device that allows userspace to read and write to a 1KB internal buffer. The `my_char_driver_open` and `my_char_driver_release` functions are no-ops, as we don't need to perform any specific actions when the device is opened or closed. The `my_char_driver_read` and `my_char_driver_write` functions handle read and write operations, respectively, by copying data to/from the internal buffer using the `copy_to_user` and `copy_from_user` functions.