#include "contiki.h"
#include "sys/etimer.h"
#include "button-sensor.h"
#include "batmon-sensor.h"

#include <stdio.h>


PROCESS(button_process, "Test button");

AUTOSTART_PROCESSES(&button_process);

PROCESS_THREAD(button_process, ev, data)
{
  struct sensors_sensor *sensor;

  PROCESS_BEGIN();

  SENSORS_ACTIVATE(batmon_sensor);
  SENSORS_ACTIVATE(button_sensor);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event);

    sensor = (struct sensors_sensor *)data;
    if(sensor == &button_sensor) {
        int val = batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP);
        printf("Leu %d \n", val);
    } else {
        printf("Event: %d \n", ev);
    }
  }

  PROCESS_END();
}
