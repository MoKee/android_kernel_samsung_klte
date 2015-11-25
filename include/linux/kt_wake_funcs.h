#include <linux/kernel.h>

#include <linux/of_gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include "../../drivers/input/touchscreen/synaptics/synaptics_i2c_rmi.h"
#include "../../drivers/sensorhub/stm/ssp.h"

extern int SYN_I2C_RETRY_TIMES;
extern int GPIO_CFG_KT;

extern struct ssp_data *main_prox_data;

extern unsigned int screen_wake_options; // 0 = disabled; 1 = s2w; 2 = s2w only while charging; 3 = dtap2wake; 4 = dtap2wake only while charging; 5 = both
extern unsigned int screen_wake_options_prox_max;
extern unsigned int screen_wake_options_when_off;

extern bool screen_is_off;

extern void check_touch_off(int x, int y, unsigned char state, unsigned char touch_count);

extern void wake_funcs_sysfs(struct synaptics_rmi4_data *rmi4_data, struct i2c_client *client);

extern void wake_funcs_init(void);

void pwr_trig_fscreen(void);

extern bool wake_options_okto_enable(void);
