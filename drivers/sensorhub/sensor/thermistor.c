#include "../sensormanager/shub_sensor.h"
#include "../sensormanager/shub_sensor_manager.h"
#include "../utility/shub_utility.h"

#include <linux/slab.h>

struct thermistor_event {
	u8 type;
	s16 raw;
} __attribute__((__packed__));

static void print_thermistor_debug(void)
{
	struct shub_sensor *sensor = get_sensor(SENSOR_TYPE_THERMISTOR);
	struct sensor_event *event = &(sensor->event_buffer);
	struct thermistor_event *sensor_value = (struct thermistor_event *)(event->value);

	shub_info("%s(%u) : %d, %d (%lld) (%ums, %dms)", sensor->name, SENSOR_TYPE_THERMISTOR, sensor_value->type,
		  sensor_value->raw, event->timestamp, sensor->sampling_period, sensor->max_report_latency);
}

void init_thermistor(bool en)
{
	struct shub_sensor *sensor = get_sensor(SENSOR_TYPE_THERMISTOR);

	if (!sensor)
		return;

	if (en) {
		strcpy(sensor->name, "thermistor_sensor");
		sensor->receive_event_size = 3;
		sensor->report_event_size = 3;
		sensor->event_buffer.value = kzalloc(sizeof(struct thermistor_event), GFP_KERNEL);

		sensor->funcs = kzalloc(sizeof(struct sensor_funcs), GFP_KERNEL);
		sensor->funcs->print_debug = print_thermistor_debug;
	} else {
		kfree(sensor->event_buffer.value);
		sensor->event_buffer.value = NULL;

		kfree(sensor->funcs);
		sensor->funcs = NULL;
	}
}
