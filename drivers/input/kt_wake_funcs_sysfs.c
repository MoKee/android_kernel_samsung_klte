#include <linux/kt_wake_funcs.h>

static ssize_t synaptics_rmi4_doubletap_wake_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	int ret;
	ret = sprintf(buf, "%d\n", doubletap_wake);
	return ret;
}
static ssize_t synaptics_rmi4_doubletap_wake_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	int retval;
	unsigned int val = 0;

	retval = sscanf(buf, "%d", &val);
	if (retval != 0) {
		doubletap_wake = (val == 1) ? true : false;
	}
	if (!doubletap_wake)
	{
		SYN_I2C_RETRY_TIMES = 10;
		GPIO_CFG_KT = GPIO_CFG_2MA;
	}
	else
	{
		SYN_I2C_RETRY_TIMES = 5;
		GPIO_CFG_KT = GPIO_CFG_16MA;
	}
	return count;
}

static struct device_attribute attrs[] = {
	__ATTR(doubletap_wake, (S_IRUGO | S_IWUSR | S_IWGRP),
			synaptics_rmi4_doubletap_wake_show,
			synaptics_rmi4_doubletap_wake_store),
};

void wake_funcs_sysfs(struct synaptics_rmi4_data *rmi4_data, struct i2c_client *client)
{
	unsigned char attr_count;
	int retval;
	
	for (attr_count = 0; attr_count < ARRAY_SIZE(attrs); attr_count++) {
		retval = sysfs_create_file(&rmi4_data->input_dev->dev.kobj,
				&attrs[attr_count].attr);
		if (retval < 0) {
			dev_err(&client->dev,
					"%s: Failed to create sysfs attributes\n",
					__func__);
		}
	}
}

