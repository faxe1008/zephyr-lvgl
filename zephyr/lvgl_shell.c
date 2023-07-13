/*
 * Copyright (c) 2023 Fabian Blatz <fabianblatz@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include <lvgl.h>
#include <autoconf.h>

LOG_MODULE_REGISTER(lvgl_shell, CONFIG_LV_LOG_LEVEL);

#ifdef CONFIG_LV_Z_MEM_POOL_SYS_HEAP
#include "lvgl_mem.h"
#endif

#ifdef CONFIG_LV_USE_PERF_MONITOR

void perf_sample(struct k_timer *timer){
    ARG_UNUSED(timer);

    uint32_t avg_fps = lv_refr_get_fps_avg();

    lv_refr_reset_fps_counter();
}

K_TIMER_DEFINE(perf_sample_timer, perf_sample, NULL);
#endif

static int cmd_lvgl_stats(const struct shell *sh, size_t argc,
			    char *argv[])
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    shell_help(sh);
    return SHELL_CMD_HELP_PRINTED;
}

static int cmd_lvgl_stats_memory(const struct shell *sh, size_t argc,
			    char *argv[])
{
#ifdef CONFIG_LV_Z_MEM_POOL_SYS_HEAP
    bool dump_chunks = false;

    if(argc == 2) {
        if(strcmp(argv[1], "-c") == 0){
            dump_chunks = true;
        } else {
            shell_error(sh, "unsupported option %s", argv[1]);
            shell_help(sh);
            return SHELL_CMD_HELP_PRINTED;
        }
    }

    lvgl_print_heap_info(dump_chunks);
#else
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    shell_error(sh, "Set CONFIG_LV_Z_MEM_POOL_SYS_HEAP to enable memory statistics support.");
#endif
    return 0;
}

static int cmd_lvgl_stats_perf(const struct shell *sh, size_t argc,
			    char *argv[])
{
#ifdef CONFIG_LV_USE_PERF_MONITOR
    return 0;
#else
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    shell_error(sh, "Set CONFIG_LV_USE_PERF_MONITOR to enable performance statistics support.");
#endif
}

SHELL_STATIC_SUBCMD_SET_CREATE(lvgl_cmd_stats,
	SHELL_CMD_ARG(memory, NULL, 
        "Show LVGL memory statistics\n"
        "Usage: lvgl stats memory [-c]\n"
		"-c  dump chunk information\n", cmd_lvgl_stats_memory, 1, 1),
    SHELL_CMD_ARG(perf, NULL,
        "Start or stop performance profiling\n"
        "Usage: lvgl stats perf <start|stop>", cmd_lvgl_stats_perf, 2, 0),
	SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(lvgl_cmds,
	SHELL_CMD(stats, &lvgl_cmd_stats, "Show LVGL statistics", cmd_lvgl_stats),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(lvgl, &lvgl_cmds, "LVGL shell commands", NULL);