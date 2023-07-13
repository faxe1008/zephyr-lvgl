/*
 * Copyright (c) 2023 Fabian Blatz <fabianblatz@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/shell/shell.h>
#include <lvgl.h>
#include <autoconf.h>

#ifdef CONFIG_LV_Z_MEM_POOL_SYS_HEAP
#include "lvgl_mem.h"
#endif


static int cmd_lvgl_mem_stat(const struct shell *sh, size_t argc,
			    char *argv[])
{
    bool dump_chunks = false;

    if(argc == 2){
        if(strcmp(argv[1], "--chunks")){
            dump_chunks = true;
        }
    }

    lvgl_print_heap_info(dump_chunks);

    return 0;
}



SHELL_STATIC_SUBCMD_SET_CREATE(lvgl_cmds,
#ifdef CONFIG_LV_Z_MEM_POOL_SYS_HEAP
	SHELL_CMD_ARG(mem_stat, NULL, "Print LVGL memory statistics", cmd_lvgl_mem_stat, 1, 1),
#endif
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(lvgl, &lvgl_cmds, "LVGL shell commands", NULL);