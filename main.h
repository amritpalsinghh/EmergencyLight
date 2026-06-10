/*
 * main.h
 *
 * Created: 02/22/18 6:50:17 PM
 *  Author: AMRIT
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include "../Inc_Config_Files/Config_hw_set.h"
#include "../Inc_Config_Files/Conf_sys_set.h"
#include "../Inc_Config_Files/rtc.h"
 

//Custom Conditions as per Specifications 
#define _110V_DC_APPLIED						ob->flag.supp_Stat_f //0 Supply off | 1 supply ON
#define _110V_DC_NOT_APPLIED					!ob->flag.supp_Stat_f 
#define BATT_INTECT								ob->batt.c_volt>=900
#define BATT_NOT_INTECT							ob->batt.c_volt<900
#define BATT_UNHEALTHY_AND_NEED_REPLACEMENT		ob->flag.batt_bad_health_f==1
#define BATT_VOLT_BELOW_10_5                    ob->batt.c_volt<1050
#define BATT_VOLT_ABOVE_10_5                    ob->batt.c_volt>=1050
#define UNIT_BATT_DISCHARGING_AT_20PER          ob->flag.cyclic20per==1

#define TRUE	1
#define FALSE 	0

#define NOPE	asm("nop")

#define NULL    ((void *)0)
//bitwise
#define BV(m)	(1<<m)
#define BVM(m)	(0<<m)



//**********************************************************************	
 //************************** macros ***********************************


#define MASK_B	(BV(RED)			| BV(GREEN) | BV(AMBER) )
#define MASK_C	(BV(DUMMY_LOAD))
#define MASK_D   (BV(H_LUX)	| BV(CHARGING) | BV(AMBER_C)) | BV(WDT)

//PROTOTYPES
//**************** Function Prototype ***********************************
//enumerator for Ds1307 RTC base address
enum RTC_TEST_DATE
{
	Charg1stDOM_START = 0,
	Charg1stDOM_END,
	Charg16thDOM_START,
	Disch1stDOM_END,
	Disch16thDOM_START,
	CURRENTDATE_END
};
typedef struct STR_NUMB
{
	uint8_t d1:4;
	uint8_t d2:4;
	uint8_t d3:4;
	uint8_t d4:4;
}STR_NUMB;
typedef struct STR_F
{
	uint8_t above_14_4_volt_f		;
	uint8_t bw_13_5___14_4_volt_f	;
	uint8_t bw_10_5___13_5_volt_f	;
	uint8_t bw_9___10_5_volt_f		;
	uint8_t below_9_volt_f			;
	
	uint8_t lvbAtnosupply			;
	uint8_t lvbAtsupply				;
	uint8_t lvb_soft_stateATnosupply;
	uint8_t batt_bad_health_f		;
 
	
	uint8_t FullLux_f				;
	uint8_t Charg_1_16th_f			;
	uint8_t DisCharg_1_16th_f		;
	

	uint8_t cluster_f				;
	uint8_t Full_Lux_Two_H_f		;
	uint8_t Full_Lux_Two_H_end_f	;
	
 
	uint8_t supp_Stat_f             ;
	uint8_t ChargEnable_f			;
	uint8_t DischEnable_f			;
	uint8_t cyclic20per				;
	uint8_t reading_f				;
	uint8_t lower_cut_on_f          ;
	
	 

	
	uint8_t test1day1min_f			;
	uint8_t testdebug_f				;
	uint8_t curr_add_to_batt        ;
}STR_F;
typedef struct STR_TIME
{
	uint8_t SEC	: 6; //59 =  11 1011
	uint8_t MIN	: 6; //59 =  11 1011
	uint8_t HOUR: 5; //24 =   1 1000
}STR_TIME;
typedef struct STR_DATE
{
	uint8_t DAY	    : 3; // 7 =      111
	uint8_t DOM	    : 6; //31 =   1 1111
	uint8_t MONTH	: 4; //12 =     1100
	uint8_t YEAR	: 7; //99 = 110 1100
}STR_DATE;

typedef struct STR_BATT
{
	uint32_t	c_volt			;	// Current Voltage // Read the Voltage b/w Batt Positive Terminal and GND of Board     
	uint32_t	c_curr			;	// Current_Current //Measure only Charging Current in miliAmper
	uint32_t	c_supp_volt		;	// Current Supp V  //Voltage at output of supply which will charge the battery
	uint8_t		sampling_time	;	//				   //Time interval to calculate the dv/dt 
 	uint32_t	volt_at_T0		;	//				   //CURRENT Voltage when sampling time start
	uint32_t	volt_at_T1		;	//				   //AFTER SAMPLING INTERVEL get ends
	uint32_t	change_in_BV	;	//				   //Change in voltage level during sampling time  
    uint8_t		disch_f			;	//				   //During Sampling time battery voltage level get lower 
	uint8_t		charg_f			;	//				   //During Sampling time battery voltage level get rise 
	uint32_t	dvdt			;	// 'dv/dt'         // Rate of change of voltage of battery during charging 

}STR_BATT;
typedef struct STR_CAL
{
	STR_TIME time		;
	STR_DATE date		;
	uint8_t old_saved_day; // It save the current day  when we under go to Test (1Day = 1 Min ), This Saved value will again used when we go out of this Test for normal process| when we are out of Test then it is remain 0
	uint8_t old_saved_dom; // It save the current date when we under go to Test (1Day = 1 Min ), This Saved value will again used when we go out of this Test for normal process| when we are out of Test then it is remain 0
	uint8_t old_saved_month;
	uint8_t old_saved_year;
}STR_CAL;
typedef struct STR_CHARG
{
	uint8_t time;
}STR_CHARG;
typedef struct STR_FULL_LUX_TIME
{
  
	uint8_t start_min	;
	uint8_t start_hour	;
	uint8_t allowd_hour	;
	uint8_t allowd_min	;
	uint8_t end_min		;
    uint8_t end_hour	;
}STR_FULL_LUX_TIME;
typedef struct STR_DLTime 
{
	
	uint8_t start_min	;
	uint8_t start_hour	;
	uint8_t allowd_hour	;
	uint8_t allowd_min	;
	uint8_t end_min		;
	uint8_t end_hour	;
}STR_DLTime;
typedef struct STR_20PER_TIME
{
  
	uint8_t start_min	;
	uint8_t start_hour	;
	uint8_t allowd_hour	;
	uint8_t allowd_min	;
	uint8_t read_min	;                
    uint8_t read_hour	;
	uint8_t StartVolt	;
}STR_20PER_TIME;
typedef struct STR_DEBUG // Debug object
{
	uint8_t allowed_hour	;   // Time after which the readings get logged
	uint8_t allowed_min		;   //
	uint8_t allowed_sec		;
	uint8_t next_reading_hour;
	uint8_t next_reading_min;
	uint8_t next_reading_sec;
}STR_DEBUG ;
typedef struct STR_MPOBJ// main process object
{
	STR_F				flag			;
	STR_CAL				calan			;
	STR_BATT			batt			;
	STR_FULL_LUX_TIME	FullLuxTime		;
	STR_DLTime			cl_fl_rm_lvb	; // "Cluster flashing remove at lvb" using dummy load 
	STR_20PER_TIME		battdisch20per	;
	STR_DEBUG			reading			; // Object deal with the reading intervels
}STR_MPOBJ;
 



void USART_Init( unsigned int ubrr );
void USART_Transmit( unsigned char data );
void USART_Transmit_str( unsigned char * str );
unsigned char USART_Receive( void );

STR_MPOBJ * update_obj		(STR_MPOBJ * ob);
STR_MPOBJ * system_init		(STR_MPOBJ * ob);
void		debug_obj		(STR_MPOBJ *ob,uint8_t * str);
void		debug_num		(uint32_t no,uint8_t f,uint8_t digit,uint8_t new_line,int8_t *str);

STR_MPOBJ * debug_process	(STR_MPOBJ * ob);           // Main Function which have all sub_debug_fucntions



STR_MPOBJ * init_STR_MPOBJ	(STR_MPOBJ *ob);            // Initialize the objects parameters 
STR_MPOBJ * dv_dt			(STR_MPOBJ * ob);			// To detect Rate of change of Battery 
STR_MPOBJ * batt_health_f_check(STR_MPOBJ * ob);			// To Check Battery health
STR_MPOBJ * charging		(STR_MPOBJ * ob);			//

STR_MPOBJ * above_14_4_volt_f_state_check(STR_MPOBJ * ob);  // Is that Battery voltages reaches above 14.4Volt while charging?
STR_MPOBJ * bw_13_5___14_4_volt_f_state_check(STR_MPOBJ * ob);  // Is that BattVolt in b/w 13.5volt and 14.4 Volt?
STR_MPOBJ * bw_10_5___13_5_volt_f_state_check(STR_MPOBJ * ob);  // Is that BattVolt in b/w 10.5volt and 13.5 Volt?
STR_MPOBJ * bw_9___10_5_volt_f_state_check(STR_MPOBJ * ob);  // Is that BattVolt in b/w 9volt and 10.5 Volt?
STR_MPOBJ * below_9_volt_f_state_check(STR_MPOBJ * ob);  // Is that BattVolt below 9volt?
STR_MPOBJ * SaveTimeOfStartOfFullLux(STR_MPOBJ * ob);     // Save Time When The Full lux get started 
STR_MPOBJ * GetTimeElepsedOfFullLux(STR_MPOBJ * ob);  // Time elepsed of Full Lux Start
STR_MPOBJ * CalculateEndTimeOfFullLux(STR_MPOBJ * ob);// After getting FullLuxStartTime we will calculagte the End Time 
uint8_t     difference_in_no(uint8_t no1,uint8_t no2); // To get Difference b/w two no
STR_MPOBJ * Charging(STR_MPOBJ * ob);// check the charging conditions only
STR_MPOBJ * Handle2HourFullLuxCond(STR_MPOBJ * ob);// Handle2HourFullLuxCond
STR_MPOBJ * HandleCharg_1_16th(STR_MPOBJ * ob);// Handle
STR_MPOBJ * HandleDisch_1_16th(STR_MPOBJ * ob);// Handle
STR_MPOBJ * TestTimeSimMinEqDay(STR_MPOBJ * ob);// 15 Days = 15 minute

STR_MPOBJ * cyclic_discharge_20per_batt(STR_MPOBJ * ob);  //
STR_MPOBJ * get_Time_Date_form_RTC(STR_MPOBJ * ob);     // Get Date and time form RTC and update the Objects variales
STR_MPOBJ * debug_date_n_time(STR_MPOBJ * ob);			// will print date and time on UART
void Update_Date_n_time_dec2bcd_to_rtc(uint8_t day_t,uint8_t dom_t,uint8_t month_t,uint8_t year_t,uint8_t hour_t,uint8_t min_t,uint8_t sec_t);        // Update the RTC Calander with the mannual feed date and times
uint8_t		get_bcd2dec		(uint8_t add);				// Reading from RTC
uint8_t		get_dec2bcd		(uint8_t no);				// Writing to   RTC
STR_MPOBJ * cluster_cnt(STR_MPOBJ * ob,uint8_t stat);
STR_MPOBJ * CalculateNextReaingTime(STR_MPOBJ * ob);// Time of next reading to be taken

STR_MPOBJ * ConnectorP3_Rev2_MainBoardFunct(STR_MPOBJ * ob);//  Debug and Testing Handling
STR_MPOBJ * save_current_day_dom(STR_MPOBJ * ob);// This will Save the Current Day and Dom Which helps to update the RTC when we come out of Test (1Day = 1Min)
STR_MPOBJ * update_batt_curr_volt(STR_MPOBJ * ob); 
 
//STR_MPOBJ * startup_supply_control(STR_MPOBJ *  ob);  // Supply Control at startup :  
//STR_MPOBJ * Cluster_flik_cnt_at_10_5V_(STR_MPOBJ * ob);//  Flacutation of Cluster at 10.5 will fixed here


#endif /* MAIN_H_ */
