#ifndef BUILD_LK
#include <linux/string.h>
#include <linux/kernel.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
	#include <string.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
	#include <mach/mt_gpio.h>
#endif
// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (800)
#define FRAME_HEIGHT (1280)

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};
static g_LCM_UTIL_FUNCS = {0};

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	        lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)			lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)							lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)				lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)							lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   			lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)
#define g_read_reg_v2(cmd, buffer, buffer_size)   			g_LCM_UTIL_FUNCS.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)
#define _memzero(a,l)       						memset((void*)a,0,l)


void lcd_reset_part_1()
{
    mt_set_gpio_mode(0x46, 0);
    mt_set_gpio_dir(0x46, 1);
    mt_set_gpio_out(0x46, 1);
}

void stbyb_control_part_2()
{
    mt_set_gpio_mode(0x44, 0);
    mt_set_gpio_dir(0x44, 1);
    mt_set_gpio_out(0x44, 1);
}

void lcd_vdd_enable_part_3()
{
    mt_set_gpio_mode(0x09, 0);
    mt_set_gpio_dir(0x09, 1);
    mt_set_gpio_out(0x09, 1);
}

void lcd_reset(unsigned int flag)
{
    if (flag)
    {
        lcd_reset_part_1();
    }
    else
    {
        mt_set_gpio_mode(0x46, 0);
        mt_set_gpio_dir(0x46, 1);
        mt_set_gpio_out(0x46, 0);
    }
}

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
	memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
	memcpy(&g_LCM_UTIL_FUNCS, util, 0x64);
}

static void lcm_get_params(LCM_PARAMS *params)
{
    memset(params, 0, sizeof(LCM_PARAMS));
    _memzero(params, 0x2EC);
    params->physical_height = 173;
    params->physical_width = 108;
    params->dbi.te_edge_polarity = 1;
    params->dsi.packet_size = 256;
    params->dsi.word_count = 2400;
    params->dsi.vertical_sync_active = 4;
    params->dsi.vertical_backporch = 11;
    params->dsi.horizontal_frontporch = 32;
    params->type = 2;
    params->dsi.PLL_CLOCK = 225;
    params->dsi.data_format.format = 2;
    params->dsi.intermediat_buffer_num = 2;
    params->dsi.PS = 2;
    params->width = 800;
    params->dsi.horizontal_active_pixel = 800;
    params->height = 1280;
    params->dsi.vertical_active_line = 1280;
    params->dbi.te_mode = 0;
    params->dsi.data_format.color_order = 0;
    params->dsi.data_format.trans_seq = 0;
    params->dsi.data_format.padding = 0;
    params->dsi.mode = 3;
    params->dsi.LANE_NUM = 3;
    params->dsi.vertical_frontporch = 8;
    params->dsi.horizontal_sync_active = 8;
    params->dsi.horizontal_backporch = 8;
}

static void lcm_init(void)
{
    unsigned int data_array[16];

    lcd_reset(0);

    mt_set_gpio_mode(0x09, 0);
    mt_set_gpio_dir(0x09, 1);
    mt_set_gpio_out(0x09, 0);
    MDELAY(100);

    printk("%s, begin\n", "lcd_power_en");
    MDELAY(50);

    lcd_vdd_enable_part_3();
    MDELAY(30);

    stbyb_control_part_2();
    MDELAY(30);

    lcd_reset_part_1();
    MDELAY(50);

    lcd_reset(0);
    MDELAY(50);

    lcd_reset_part_1();
    MDELAY(80);

    printk("[Elink_LCM_Drv Nicholas OTM1283A] %s, kernel", "init_lcm_registers");

    data_array[0] = 0xE01500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x93E11500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x65E21500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xF8E31500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x2801500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x2E61500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x2E71500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x4E01500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x32D1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xE01500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x2701500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x23711500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x6721500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1E01500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x96011500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x31500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x96041500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x171500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x75181500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x191500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1A1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x751B1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1C1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x481F1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x23201500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x23211500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xE221500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x231500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x38241500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xD3261500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x59371500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x5381500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x8391500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x123A1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x783C1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x803E1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x803F1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x6401500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xA0411500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xF551500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1561500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x85571500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xA581500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xA591500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x325A1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xF5B1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x705D1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x6E5E1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x655F1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x5D601500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x5F611500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x51621500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x56631500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x3F641500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x55651500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x50661500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x4A671500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x68681500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x51691500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x586A1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x336B1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x336C1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x2A6D1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1A6E1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x6F1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x7F701500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x76711500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x6D721500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x65731500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x66741500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x59751500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x5E761500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x47771500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x5D781500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x58791500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x537A1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x6F7B1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x597C1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x5F7D1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x3B7E1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x3B7F1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x31801500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x23811500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x821500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x2E01500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F001500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F011500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x13021500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x11031500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xB041500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xB051500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x9061500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x9071500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x7081500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F091500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F0A1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F0B1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F0C1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F0D1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F0E1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x70F1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x5101500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x5111500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1121500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x3131500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F141500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F151500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F161500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F171500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x12181500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x10191500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xA1A1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xA1B1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x81C1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x81D1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x61E1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F1F1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F201500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F211500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F221500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F231500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F241500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x6251500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x4261500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x4271500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x281500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x2291500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F2A1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F2B1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F2C1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F2D1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x2E1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x22F1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x8301500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x8311500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xA321500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xA331500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x4341500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F351500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F361500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F371500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F381500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F391500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F3A1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x43B1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x63C1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x63D1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x123E1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x103F1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F401500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F411500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F421500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F431500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1441500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x3451500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x9461500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x9471500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xB481500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xB491500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x54A1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F4B1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F4C1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F4D1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F4E1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F4F1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F501500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x5511500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x7521500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x7531500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x13541500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x11551500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F561500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1F571500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x40581500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x591500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x5A1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x305B1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x85C1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x305D1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x15E1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x25F1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x30601500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1611500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x2621500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x3631500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x6B641500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x75651500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xD661500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x73671500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xA681500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x3691500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x6B6A1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x86B1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x6C1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x46D1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x46E1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x886F1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x701500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x711500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x6721500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x7B731500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x741500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x3C751500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x761500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xD771500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x2C781500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x791500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x7A1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x7B1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x7C1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x37D1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x7B7E1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x1E01500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x10E1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x3E01500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x2F981500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x4E01500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x2B2B1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x442E1500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xE01500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0xE01500;
    dsi_set_cmdq(&data_array, 1, 1);

    data_array[0] = 0x110500;
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(150);

    data_array[0] = 0x290500;
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(20);
}

static void lcm_suspend(void)
{
    unsigned int data_array[16];

    printk("[Elink_LCM_Drv ZX_______] %s, kernel", "lcm_suspend");
    data_array[0] = 0x280500;
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(30);

    data_array[0] = 0x100500;
    dsi_set_cmdq(&data_array, 1, 1);
    MDELAY(120);

    lcd_reset(0);
    MDELAY(10);
    MDELAY(10);

    mt_set_gpio_mode(0x44, 0);
    mt_set_gpio_dir(0x44, 1);
    mt_set_gpio_out(0x44, 0);
    MDELAY(10);

    mt_set_gpio_mode(0x09, 0);
    mt_set_gpio_dir(0x09, 1);
    mt_set_gpio_out(0x09, 0);
    MDELAY(80);
}

static void lcm_resume(void)
{
    printk("[Elink_LCM_Drv ZX_______] %s, kernel", "lcm_resume");
    lcm_init();
}

// usefull STAFF

LCM_DRIVER jd9366_ivo_ips_dsi_lcm_drv = 
{
    	.name		= "jd9366_ivo_ips_dsi",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
};
