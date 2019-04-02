#include <stdio.h>

#include "contiki.h"
#include "ti-lib.h"
#include "driverlib/aux_adc.h"
#include "driverlib/aux_wuc.h"

uint16_t singleSample;
static struct etimer et_adc;

PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);

PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();

  printf("Inicio ADC");

  etimer_set(&et_adc, CLOCK_SECOND * 2);

   while(1) {
     PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et_adc));

     printf("ADC reading!\n");

     ti_lib_aon_wuc_aux_wakeup_event(AONWUC_AUX_WAKEUP);
     while(!(ti_lib_aon_wuc_power_status_get() & AONWUC_AUX_POWER_ON)) ;
     ti_lib_aux_wuc_clock_enable(AUX_WUC_ADI_CLOCK | AUX_WUC_ANAIF_CLOCK | AUX_WUC_SMPH_CLOCK);
     while(ti_lib_aux_wuc_clock_status(AUX_WUC_ADI_CLOCK | AUX_WUC_ANAIF_CLOCK | AUX_WUC_SMPH_CLOCK) != AUX_WUC_CLOCK_READY) ;
     AUXADCSelectInput(ADC_COMPB_IN_AUXIO6);
     AUXADCEnableSync(AUXADC_REF_FIXED,  AUXADC_SAMPLE_TIME_2P7_US, AUXADC_TRIGGER_MANUAL);
     AUXADCGenManualTrigger();
     singleSample = AUXADCReadFifo();
     AUXADCDisable();
     printf("%d mv on ADC\r\n",singleSample);

     etimer_reset(&et_adc);
   }


  PROCESS_END();
}
