#include "contiki.h"
#include "sys/etimer.h"
#include "button-sensor.h"
#include "batmon-sensor.h"

#include <stdio.h>

/*---------------------------------------------------------------------------*/
#define BUF_SIZE 8
static int buffer[BUF_SIZE];
static int buf_c = 0;

static struct etimer et_sensor;
static struct etimer et_uart;
static struct etimer et_range;

/*---------------------------------------------------------------------------*/
PROCESS(sensor_process, "Sensor process");
PROCESS(uart_process, "Serial process");
PROCESS(range_process, "Range process");

/*---------------------------------------------------------------------------*/
AUTOSTART_PROCESSES(&sensor_process, &uart_process, &range_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(sensor_process, ev, data)
{
  PROCESS_BEGIN();

  SENSORS_ACTIVATE(batmon_sensor);

  etimer_set(&et_sensor, 1*CLOCK_SECOND); // a cada segundo

  while(1) {
    PROCESS_WAIT_EVENT();
    if(ev == PROCESS_EVENT_TIMER)  // se passou um segundo
    {
        etimer_reset(&et_sensor); // reinicia timer

        int val = batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP); // lê sensor

        buffer[buf_c++] = val;
        buf_c &= BUF_SIZE - 1;

        printf("Leu %d\n", val);
    }
  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(uart_process, ev, data)
{
  PROCESS_BEGIN();
  etimer_set(&et_uart, 10*CLOCK_SECOND); // a cada 10 segundos

  /* Insira seu código aqui */

  while(1) {
    PROCESS_WAIT_EVENT();
    if(ev == PROCESS_EVENT_TIMER)  // se passaram 10 segundos
    {
        /* Insira seu código aqui */
        etimer_reset(&et_uart); // reinicia timer

        int avg = 0;
        for(int i=0;i<BUF_SIZE;i++) {
            avg += buffer[i];
        }
        avg = avg/BUF_SIZE;
        printf("Temperatura media: %d, buf_c: %d\n", avg, buf_c);

    }
  }

  PROCESS_END();
}


/*---------------------------------------------------------------------------*/
PROCESS_THREAD(range_process, ev, data)
{
  PROCESS_BEGIN();
  etimer_set(&et_range, 20*CLOCK_SECOND); // a cada 20 segundos

  while(1) {
    PROCESS_WAIT_EVENT();
    if(ev == PROCESS_EVENT_TIMER)  // se passaram 10 segundos
    {

        etimer_reset(&et_range); // reinicia timer

        int max = buffer[0];
        int min = buffer[0];

        for(int i=1;i<BUF_SIZE;i++) {
            if(buffer[i] > max) {
                max = buffer[i];
            }
            if(buffer[i] < min) {
                min = buffer[i];
            }
        }
        printf("Valor Maximo: %d, Valor Minimo: %d\n", max, min);

    }
  }

  PROCESS_END();
}

