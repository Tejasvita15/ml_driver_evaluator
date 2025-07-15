Here is a complete Linux platform driver in C for a memory-mapped GPIO with interrupt support:
```
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/slab.h>

#define GPIO_BASE 0x10000000
#define GPIO_SIZE 0x100
#define GPIO_IRQ 10

struct gpio_chip gpio_chip;
struct irq_chip irq_chip;

static void gpio_irq_handler(struct irq_desc *desc)
{
    struct gpio_chip *gc = irq_desc_get_handler_data(desc);
    u32 irq_status;

    irq_status = ioread32(gc->base + 0x10);
    if (irq_status) {
        gpio_handle_irq(gc, irq_status);
    }
}

static int gpio_irq_request(struct irq_handler *handler, irq_handler_t *action)
{
    return request_irq(GPIO_IRQ, action->handler, IRQF_SHARED, "gpio-irq", handler);
}

static void gpio_irq_free(struct irq_handler *handler)
{
    free_irq(GPIO_IRQ, handler);
}

static int gpio_direction_input(struct gpio_chip *gc, unsigned int offset)
{
    iowrite32(0, gc->base + offset * 4);
    return 0;
}

static int gpio_direction_output(struct gpio_chip *gc, unsigned int offset, int value)
{
    iowrite32(value, gc->base + offset * 4);
    return 0;
}

static int gpio_get(struct gpio_chip *gc, unsigned int offset)
{
    return !!(ioread32(gc->base + offset * 4));
}

static void gpio_set(struct gpio_chip *gc, unsigned int offset, int value)
{
    iowrite32(value, gc->base + offset * 4);
}

static int gpio_probe(struct platform_device *pdev)
{
    struct gpio_chip *gc;
    struct irq_chip *ic;
    void __iomem *base;

    base = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(base))
        return PTR_ERR(base);

    gc = devm_kzalloc(&pdev->dev, sizeof(*gc), GFP_KERNEL);
    if (!gc)
        return -ENOMEM;

    gc->base = base;
    gc->ngpio = 32;
    gc->direction_input = gpio_direction_input;
    gc->direction_output = gpio_direction_output;
    gc->get = gpio_get;
    gc->set = gpio_set;

    ic = devm_kzalloc(&pdev->dev, sizeof(*ic), GFP_KERNEL);
    if (!ic)
        return -ENOMEM;

    ic->irq_mask_ack = gpio_irq_handler;
    ic->irq_unmask = gpio_irq_request;
    ic->irq_mask = gpio_irq_free;

    gpio_chip_register(gc);
    irq_chip_register(ic);

    return 0;
}

static int gpio_remove(struct platform_device *pdev)
{
    gpio_chip_unregister(&gpio_chip);
    irq_chip_unregister(&irq_chip);
    return 0;
}

static const struct of_device_id gpio_of_match[] = {
    { .compatible = "gpio-mmio" },
    { /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, gpio_of_match);

static struct platform_driver gpio_driver = {
    .probe = gpio_probe,
    .remove = gpio_remove,
    .driver = {
        .name = "gpio-mmio",
        .of_match_table = gpio_of_match,
    },
};

module_platform_driver(gpio_driver);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("GPIO MMIO Driver");
```
This driver assumes that the GPIO controller is memory-mapped at address `GPIO_BASE` and has a size of `GPIO_SIZE`. It also assumes that the interrupt number is `GPIO_IRQ`.

The driver provides a basic GPIO interface with input/output direction control, get/set operations, and interrupt handling. It uses the `gpio_chip` and `irq_chip` structures to register the GPIO controller with the Linux kernel.

Note that this is just a sample driver and may need to be modified to fit your specific use case. You may also need to add additional error handling and debugging code.