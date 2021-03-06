/*
 * This file is part of the coreboot project.
 *
 * Copyright 2016 Rockchip Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <arch/cache.h>
#include <arch/io.h>
#include <boardid.h>
#include <boot/coreboot_tables.h>
#include <device/device.h>
#include <console/console.h>
#include <gpio.h>
#include <soc/clock.h>
#include <soc/grf.h>

static void configure_sdmmc(void)
{
	gpio_output(GPIO(0, A, 1), 1);	/* SDMMC_PWR_EN */
	gpio_input(GPIO(0, A, 7));	/* SDMMC_DET_L */
	write32(&rk3399_grf->iomux_sdmmc, IOMUX_SDMMC);
}

static void configure_emmc(void)
{
	/* emmc core clock multiplier */
	rk_clrsetreg(&rk3399_grf->emmccore_con[11], 0xff << 0, 0 << 0);

	rkclk_configure_emmc();
}

static void configure_usb(void)
{
	gpio_output(GPIO(4, D, 1), 1); /* vbus_drv_en */
}

static void mainboard_init(device_t dev)
{
	configure_sdmmc();
	configure_emmc();
	configure_usb();
}

static void mainboard_enable(device_t dev)
{
	dev->ops->init = &mainboard_init;
}

struct chip_operations mainboard_ops = {
	.name = "rk3399evb",
	.enable_dev = mainboard_enable,
};
