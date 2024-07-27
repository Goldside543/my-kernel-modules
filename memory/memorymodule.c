/*
 * memorymodule.c - my first module
 * Includes a shell script that pulls up the amount of free RAM.
 * The higher the timestamp number, the newer the measurement is.
 */ 

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/timer.h>

#define TIMER_INTERVAL (5 * 60 * HZ) // 5 minutes

static struct timer_list mem_timer;

void get_memory_info(struct timer_list *t) {
    struct sysinfo si;
    
    si_meminfo(&si);
    
    printk(KERN_INFO "Memory Info:\n");
    printk(KERN_INFO "Total RAM: %lu kB\n", si.totalram * 4);
    printk(KERN_INFO "Free RAM: %lu kB\n", si.freeram * 4);
    printk(KERN_INFO "Shared RAM: %lu kB\n", si.sharedram * 4);
    printk(KERN_INFO "Buffered RAM: %lu kB\n", si.bufferram * 4);
    
    // Re-schedule the timer
    mod_timer(&mem_timer, jiffies + TIMER_INTERVAL);
}

static int __init mem_info_init(void) {
    printk(KERN_INFO "Memory Info Module Loaded\n");
    
    // Initialize the timer
    timer_setup(&mem_timer, get_memory_info, 0);
    
    // Schedule the timer for the first time
    mod_timer(&mem_timer, jiffies + TIMER_INTERVAL);
    
    return 0;
}

static void __exit mem_info_exit(void) {
    del_timer(&mem_timer);
    printk(KERN_INFO "Memory Info Module Unloaded\n");
}

module_init(mem_info_init);
module_exit(mem_info_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Goldside543");
MODULE_DESCRIPTION("A module that outputs memory information every 5 minutes.");
