Here is a complete Linux char driver in C that supports read/write operations with a 1KB internal buffer:
```
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple char driver with a 1KB internal buffer");

#define DRIVER_NAME "my_char_driver"
#define BUFFER_SIZE 1024

static char buffer[BUFFER_SIZE];

static int my_char_driver_open(struct inode *inode, struct file *file)
{
    return 0; /* always success */
}

static int my_char_driver_release(struct inode *inode, struct file *file)
{
    return 0; /* always success */
}

static ssize_t my_char_driver_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
    int bytes_to_copy;
    int bytes_copied;

    bytes_to_copy = min(count, BUFFER_SIZE - *ppos);
    if (bytes_to_copy <= 0)
        return 0; /* end of buffer */

    if (copy_to_user(user_buf, buffer + *ppos, bytes_to_copy))
        return -EFAULT;

    bytes_copied = bytes_to_copy;
    *ppos += bytes_copied;

    return bytes_copied;
}

static ssize_t my_char_driver_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
    int bytes_to_copy;
    int bytes_copied;

    bytes_to_copy = min(count, BUFFER_SIZE - *ppos);
    if (bytes_to_copy <= 0)
        return -ENOSPC; /* buffer full */

    if (copy_from_user(buffer + *ppos, user_buf, bytes_to_copy))
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

    ret = register_chrdev(0, DRIVER_NAME, &my_char_driver_fops);
    if (ret < 0) {
        printk(KERN_ERR "Failed to register char driver\n");
        return ret;
    }

    printk(KERN_INFO "Registered char driver with major number %d\n", ret);

    return 0;
}

static void __exit my_char_driver_exit(void)
{
    unregister_chrdev(0, DRIVER_NAME);
    printk(KERN_INFO "Unregistered char driver\n");
}

module_init(my_char_driver_init);
module_exit(my_char_driver_exit);
```
This driver creates a char device with a 1KB internal buffer. The `open` and `release` functions are simple and always succeed. The `read` function copies data from the internal buffer to the user buffer, and the `write` function copies data from the user buffer to the internal buffer. The `file_operations` structure is used to register the driver with the kernel. The `init` function registers the driver and the `exit` function unregisters it.