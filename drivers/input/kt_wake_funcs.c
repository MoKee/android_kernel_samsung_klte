#include <linux/kt_wake_funcs.h>
#include <linux/qpnp/power-on.h>

struct qpnp_pon *screenwake_pwrdev;
DEFINE_MUTEX(scr_lock);

struct ssp_data *main_prox_data;

unsigned long last_touch_time = 0;
bool screen_is_off = false;
bool doubletap_wake = false;
unsigned int prox_max = 55;
unsigned int doubletap_when_off = 0;

void screenwake_setdev(struct qpnp_pon * pon)
{
	screenwake_pwrdev = pon;
}

void screenwake_presspwr(struct work_struct *screenwake_presspwr_work)
{
	pr_alert("POWER TRIGGER CALLED");
	//if (screen_is_off)
	//	gkt_boost_cpu_call(true, true);

	input_report_key(screenwake_pwrdev->pon_input, 116, 1);
	input_sync(screenwake_pwrdev->pon_input);
	msleep(250);
	input_report_key(screenwake_pwrdev->pon_input, 116, 0);
	input_sync(screenwake_pwrdev->pon_input);
	msleep(250);
	last_touch_time = 0;
	mutex_unlock(&scr_lock);
}
DECLARE_WORK(screenwake_presspwr_work, screenwake_presspwr);

void pwr_trig_fscreen(void)
{
	if (mutex_trylock(&scr_lock))
		schedule_work(&screenwake_presspwr_work);
}

void check_touch_off(int x, int y, unsigned char state, unsigned char touch_count)
{
	unsigned char prox = 0;
	if (main_prox_data != NULL)
		prox = main_prox_data->buf[PROXIMITY_RAW].prox[0];//get_proximity_rawdata(main_prox_data);
	//pr_alert("KT TOUCH2 - %d\n", prox);
	if (prox > prox_max)
		return;

	//Double Tap 2 wake
	if (doubletap_wake && !state)
	{
		bool block_store = false;
		if (last_touch_time)
		{
			pr_alert("DOUBLE TAP WAKE TOUCH %d-%d-%ld-%ld-%d\n", x, y, jiffies, last_touch_time, touch_count);
			if (!touch_count && jiffies_to_msecs(jiffies - last_touch_time) < 2000) //(x < x_lo) && (y > y_hi) && //jiffies_to_msecs(jiffies - last_touch_time) > 50
			{
				pr_alert("DOUBLE TAP WAKE POWER BTN CALLED %d-%d\n", x, y);
				pwr_trig_fscreen();
				last_touch_time = 0;
				block_store = true;
			}
			else
			{
				pr_alert("DOUBLE TAP WAKE DELETE %d-%d-%ld-%ld\n", x, y, jiffies, last_touch_time);
				last_touch_time = 0;
				block_store = true;
			}
		}
		if (!last_touch_time && !block_store)
			last_touch_time = jiffies;
	}
}

void wake_funcs_init(void)
{
	mutex_init(&scr_lock);
}

bool wake_options_okto_enable(void)
{
	return doubletap_wake;
}

