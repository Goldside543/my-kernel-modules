// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * This module logs memory information to dmesg every 5 minutes.
 * Useful for seeing how memory logging works, and seeing
 * how timers work in the Linux kernel.
 * 
 * Copyright (C) 2024 Goldside543 <goldside000@outlook.com>
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/timer.h>

#define TIMER_INTERVAL (5 * 60 * HZ) // 5 minutes

static struct timer_list mem_timer;
static unsigned int interval = TIMER_INTERVAL; // Default interval

module_param(interval, uint, 0644);
MODULE_PARM_DESC(interval, "Timer interval in seconds for memory info logging");

void get_memory_info(struct timer_list *t) {
    struct sysinfo si;

    si_meminfo(&si);

    printk(KERN_INFO "Memory Info:\n");
    printk(KERN_INFO "Total RAM: %lu kB\n", si.totalram * si.mem_unit / 1024);
    printk(KERN_INFO "Free RAM: %lu kB\n", si.freeram * si.mem_unit / 1024);
    printk(KERN_INFO "Shared RAM: %lu kB\n", si.sharedram * si.mem_unit / 1024);
    printk(KERN_INFO "Buffered RAM: %lu kB\n", si.bufferram * si.mem_unit / 1024);

    // Re-schedule the timer
    mod_timer(&mem_timer, jiffies + interval * HZ);
}

static int __init mem_info_init(void) {
    printk(KERN_INFO "Memory Info Module Loaded\n");

    // Initialize the timer
    timer_setup(&mem_timer, get_memory_info, 0);

    // Schedule the timer for the first time
    mod_timer(&mem_timer, jiffies + interval * HZ);

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
MODULE_DESCRIPTION("A module that outputs memory information periodically.");
