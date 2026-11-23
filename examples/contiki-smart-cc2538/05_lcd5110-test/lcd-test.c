

#include "contiki.h"
#include "cfs/cfs.h"
#include "cpu.h"

#include "sys/etimer.h"
#include "lcd5110.h"
#include "dev/cc2538-sensors.h"
#include "dev/button-sensor.h"
#include "dev/sys-ctrl.h"

#include <stdio.h> /* For printf() */


static char str[14] = "hello world";
static char *pstr;



/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
	int i =0;

  	PROCESS_BEGIN();

	initLcd();	

   while(1){
	
	
	PROCESS_WAIT_EVENT();
	if(ev == sensors_event){
	  if(data == &button_up_sensor){
		printf("up button pressed. \n");
		initLcd();
		LCD_write_english_string(0,0,"PANID: 0xABCD");
		LCD_write_english_string(0,1,"CHANNEL: 26");
		LCD_write_english_string(0,2,"RPL: ContikiMac");
//		LCD_write_english_string(0,3,"Row 4");
		LCD_write_english_string(0,4,"Temp: 18.2");
		LCD_write_english_string(0,5,"Voltage: 3.20");
	  }else if(data == &button_down_sensor){
		printf("down button pressed. \n");
//		LCD_clear();
		gpio_reset(LCD_SCL);
		gpio_reset(LCD_DC);
		gpio_set(LCD_DC);
		gpio_set(LCD_SCL);
		
	  }else if(data == &button_left_sensor){
		printf("left button pressed. \n");
		initLcd();
		LCD_set_XY(0,0);
//		i = 0;
//		while(str[i]){
		for(i=0;i<strlen(str);i++){
   	 	LCD_write_char(str[i]);
		printf("%c",str[i]);
//		 str++;
//		 i ++;
//		 if(i >=14) return;
		}
	  }else if(data == &button_right_sensor){
		printf("down button pressed. \n");
		LCD_set_XY(0,0);
		LCD_write_char('A');
		LCD_set_XY(1,1);
		LCD_write_char('B');
		LCD_set_XY(2,2);
		LCD_write_char('C');
		LCD_set_XY(3,3);
		LCD_write_char('D');
		LCD_set_XY(4,4);
		LCD_write_char('E');
		LCD_set_XY(5,5);
		LCD_write_char('F');
		LCD_set_XY(6,0);
		LCD_write_char('G');
	   
	  }else if(data == &button_select_sensor){
		printf("left button pressed. \n");
		LCD_write_char('A');
		LCD_write_char('E');
//		LCD_write_byte(0xff, 1);
//		LCD_write_byte(0xff, 1);
//		LCD_write_byte(0xff, 1);
//		LCD_write_byte(0xff, 1);
//		LCD_write_byte(0xff, 1);
//		LCD_write_byte(0xff, 1);
	   }

	}else if(ev == PROCESS_EVENT_TIMER){
	}
      
    }

  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
