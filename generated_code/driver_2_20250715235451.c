Here is a complete Linux platform driver in C for a memory-mapped GPIO with interrupt support:
```
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Memory-mapped GPIO driver with interrupt support");

#define GPIO_REG_BASE 0x10000000 // adjust to your device's base address
#define GPIO_REG_SIZE 0x1000 // adjust to your device's register size
#define GPIO_IRQ 23 // adjust to your device's interrupt number

struct gpio_chip gpio_chip;
struct irq_chip irq_chip;

static void gpio_irq_handler(struct irq_desc *desc)
{
    struct gpio_chip *gc = irq_desc_get_handler_data(desc);
    int irq = irq_desc_get_irq(desc);
    int gpio = irq - gc->irq_base;

    gpio_chip_toggle(gc, gpio);
}

static int gpio_irq_request(struct gpio_chip *gc, unsigned int offset)
{
    int irq = gc->irq_base + offset;
    int ret = request_irq(irq, gpio_irq_handler, 0, "gpio-irq", gc);
    if (ret)
        dev_err(gc->dev, "Failed to request IRQ %d\n", irq);
    return ret;
}

static void gpio_irq_free(struct gpio_chip *gc, unsigned int offset)
{
    int irq = gc->irq_base + offset;
    free_irq(irq, gc);
}

static int gpio_direction_input(struct gpio_chip *gc, unsigned int offset)
{
    // adjust to your device's register access
    writel(0, GPIO_REG_BASE + (offset * 4));
    return 0;
}

static int gpio_direction_output(struct gpio_chip *gc, unsigned int offset, int value)
{
    // adjust to your device's register access
    writel(value, GPIO_REG_BASE + (offset * 4));
    return 0;
}

static int gpio_get(struct gpio_chip *gc, unsigned int offset)
{
    // adjust to your device's register access
    return readl(GPIO_REG_BASE + (offset * 4));
}

static void gpio_set(struct gpio_chip *gc, unsigned int offset, int value)
{
    // adjust to your device's register access
    writel(value, GPIO_REG_BASE + (offset * 4));
}

static int gpio_probe(struct platform_device *pdev)
{
    struct gpio_chip *gc;
    struct resource *res;
    int ret;

    gc = kzalloc(sizeof(*gc), GFP_KERNEL);
    if (!gc)
        return -ENOMEM;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res)
        return -ENODEV;

    gc->dev = &pdev->dev;
    gc->label = "gpio-mm";
    gc->owner = THIS_MODULE;
    gc->base = -1;
    gc->ngpio = 32; // adjust to your device's number of GPIOs
    gc->irq_base = GPIO_IRQ;
    gc->irq_request = gpio_irq_request;
    gc->irq_free = gpio_irq_free;
    gc->direction_input = gpio_direction_input;
    gc->direction_output = gpio_direction_output;
    gc->get = gpio_get;
    gc->set = gpio_set;

    ret = gpiochip_add(gc);
    if (ret)
        goto err_free_gc;

    platform_set_drvdata(pdev, gc);

    return 0;

err_free_gc:
    kfree(gc);
    return ret;
}

static int gpio_remove(struct platform_device *pdev)
{
    struct gpio_chip *gc = platform_get_drvdata(pdev);

    gpiochip_remove(gc);
    kfree(gc);

    return 0;
}

static const struct platform_device_id gpio_ids[] = {
    { "gpio-mm", 0 },
    { },
};

MODULE_DEVICE_TABLE(platform, gpio_ids);

static struct platform_driver gpio_driver = {
    .probe = gpio_probe,
    .remove = gpio_remove,
    .id_table = gpio_ids,
    .driver = {
        .name = "gpio-mm",
    },
};

module_platform_driver(gpio_driver);
```
Note that you'll need to adjust the `GPIO_REG_BASE`, `GPIO_REG_SIZE`, and `GPIO_IRQ` definitions to match your device's specific memory-mapped GPIO registers and interrupt number. Additionally, you'll need to implement the `gpio_direction_input`, `gpio_direction_output`, `gpio_get`, and `gpio_set` functions to access your device's registers correctly.