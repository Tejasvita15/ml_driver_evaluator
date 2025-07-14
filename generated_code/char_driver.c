#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "char_demo"
#define BUF_LEN 1024

static char buffer[BUF_LEN];
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = {
    .read = dev_read,
    .write = dev_write,
    .open = dev_open,
    .release = dev_release
};

static int __init demo_init(void) {
    register_chrdev(0, DEVICE_NAME, &fops);
    return 0;
}

static void __exit demo_exit(void) {
    unregister_chrdev(0, DEVICE_NAME);
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");
