/*
 * ELU SW_VER1.0.c
 *
 * Created: 02/22/18 6:47:12 PM
 * Author : AMRIT
 */ 

//;*************** Header Files Included ********************************

#include "main.h"
#include "AtmegaDriver.h"
#include <stdio.h>
//*************************** global variable ***************************/
uint16_t Timer = 0; 
#define RXEN 4
#define TXEN 3
#define USB 3
#define UCSZ0 1
#define UDRE 5
#define RXC 7
#define USBS 3     
//#define FOSC 8000000// Clock Speed
#define F_CPU 8000000UL   // or 16000000UL
#define BAUD 9600
//#define MYUBRR ((F_CPU/16)/BAUD)-1
#define MYUBRR ((F_CPU/16/BAUD)-1)

#define SV_ADC7_NO 1700
 int ml=0;
uint32_t adcc(uint8_t adcno)
{
	uint32_t val;
	
	ADMUX=adcno;
	ADCSRA = _BV(ADEN) | _BV(ADPS2)| _BV(ADPS1)| _BV(ADPS0);   
   		
	ADCSRA |= _BV(ADSC);
    while (ADCSRA & _BV(ADSC) ) {}     								
    val = ADCW;// 10 bit adc value
	return val;
}
 //uint8_t charg_f=0;
 //uint8_t disch_f=0;
//#define  UPDATE_RTC
//*************************** mains ************************************
int main()
	{
		int j=0;
		uint8_t ttemp=0;
    STR_MPOBJ * ob;
	ob=(STR_MPOBJ *)malloc(sizeof(STR_MPOBJ));
	memset(ob, 0, sizeof(ob));
	ob=system_init(ob);
    //debug_obj(obj  ,"Flag's Status");
	//debug_num(123,3,0,"Number=");
	 debug_device_signature();
	
	
	while(1)				  
	//{ DUMMY_LOAD_ON}
		{
		ob=update_obj(ob);
		//ob=startup_supply_control(ob);     // For Supply Startup proper (can sustain Load of 800mA)at LowerCutON 80V 
		//ob=Cluster_flik_cnt_at_10_5V_(ob); //  Flacutation of Cluster at 10.5 will fixed here
		if(  BATT_UNHEALTHY_AND_NEED_REPLACEMENT		     )
		{
			GREEN_ON	    RED_FL		AMBER_OFF
			  ob->flag.cluster_f=0;HIGH_LUX_OFF ob->flag.Full_Lux_Two_H_f=0; //// Pending By checking dv/dt at 20% : it is checked only by software
			DUMMY_LOAD_OFF ob->flag.DischEnable_f=0;
		}
		else
		{
 
			if(ob->flag.below_9_volt_f==0)
			{
				if(  BATT_INTECT	 && _110V_DC_APPLIED 		 ) {GREEN_ON	RED_OFF		AMBER_OFF		ob=cluster_cnt(ob,0);	}   // FillFullBySoftware
				if(  BATT_NOT_INTECT && _110V_DC_APPLIED 		 ) {GREEN_OFF	RED_OFF		AMBER_OFF		ob=cluster_cnt(ob,0);	}   // As Power Off the MCU VCC Turned OFF  and ALL LEDS get OFF (? AMBER WORKS DEFAULT)
				//	if(  BATT_NOT_INTECT && _110V_DC_NOT_APPLIED ) {GREEN_OFF	RED_OFF		AMBER_OFF		ob=cluster_cnt(ob,0);	}   // sYSTEM GET OFF
				if(  BATT_INTECT		&&  _110V_DC_NOT_APPLIED ) {GREEN_ON    RED_ON		AMBER_OFF       ob=cluster_cnt(ob,1);	}   // FillFullBySoftware (LED CONTROL BY SOFT BUT CLUSTER CONTROL BY BOTH)
					
				if(  UNIT_BATT_DISCHARGING_AT_20PER   &&ob->flag.DisCharg_1_16th_f  &&	_110V_DC_APPLIED	 ) {GREEN_FL	RED_OFF		AMBER_OFF								}	// Pending
				if(  UNIT_BATT_DISCHARGING_AT_20PER   &&ob->flag.DisCharg_1_16th_f  &&	_110V_DC_NOT_APPLIED	 ) {GREEN_FL	RED_ON		AMBER_OFF								}	// Pending
			 
				//if(  MICROPROCESSOR FAILURE             		 ) {GREEN_OFF	RED_OFF		AMBER_ON								}	// By Removing The VCC of MCU ONLY or by Erase Program Memory
				//if(  110V_DC_NOT_APPLIED                		 ) {GREEN_ON	RED_ON		AMBER_OFF		ob=cluster_cnt(ob,1);	}   // Checked by Condition 'COND4'  //Monitored by Hardware by TP8:0 Supply On Vice versa
				//if(  CHARGING_CIRCUIT_FAILURE              	 ) {GREEN_OFF	RED_ON		AMBER_OFF		ob=cluster_cnt(ob,1);	}   // Checked by Condition 'COND4'
				//if(  INPUT_DC<80                         		 ) {GREEN_ON	RED_ON		AMBER_OFF		ob=cluster_cnt(ob,1);	}   // Checked by Condition 'COND4'  //By Setting Lower Cutt ON by Hardware
				//if(  INPUT_DC>80                        		 ) {GREEN_ON	RED_OFF		AMBER_OFF		ob=cluster_cnt(ob,0);	}   // Checked by Condition 'COND1'  //Both Conditon is monitored by Hardware by TP8:0 Supply On Vice versa
				//if(  BATT_UNHEALTHY_AND_NEED_REPLACEMENT		 ) {GREEN_ON	RED_FL		AMBER_OFF		ob=cluster_cnt(ob,0);	}   // Pending By checking dv/dt at 20% : it is checked only by software
				if(  ob->flag.lvbAtsupply  )					{GREEN_ON	RED_OFF		AMBER_FL		ob=cluster_cnt(ob,0);	}   // Checked by Condition 'COND1'
				if(  ob->flag.lvbAtnosupply)						{GREEN_ON	RED_ON		AMBER_FL		ob=cluster_cnt(ob,0);	}   // Checked by Condition 'COND1'
			}
			else
			{
				GREEN_OFF	RED_OFF		AMBER_OFF
				  ob->flag.cluster_f=0;HIGH_LUX_OFF ob->flag.Full_Lux_Two_H_f=0; //// Pending By checking dv/dt at 20% : it is checked only by software
				DUMMY_LOAD_OFF ob->flag.DischEnable_f=0;
			}
			


		//	ob=Charging(ob);
 
								 
			 ob=Handle2HourFullLuxCond(ob);
			if(ob->calan.date.DOM==1 || ob->calan.date.DOM==16)
			{
				
				ob=HandleDisch_1_16th(ob);
				ob=HandleCharg_1_16th(ob);
				ob=dv_dt(ob);
				ob=batt_health_f_check(ob);					// To detect Rate of change of Battery
			}
			else
			{
				ob=dv_dt(ob);
				ob=batt_health_f_check(ob);					// To detect Rate of change of Battery
				ob->flag.DisCharg_1_16th_f=0;
				DUMMY_LOAD_OFF
				CHARGING_ON 
				
			}

			ob=ConnectorP3_Rev2_MainBoardFunct(ob);
			
		}
	}


return 0;
}
 
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
 STR_MPOBJ * startup_supply_control(STR_MPOBJ *  ob)
{
	    int m=0;
		if(  ob->flag.lower_cut_on_f==0 || ob->batt.c_supp_volt<=1300)
		{
			ob->flag.supp_Stat_f=0; 	
			CHARGING_OFF
		//	ob->flag.ChargEnable_f=0;
		}
		
		if(  ob->flag.lower_cut_on_f==1 && ob->flag.supp_Stat_f==0)//ie more than
		{ 		
			CHARGING_OFF
		//	ob->flag.ChargEnable_f=0;
			ob->batt.c_supp_volt=(adcc(7)*SV_ADC7_NO)/1023;	// Supply Voltage which charges before Diode D4 in ELU_Main_PCB REV_V1	
				ob->flag.supp_Stat_f= 1;  
			for(m=0;m<5 && ob->flag.supp_Stat_f==1 ;m++)
			{
			
				_delay_ms(200);
				ob->batt.c_supp_volt=(adcc(7)*3140)/1023;	// Supply Voltage which charges before Diode D4 in ELU_Main_PCB REV_V1	
				if(ob->batt.c_supp_volt>1200)  
					ob->flag.supp_Stat_f =ob->flag.supp_Stat_f & 1; 
				else 
					ob->flag.supp_Stat_f =ob->flag.supp_Stat_f & 0;
					
			 	USART_Transmit( '\n');
				debug_num(ob->flag.supp_Stat_f,0,1,0,"BV: ");
				debug_num(ob->batt.c_supp_volt,1,4,0," SV=");
				debug_num(ob->flag.ChargEnable_f,1,1,0," Ch:");	 // Charging
			}
		}
return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * save_current_day_dom(STR_MPOBJ * ob)// This will Save the Current Day and Dom Which helps to update the RTC when we come out of Test (1Day = 1Min)
{
    ob->calan.old_saved_day=ob->calan.date.DAY;
	ob->calan.old_saved_dom=ob->calan.date.DOM;
	ob->calan.old_saved_month=ob->calan.date.MONTH;
	ob->calan.old_saved_year=ob->calan.date.YEAR;

	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * ConnectorP3_Rev2_MainBoardFunct(STR_MPOBJ * ob)
{
	static uint8_t temp=0;
	if(ob->flag.testdebug_f	)  // 1 : when Jumper at P3 is Connected with 'D'(Debug Start)  Otherwise 0 (No jumper ie No Debugging) | Only Normal Debugging
	ob=debug_process(ob);
	if(ob->flag.test1day1min_f	)  // 1 : when Jumper at P3 is Connected with 'T'(1Day=1Min Test along with Debug Start)  Otherwise 0 (No jumper ie Nothing Take Place)
	{
		if(temp==0)
		{
			ob=save_current_day_dom(ob);			//
			temp=1;
		}
		ob=TestTimeSimMinEqDay(ob);
	}
	else
	{	if(temp==1)
		{
			I2c_Write(day		,get_dec2bcd(ob->calan.old_saved_day));// link monday tusday etc
			I2c_Write(dom		,get_dec2bcd(ob->calan.old_saved_dom));
			I2c_Write(month		,get_dec2bcd(ob->calan.old_saved_month));
			I2c_Write(year		,get_dec2bcd(ob->calan.old_saved_year));
		}
		
		ml=0;
		ob->calan.old_saved_day=0;
		ob->calan.old_saved_dom=0;
		ob->calan.old_saved_month=0;
		ob->calan.old_saved_year=0;
		temp=0;
	}
	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * cluster_cnt(STR_MPOBJ * ob,uint8_t stat)
{
/*	if(!ob->flag.below_9_volt_f && !ob->flag.bw_9___10_5_volt_f && stat==1 && ob->flag.cluster_cnt_At_10_5_f==0 )
	{
		CLUSTER_ON
		ob->flag.cluster_f=1;
//		HIGH_LUX_ON
//		ob->flag.Full_Lux_Two_H_f=0;
	}
	else
	{
		CLUSTER_OFF 
		ob->flag.cluster_f=0;
		HIGH_LUX_OFF
		 
		ob->flag.Full_Lux_Two_H_f=0;
		ob->flag.Full_Lux_Two_H_end_f=0;

		if(ob->flag.lvbAtnosupply==1 )AMBER_FL;
		
		ob->FullLuxTime.start_hour=0;
		ob->FullLuxTime.start_min=0;	
		ob->FullLuxTime.end_hour=0;
		ob->FullLuxTime.end_min=0;
	}	*/
	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * Handle2HourFullLuxCond(STR_MPOBJ * ob)// Handle2HourFullLuxCond
{
	
	//if(!ob->flag.supp_Stat_f  && !ob->flag.below_9_volt_f && !ob->flag.bw_9___10_5_volt_f && ob->flag.cluster_f==1)
//	if(CLUSTER_STAT_PIN==0)// cluster ON
	//{
		static uint8_t ttemp=1;
		int j=0;
 
				 		 
	 
		
	 if(ob->flag.cluster_f==1 && ttemp==1)
	 {
 
		  if(ob->flag.cluster_f==1 && ttemp==1)
		  {
 
			  		 if(ob->flag.cluster_f==1 && ttemp==1)
			  		 {
							if(ob->flag.Full_Lux_Two_H_f==0 && ob->flag.Full_Lux_Two_H_end_f==0  )
							{
							
	 
										ob->FullLuxTime.start_min=ob->calan.time.MIN;
									ob->FullLuxTime.start_hour=ob->calan.time.HOUR;			
									ob=CalculateEndTimeOfFullLux(ob);
									HIGH_LUX_ON
									ob->flag.Full_Lux_Two_H_f=1;
			 
		
								}
 
								if(ob->FullLuxTime.end_hour==ob->calan.time.HOUR && ob->FullLuxTime.end_min==ob->calan.time.MIN    )
								{
									HIGH_LUX_OFF
									ob->flag.Full_Lux_Two_H_end_f=1;
									ob->flag.Full_Lux_Two_H_f=0;
		 
									ob->FullLuxTime.start_hour=0;
									ob->FullLuxTime.start_min=0;
			 
											ob->FullLuxTime.end_hour=0;
									ob->FullLuxTime.end_min=0;
	 
								}
					   }
		  
     }
	 }
 else
	{
		            HIGH_LUX_OFF
					ob->flag.Full_Lux_Two_H_f=0;
					ob->flag.Full_Lux_Two_H_end_f=0;
					ob->FullLuxTime.start_hour=0;
					ob->FullLuxTime.start_min=0;
					ob->FullLuxTime.end_hour=0;
					ob->FullLuxTime.end_min=0;
					       
				
	} 
 
	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * cyclic_discharge_20per_batt(STR_MPOBJ * ob)
{
	uint8_t s_min,s_hour;
	uint8_t a_min,a_hour;
	uint8_t r_hour,r_min;
	s_hour=ob->battdisch20per.start_hour;
	s_min	=ob->battdisch20per.start_min;
	a_hour=ob->battdisch20per.allowd_hour;
	a_min	=ob->battdisch20per.allowd_min;
	
	r_hour=s_hour+a_hour;
    r_min=s_min+a_min;
	if(r_min>59)
	{
		r_min=r_min-60;
		r_hour++;
		
	}
	if(r_hour>23)
	{
		r_hour=r_hour-23;
	}
 
 
	ob->battdisch20per.read_hour=r_hour;
	ob->battdisch20per.read_min=r_min;	
	return ob;
	 
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * TestTimeSimMinEqDay(STR_MPOBJ * ob)// 15 Days = 15 minute
{

	 static int temp=0;
	 static int day_t=0;
	 static int dom_t=0;
	 static int month_t=0;
	 static int min_t=0;
	 static int year_t=0;
	 if(ml==0)temp=0;
	 if(temp==0)
	 {
	/*	 dom_t=ob->calan.date.DOM;
		 month_t=ob->calan.date.MONTH;
		 year_t=ob->calan.date.YEAR;
		 min_t=ob->calan.time.MIN;
	*/
		  dom_t=ob->calan.old_saved_dom;
		  month_t=ob->calan.old_saved_month;
		  year_t=ob->calan.old_saved_year ;

		  min_t=ob->calan.time.MIN;
		 temp=1;
		 ml=1;
		
	 }
	 if(min_t!=ob->calan.time.MIN)
	 {
		 
		 dom_t=ob->calan.date.DOM;
		 dom_t++;
		 day_t++;
		 
		 
		 USART_Transmit_str("=================================================================================== ");
	    if(day_t>8)day_t=1;
		if(month_t!=2) 
		{
			if((month_t==1 )||( month_t==3 )||( month_t==5 )||( month_t==7 )||( month_t==8 )||( month_t==10 )||( month_t==12 ))
			{ 
				    if(dom_t>31)
					{
						dom_t=1;month_t++;
					}
 
			}
			else 
			{
				 if(dom_t>30)
				 {
					 dom_t=1;month_t++;
				 }
				 
			}

		}
		else
		{
			if(dom_t>28)
			{
				dom_t=1;month_t++;
			}
		}
		if(month_t>12)
		{
			 month_t=1;year_t++;
		}
		I2c_Write(day    	,get_dec2bcd(day_t));
		I2c_Write(dom		,get_dec2bcd(dom_t));
		I2c_Write(month		,get_dec2bcd(month_t));
		I2c_Write(year		,get_dec2bcd(year_t));
 
		 min_t=ob->calan.time.MIN;
	 }
	// if(inputpin==0)
	// {
	//	 temp=0;
	// }
	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * HandleDisch_1_16th(STR_MPOBJ * ob)// Handle 
{       
	    
		if(ob->calan.time.HOUR>=7 && ob->calan.time.HOUR<12    )   // Condition for Discharging
		{
					CHARGING_OFF
					
				//	 ob->flag.ChargEnable_f 
				     
					_delay_ms(100);
					ob->battdisch20per.StartVolt=ob->batt.c_volt;
					if(BATT_STAT_BELOW_10_5_PIN==0 && ob->flag.cluster_f==0)
					{
						DUMMY_LOAD_ON	
						ob->flag.DischEnable_f=1;
					}
					else
					{
						DUMMY_LOAD_OFF
						ob->flag.cyclic20per=0;
						ob->flag.DischEnable_f=0;
						ob->flag.DisCharg_1_16th_f=1;
						
					}
					ob->flag.DisCharg_1_16th_f=1;
		            ob->flag.cyclic20per=1;
					_delay_ms(100);
					 
		}
		else
		{
			DUMMY_LOAD_OFF
			ob->flag.cyclic20per=0;
			ob->flag.DischEnable_f=0;
			ob->flag.DisCharg_1_16th_f=1;
			CHARGING_ON
			//ob->flag.ChargEnable_f=1;
		}
	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * HandleCharg_1_16th(STR_MPOBJ * ob)// Handle 
{
		if(ob->calan.time.HOUR>=12 && ob->calan.time.HOUR<23) // Condition for Charging
		{
			if(ob->calan.time.MIN<=59)
			{
					DUMMY_LOAD_OFF	
					ob->flag.DischEnable_f=0;
					_delay_ms(100);
					CHARGING_ON
					//ob->flag.ChargEnable_f=1;
					_delay_ms(100);
			}
			
			ob=cyclic_discharge_20per_batt(ob);
		}

	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * Charging(STR_MPOBJ * ob)// check the charging conditions only
{
	if( ob->flag.supp_Stat_f && !ob->flag.DischEnable_f)
	{
		
		CHARGING_ON
	//	ob->flag.ChargEnable_f=1;
		_delay_ms(100);
	}
	else
	{
		CHARGING_OFF
	//	ob->flag.ChargEnable_f=0;
		_delay_ms(100);
	}
	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * CalculateNextReaingTime(STR_MPOBJ * ob)// Time of next reading to be taken
{
	uint8_t s_hour,s_min,s_sec;
	uint8_t a_hour,a_min,a_sec;
	uint8_t n_hour,n_min,n_sec;
	
	s_hour=ob->calan.time.HOUR;
	s_min=ob->calan.time.MIN;
	s_sec=ob->calan.time.SEC;
	
	a_hour=ob->reading.allowed_hour;
	a_min=ob->reading.allowed_min;
	a_sec=ob->reading.allowed_sec;
	
	n_hour=s_hour+a_hour;
	n_min=s_min+a_min;
	n_sec=s_sec+a_sec;
	if(n_sec>59)
	{
		n_sec=n_sec-60;
		n_min++;
		
	}
	if(n_min>59)
	{
		n_min=n_min-60;
		n_hour++;
		
	}
	if(n_hour>23)
	{
		n_hour=n_hour-23;
	}
	
	
	ob->reading.next_reading_hour=n_hour;
	ob->reading.next_reading_min=n_min;
	ob->reading.next_reading_sec=n_sec;
	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * CalculateEndTimeOfFullLux(STR_MPOBJ * ob)// After getting FullLuxStartTime we will calculagte the End Time 
{
	uint8_t s_min,s_hour;
	uint8_t a_min,a_hour;
	uint8_t e_hour,e_min;
	s_hour=ob->FullLuxTime.start_hour;
	s_min=ob->FullLuxTime.start_min;
	a_hour=ob->FullLuxTime.allowd_hour;
	a_min=ob->FullLuxTime.allowd_min;
	
	e_hour=s_hour+a_hour;
    e_min=s_min+a_min;
	if(e_min>59)
	{
		e_min=e_min-60;
		e_hour++;
		
	}
	if(e_hour>23)
	{
		e_hour=e_hour-23;
	}
 
 
	ob->FullLuxTime.end_hour=e_hour;
	ob->FullLuxTime.end_min=e_min;	
	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
uint8_t difference_in_no(uint8_t no1,uint8_t no2) // To get Difference b/w two no
{
	if(no1==no2)
	return 0;
	if(no1>no2)
	{
		return no1-no2;
	}
	else
	return no2-no1;
}
 /*===============================================================
 // Function Name:
 // Why Req      :
 // Description  :
 // Note         :
 //===============================================================*/
STR_MPOBJ * SaveTimeOfStartOfFullLux(STR_MPOBJ * ob) // time check of two hours for full lux 
{
   
	ob->FullLuxTime.start_hour=ob->calan.time.HOUR;
	ob->FullLuxTime.start_min=ob->calan.time.HOUR;
	ob->flag.FullLux_f=1;
	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * above_14_4_volt_f_state_check(STR_MPOBJ * ob)  // Is that Battery voltages reaches above 14.4Volt while charging?
{
	if(TRICKLE_STATE_IP_HARDWARE_PIN==1)     //14.4 volt
	{
		ob->flag.above_14_4_volt_f=1;
		ob->flag.above_14_4_volt_f=1;
	}
	else 
	ob->flag.above_14_4_volt_f=0;
	
	//if(ob->flag.bw_10_5___13_5_volt_f  && ob->flag.above_14_4_volt_f)     //13.5 volt    + Once the Batt hase reached Reaches above 14.4volt?
	//ob->flag.above_14_4_volt_f=0;
	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * bw_13_5___14_4_volt_f_state_check(STR_MPOBJ * ob)  // Is that BattVolt in b/w 13.5volt and 14.4 Volt?
{
	if(ob->batt.c_volt>=1350 && ob->batt.c_volt<1440)
	ob->flag.bw_13_5___14_4_volt_f=1;
	else
	ob->flag.bw_13_5___14_4_volt_f=0;
	return ob;
}
STR_MPOBJ * bw_9___10_5_volt_f_state_check(STR_MPOBJ * ob)  // Is that BattVolt in b/w 9volt and 10.5 Volt?
{
/*	if(ob->batt.c_volt>=900 && ob->batt.c_volt<1050)
	ob->flag.bw_9___10_5_volt_f=1;
	else
	ob->flag.bw_9___10_5_volt_f=0;*/
   if(BATT_STAT_BELOW_10_5_PIN	){ob->flag.bw_9___10_5_volt_f=1;} else  {ob->flag.bw_9___10_5_volt_f=0;}
	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * bw_10_5___13_5_volt_f_state_check(STR_MPOBJ * ob)  // Is that BattVolt in b/w 10.5volt and 13.5 Volt?
{
	if(ob->flag.lvbAtnosupply || ob->flag.lvbAtsupply)
	{
		if(ob->batt.c_volt>=1050 && ob->batt.c_volt<1350 )
			ob->flag.bw_10_5___13_5_volt_f=1;
		else
			ob->flag.bw_10_5___13_5_volt_f=0;
	}
	else
	{
		ob->flag.bw_10_5___13_5_volt_f=0;
	}
		
		
	return ob;
}

STR_MPOBJ * below_9_volt_f_state_check(STR_MPOBJ * ob)  // Is that BattVolt below 9volt?
{
	if(ob->batt.c_volt<900)     //Below 9.00 volt
	ob->flag.below_9_volt_f=1;
	else
	ob->flag.below_9_volt_f=0;
	return ob;
}

STR_MPOBJ * lvb_flag_updation(STR_MPOBJ * ob)  //  
{
	if(ob->flag.bw_9___10_5_volt_f ||ob->flag.below_9_volt_f )
	{
		if(ob->flag.supp_Stat_f)//110  applied
		{
			ob->flag.lvbAtnosupply=0;
			ob->flag.lvbAtsupply=1;
		}
		if(!ob->flag.supp_Stat_f) //110 not applied
		{
			ob->flag.lvbAtsupply=0;		
			ob->flag.lvbAtnosupply=1;
		}
	}
	else
	{
				ob->flag.lvbAtsupply=0;		
			ob->flag.lvbAtnosupply=0;
	}
	
 
	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * batt_health_f_check(STR_MPOBJ * ob)					// To detect Rate of change of Battery 
{
		if(  ob->batt.dvdt>288)// When we OFF the supply Full BattVolt 14.4 then 20% 2.88 so no used is 288
		{
			ob->flag.batt_bad_health_f=1;
			debug_num(300,1,3,0,"!Set Value:");
			debug_num(ob->batt.volt_at_T0,1,4,0," Battey Bad Health Occur  | B0:");
			debug_num(ob->batt.volt_at_T1,1,4,0,"|B1:");
			USART_Transmit_str("|dvdt:");
			if(ob->batt.volt_at_T0==ob->batt.volt_at_T1 )USART_Transmit_str(" ");
			if(ob->batt.volt_at_T0<ob->batt.volt_at_T1)USART_Transmit_str("+");
			if(ob->batt.volt_at_T0>ob->batt.volt_at_T1)USART_Transmit_str("-");

			debug_num(ob->batt.change_in_BV,1,4,0,"");
			debug_num(ob->batt.sampling_time,1,1,0,"|");
			if(ob->batt.volt_at_T0==ob->batt.volt_at_T1 )USART_Transmit_str("= ");
			if(ob->batt.volt_at_T0<ob->batt.volt_at_T1)USART_Transmit_str("=+");
			if(ob->batt.volt_at_T0>ob->batt.volt_at_T1)USART_Transmit_str("=-");
			debug_num(ob->batt.dvdt,1,4,0,"");
			USART_Transmit_str( "\n");
		}
	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * dv_dt(STR_MPOBJ * ob)  // To detect Rate of change of Battery 
{
	
	//#define DEBUG_dv_dv													// Will Enable the Local Debugging
	
	uint8_t time=ob->batt.sampling_time;								// time is 3 second as above
	 
		//ob->batt.volt_at_T0=(adcc(2)*BATT_VOLT_ADC_CALLIBRATION_NO)/1023;							// Get Battery Voltage at start of time say at 0
		//ob=update_batt_curr_volt(ob);
		ob->batt.c_volt=(adcc(2)*BATT_VOLT_ADC_CALLIBRATION_NO_WHILE_CHARGING)/1023;			   
		ob->batt.volt_at_T0=ob->batt.c_volt;
		for(int i=0;i<time;i++)											// For Time = 1sec * "time"
		{
			_delay_ms(1000);											// 1 Second
			//	if(  ob->flag.cyclic20per==1  		 ) Green_flash();
		}
		//ob=update_batt_curr_volt(ob);
		ob->batt.c_volt=(adcc(2)*BATT_VOLT_ADC_CALLIBRATION_NO_WHILE_CHARGING)/1023;		 	 
		ob->batt.volt_at_T1=ob->batt.c_volt;//(adcc(2)*BATT_VOLT_ADC_CALLIBRATION_NO)/1023;							// Get Battery Voltage at start of time after sampling time
	
		
		if(ob->batt.volt_at_T0==ob->batt.volt_at_T1)				// No Change
		{
			ob->batt.dvdt=0;
			ob->batt.change_in_BV=0;
			ob->batt.disch_f	=0;							// 1: Discharging take place vice versa
			ob->batt.charg_f	=0;							// 1: Charging    take place vice versa
		}
		else
		{
			if(ob->batt.volt_at_T0 < ob->batt.volt_at_T1 )						// Charging
			{
				ob->batt.change_in_BV=ob->batt.volt_at_T1 - ob->batt.volt_at_T0;
				ob->batt.dvdt=ob->batt.change_in_BV/time;
				ob->batt.disch_f	=0;
						// 1: Discharging take place vice versa
				ob->batt.charg_f	=1;							// 1: Charging    take place vice versa

			}
			else if(ob->batt.volt_at_T0 > ob->batt.volt_at_T1 ) // Discharge
			{
				ob->batt.change_in_BV=ob->batt.volt_at_T0 - ob->batt.volt_at_T1;
				ob->batt.dvdt=ob->batt.change_in_BV/time;
				ob->batt.disch_f	=1;							// 1: Discharging take place vice versa
				ob->batt.charg_f	=0;							// 1: Charging    take place vice versa
			}
		}
 
	 

	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * update_batt_curr_volt(STR_MPOBJ * ob)
{	
 
	 
	 
		 
	 
	
 
 
	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * update_obj(STR_MPOBJ * ob)
{

	static int jk=0;
	int m=1;

	ob=get_Time_Date_form_RTC(ob);
	//ob=update_batt_curr_volt(ob);
	ob->batt.c_volt=(adcc(2)*BATT_VOLT_ADC_CALLIBRATION_NO_WHILE_CHARGING)/1023;		
	ob->batt.c_curr=(adcc(6)*BATT_CURR_ADC_CALLIBRATION_NO/1023);		// Battery Charging Current [ Note: it measure charging current consumption from batt to load ]									
	 
	 
	
 
    ob->batt.c_supp_volt=(adcc(7)*SV_ADC7_NO)/1023;	// Supply Voltage which charges before Diode D3 in ELU_Main_PCB REV_2_		
	//ob->batt.c_curr=(adcc(6));	
	//ob->batt.dvdt=0;                    ///////////////////////////////
	ob->batt.disch_f=0;                 //  To calculate the new sample of dv/dt
	ob->batt.charg_f=0;                 //  we reinitialize the variables
	ob->batt.sampling_time=SAMP_TIME;   //////////////////////////////////////////////////////////////////////////
	 
	ob=above_14_4_volt_f_state_check(ob);
	ob=bw_13_5___14_4_volt_f_state_check(ob);
	ob=bw_10_5___13_5_volt_f_state_check(ob);
	ob=bw_9___10_5_volt_f_state_check(ob);

	ob=below_9_volt_f_state_check(ob);
	ob=lvb_flag_updation(ob);
	  
 
		  
    if(!SUPPLY_STAT_IP_HARDWARE_PIN 	){ob->flag.supp_Stat_f   =1;} else  {ob->flag.supp_Stat_f   =0;}
	if(!LOWERCUTON_IP_HARDWARE_PIN	){ob->flag.lower_cut_on_f=1;} else  {ob->flag.lower_cut_on_f=0;}
		int f=0;
	while(f<10)
	{
		if(ob->flag.lvbAtnosupply==0 && ob->flag.supp_Stat_f==0)
		{
		
		ob->flag.cluster_f=1;
		_delay_ms(20);
		}
		else
		{
			ob->flag.cluster_f=0;
			HIGH_LUX_OFF ob->flag.FullLux_f=0;
			break;
		}	
		f++;
	}
	
	
		
	if(ob->batt.c_curr>3 && ob->flag.supp_Stat_f==1)
	{
		ob->flag.ChargEnable_f =1; 
		ob->flag.curr_add_to_batt=1 ;  // CHARGING ENABLE
	}
	else 
	{
		ob->flag.curr_add_to_batt=0;    // CHARGING OFF
		ob->flag.ChargEnable_f =0;
	}
	//if(ob->batt.c_supp_volt>1200	)ob->flag.supp_Stat_f   =1; else  ob->flag.supp_Stat_f   =0;
	
	
	 
		
			
 
 
	if(TEST_1DAY1MIN_EN_STATE_IP_HARDWARE_PIN) //Test Control
	{											//
		ob->flag.test1day1min_f=1;				//
		ob->flag.testdebug_f=1;					//
	}											//
	else                                        //
	{                                           //
		if(DEBUG_EN_STATE_IP_HARDWARE_PIN)      //
		ob->flag.testdebug_f=1;
		else
		ob->flag.testdebug_f=0;
		ob->flag.test1day1min_f=0;
	}

		
 
			
 
/*		debug_num(ob->batt.c_supp_volt,1,4,0,"SUP Volt=");
		//debug_num(ob->batt.c_volt,1,4,0," -- BV=");
		debug_num(LOWERCUTON_IP_HARDWARE_PIN,0,1,0," LCutON:");
		debug_num(ob->flag.supp_Stat_f,0,1,0," SUP:");
 		debug_num(ob->flag.ChargEnable_f,1,1,0," Ch:");	 // Charging																	//////////////////////////////////////////////////////
		debug_num(ob->flag.DischEnable_f,1,1,0,", DCh:"); // Discharge
		USART_Transmit_str( "\n");
*/
	return ob;
	
	
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * debug_date_n_time(STR_MPOBJ * ob)  // will print date and time on uart
{
	 
//	debug_num(ob->calan.date.DAY,0,1,0,"");// MONDAY TUSDAY ....
	debug_num(ob->calan.date.DOM,0,2,0,"");
	debug_num(ob->calan.date.MONTH,0,2,0,"/");
	debug_num(ob->calan.date.YEAR,0,2,0,"/");
	
	debug_num(ob->calan.time.HOUR,0,2,0,"|");// MONDAY TUSDAY ....
	debug_num(ob->calan.time.MIN,0,2,0,":");
    debug_num(ob->calan.time.SEC,0,2,0,":");
	USART_Transmit_str( "|");
 
	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * debug_process(STR_MPOBJ * ob)
{	//static unsigned temp=0;
	//static char temp_char=65;//A
	//static char temp_char_count=0;//A
	//static unsigned int num=0;
	
	//if(!ob->reading.next_reading_hour	&& !ob->reading.next_reading_min	&& !ob->reading.next_reading_sec)
	//{
		USART_Transmit( '\n');
		ob=CalculateNextReaingTime(ob);
		#ifdef DEBUG_RTC
		ob=debug_date_n_time(ob);
		#endif // _DEBUG_DATE_TIME
		if(ob->flag.supp_Stat_f==1) debug_num(1,1,1,0,"|S:");		else debug_num(0,1,1,0,"|S:");			// logic 0:OFF(print 1)  & 1:ON(pirnt 0)
		debug_num(ob->batt.c_supp_volt,1,4,0,"|SV:");
		debug_num(ob->batt.c_volt,1,4,0,"|BV:");
		USART_Transmit_str("|Ch:");
		if(ob->batt.c_curr==0          )
			USART_Transmit_str(" ");
		else
		{
			if(ob->flag.curr_add_to_batt==1)USART_Transmit_str("+");
			if(ob->flag.curr_add_to_batt==2)USART_Transmit_str("-");			
		}

		
 
		 
		debug_num(ob->batt.c_curr,0,3,0,"");
		debug_num(ob->flag.ChargEnable_f,1,1,0,"|Ch:");	 // Charging	
		if(ob->flag.DisCharg_1_16th_f==1 && ob->calan.time.HOUR>=7 && ob->calan.time.HOUR<12    )   // Condition for Discharging
		{
			debug_num(1,1,1,0,"|DM:"); // Discharging Mode	
		}
		else
		{
			debug_num(0,1,1,0,"|DM:"); // Discharging Mode
		}
																	//////////////////////////////////////////////////////
		debug_num(ob->flag.DischEnable_f,1,1,0,"|DL:"); // Dummy Load
		
		if(TRICKLE_STATE_IP_HARDWARE_PIN==0) debug_num(0,1,1,0,"[ T:");  else debug_num(1,1,1,0, "[ T:");    // logic 0:OFF(print 0)  & 1:ON(pirnt 1)
		debug_num(ob->flag.cluster_f,1,1,0,"|CL:");// Cluster on off
		
		
		debug_num(ob->flag.Full_Lux_Two_H_f,1,1,0,"|HL:");// High Lux
		debug_num(ob->flag.batt_bad_health_f,1,1,0,"|BBH:");// Battery Bad health
		
		#ifdef DEBUG_CLUSTER_2HOUR_FULL_LUX
		debug_num(ob->FullLuxTime.start_hour,1,2,0,"[ 2HFLT|S");																		//////////////////////////////////////////////////////
		debug_num(ob->FullLuxTime.start_min,1,2,0,":");
		debug_num(ob->FullLuxTime.allowd_hour,1,2,0," A");																		//////////////////////////////////////////////////////
		debug_num(ob->FullLuxTime.allowd_min,1,2,0,":");
		debug_num(ob->FullLuxTime.end_hour,1,2,0," E");																		//////////////////////////////////////////////////////
		debug_num(ob->FullLuxTime.end_min,1,2,0,":");
		USART_Transmit_str( "|");
		#endif // DEBUG_CLUSTER_2HOUR_FULL_LUX
		#ifdef DEBUG_BATT_DVDT

		debug_num(ob->batt.volt_at_T0,1,4,0,"|B0:");
		debug_num(ob->batt.volt_at_T1,1,4,0,",B1:");
		USART_Transmit_str(",dvdt:");
		if(ob->batt.volt_at_T0==ob->batt.volt_at_T1 )USART_Transmit_str(" ");
		if(ob->batt.volt_at_T0<ob->batt.volt_at_T1)USART_Transmit_str("+");
		if(ob->batt.volt_at_T0>ob->batt.volt_at_T1)USART_Transmit_str("-");

		debug_num(ob->batt.change_in_BV,1,4,0,"");
		debug_num(ob->batt.sampling_time,1,1,0,"/");
		if(ob->batt.volt_at_T0==ob->batt.volt_at_T1 )USART_Transmit_str("=  ");
		if(ob->batt.volt_at_T0<ob->batt.volt_at_T1)USART_Transmit_str("= +");
		if(ob->batt.volt_at_T0>ob->batt.volt_at_T1)USART_Transmit_str("= -");
		debug_num(ob->batt.dvdt,1,4,0,"");
	 
		#endif
		#ifdef 	DEBUG_BATT_LVB
		debug_num(ob->flag.lvbAtnosupply,1,1,0,"|lvb=");
		debug_num(ob->flag.lvbAtsupply,1,1,0,",lvbS=");
		#endif
		#ifdef 	DEBUG_FLAGS
		debug_num(ob->flag.below_9_volt_f,1,1,0,"|BF: ");
		debug_num(ob->flag.bw_9___10_5_volt_f,1,1,0,"");
		debug_num(ob->flag.bw_10_5___13_5_volt_f,1,1,0,"");
		debug_num(ob->flag.bw_13_5___14_4_volt_f,1,1,0,"");
		
		debug_num(ob->flag.above_14_4_volt_f,1,1,0,"");
 
	 
		 


		debug_num(ob->flag.lower_cut_on_f,0,1,1," LC:");
		 
	 
		#endif
	//	temp_char='A';
	//	temp_char_count=0;
	//	num=0;
		//debug_num(READING_TIME_H,1,1,0," DT->");
	//	debug_num(READING_TIME_M,1,1,0,":");
		//debug_num(READING_TIME_S,1,1,0,":");
		//USART_Transmit_str( " ");
/*		
	}
	else
	{
		
	/*	if(temp>20)
		{
			if(temp_char_count<4)
			{
				USART_Transmit( '>');
				temp_char_count++;
				temp=0;	
			}
			else
			{
				USART_Transmit( temp_char);
				temp_char_count=0;
				if(temp_char<'Z')
				temp_char++;
				else
				{
					 num++;
					 temp_char='A';		
					 debug_num(num,1,3,0,"_");
					 
				}
			}
		}
		temp++; 
	}

	while(ob->calan.time.HOUR==ob->reading.next_reading_hour	&& ob->calan.time.MIN==ob->reading.next_reading_min	&& ob->calan.time.SEC==ob->reading.next_reading_sec)
	{
		ob=get_Time_Date_form_RTC(ob);
		ob->reading.next_reading_hour=0;
		ob->reading.next_reading_min=0;
		ob->reading.next_reading_sec=0;
	}
*/	
	
	
	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * get_Time_Date_form_RTC(STR_MPOBJ * ob)
{
	ob->calan.time.SEC	=get_bcd2dec(sec	); 
	ob->calan.time.MIN	=get_bcd2dec(min	);
	ob->calan.time.HOUR	=get_bcd2dec(hour	);
	ob->calan.date.DAY	=get_bcd2dec(day	);
	ob->calan.date.DOM	=get_bcd2dec(dom	);
	ob->calan.date.MONTH=get_bcd2dec(month	);
	ob->calan.date.YEAR	=get_bcd2dec(year	);
	return ob;
}
void Update_Date_n_time_dec2bcd_to_rtc(uint8_t day_t,uint8_t dom_t,uint8_t month_t,uint8_t year_t,uint8_t hour_t,uint8_t min_t,uint8_t sec_t)
{

          I2c_Write(day		,get_dec2bcd(day_t));// linke monday tusday etc
		  I2c_Write(dom		,get_dec2bcd(dom_t	));
		  I2c_Write(month	,get_dec2bcd(month_t));
		  I2c_Write(year	,get_dec2bcd(year_t));
		  I2c_Write(hour	,get_dec2bcd(hour_t));
		  I2c_Write(min		,get_dec2bcd(min_t));
		  I2c_Write(sec		,get_dec2bcd(sec_t));
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
uint8_t get_bcd2dec(uint8_t add) // read form rtc
{
	uint8_t temp,d1,d2;
	//=12
	temp=I2c_Read(add);
	d1=temp%16;//2
	d2=temp/16;
	temp=d2*10+d1;
	return temp;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
uint8_t get_dec2bcd(uint8_t no) // write to rtc
{
	uint8_t d1,d2;
	//=15						Decimal No
	d1=no%10;			//5
	d2=no/10;			//1
	d2=d2<<4;
	no=d1|d2;		//90     ie 0x90  BCD No
	return no;
}
 /*===============================================================
 // Function Name:
 // Why Req      :
 // Description  :
 // Note         :
 //===============================================================*/
STR_MPOBJ * system_init(STR_MPOBJ * ob)
{
	
	//GPIO MASKING 
	DDRC |=0x00;
	DDRB |=0x00;
	DDRD |=0x00;
	DDRC |= (MASK_C);
	DDRB |= (MASK_B);
	DDRD |= (MASK_D);	
	
	GREEN_OFF
	//GREEN_ON
	//GREEN_OFF
//GREEN_ON
	//GREEN_OFF
	
	RED_OFF
	//RED_ON;
	//RED_OFF;
	//RED_ON;
	//RED_OFF;
	
	AMBER_OFF
	//AMBER_ON;
	//AMBER_OFF;
	//AMBER_ON;
	//AMBER_OFF;
	
	 
 
	
	DUMMY_LOAD_OFF
	//DUMMY_LOAD_ON;
	//DUMMY_LOAD_OFF;
	//DUMMY_LOAD_ON;
	//DUMMY_LOAD_OFF;
	
    //CLUSTER_OFF
	//CLUSTER_ON;
	//CLUSTER_OFF;
	
	
	CHARGING_ON
	//CHARGING_OFF
	//CHARGING_ON
	//CHARGING_OFF
	HIGH_LUX_OFF
	//HIGH_LUX_ON
	//HIGH_LUX_OFF
	//HIGH_LUX_ON
	//HIGH_LUX_OFF
	

   
	ob=init_STR_MPOBJ(ob);
 
 
  
    
	Init_Timer0();
	USART_Init ( MYUBRR );
	i2cInit();
	 
	//Update_Date_n_time_dec2bcd_to_rtc(5,	27,1,18,	7,24 ,0);// Current Date
	//Update_Date_n_time_dec2bcd_to_rtc(5,	1,1,17,		6 ,59,30);// 1st Charging Start Test
	//Update_Date_n_time_dec2bcd_to_rtc(5,	1,1,17,		11,59,45);// 1st Charging End  and Discharging Start Test
	//Update_Date_n_time_dec2bcd_to_rtc(5,	1,1,17,		23,59,55);// 1st Discharging End Test Date
	
	//Update_Date_n_time_dec2bcd_to_rtc(5,	16,1,17,	6 ,59,45);// 16th Charging Start Test
	//Update_Date_n_time_dec2bcd_to_rtc(5,	16,1,17,	11,59,45);// 16th Charging End  and Discharging Start Test
	//Update_Date_n_time_dec2bcd_to_rtc(5,	16,1,17,	23,59,55);// 16th Discharging End Test Date
 
	
	return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
STR_MPOBJ * init_STR_MPOBJ(STR_MPOBJ *ob)
{
		ob->flag.above_14_4_volt_f		=0;
		ob->flag.bw_13_5___14_4_volt_f	=0;
		ob->flag.bw_10_5___13_5_volt_f	=0;
		ob->flag.bw_9___10_5_volt_f		=0;
		ob->flag.below_9_volt_f			=0;
		
		ob->flag.FullLux_f				=0;
		ob->flag.Full_Lux_Two_H_f       =0;
		ob->flag.Full_Lux_Two_H_end_f       =0;
		
		ob->FullLuxTime.start_hour		=0;
		ob->FullLuxTime.start_min		=0;
		ob->flag.ChargEnable_f			=1;
		ob->flag.DischEnable_f			=0;
	
		ob->flag.cluster_f				=0;
		ob->flag.DisCharg_1_16th_f      =0;
		ob->flag.Charg_1_16th_f			=0;
		
		ob->FullLuxTime.start_hour		=0;
		ob->FullLuxTime.start_min		=0;
		ob->FullLuxTime.allowd_hour		=0;
		ob->FullLuxTime.allowd_min		=1;
		ob->FullLuxTime.end_hour		=0;
		ob->FullLuxTime.end_min			=0;
		
		ob->FullLuxTime.start_hour		=0;
		ob->FullLuxTime.start_min		=0;
		ob->FullLuxTime.allowd_hour		=FULL_LUX_H;
		ob->FullLuxTime.allowd_min		=FULL_LUX_M;
		ob->FullLuxTime.end_hour		=0;
		ob->FullLuxTime.end_min			=0;
		ob->batt.sampling_time			=SAMP_TIME;
		//-------------------------------------------------------------------------
		ob->cl_fl_rm_lvb.start_hour		=0;
		ob->cl_fl_rm_lvb.start_min		=0;
		ob->cl_fl_rm_lvb.allowd_hour		=0;
		ob->cl_fl_rm_lvb.allowd_min		=1;
		ob->cl_fl_rm_lvb.end_hour		=0;
		ob->cl_fl_rm_lvb.end_min			=0;
		
		ob->cl_fl_rm_lvb.start_hour		=0;
		ob->cl_fl_rm_lvb.start_min		=0;
 
		ob->cl_fl_rm_lvb.end_hour		=0;
		ob->cl_fl_rm_lvb.end_min			=0;
		ob->batt.sampling_time			=SAMP_TIME;
		ob->flag.lower_cut_on_f			=0;
		
		ob->batt.volt_at_T0				=0;
		ob->batt.volt_at_T1				=0;
		ob->batt.change_in_BV			=0;
		ob->batt.dvdt					=0;
		ob->flag.batt_bad_health_f      =0;
		ob->flag.cyclic20per            =0;
		ob->flag.lvb_soft_stateATnosupply=0;
		ob->reading.allowed_hour			=READING_TIME_H;
		ob->reading.allowed_min				=READING_TIME_M;
		ob->reading.allowed_sec				=READING_TIME_S;
		ob->reading.next_reading_hour		=0;
		ob->reading.next_reading_min		=0;
		ob->reading.next_reading_sec		=0;
		ob->flag.reading_f					=0;
		
		ob->calan.old_saved_day             =0;
		ob->calan.old_saved_dom             =0;
		ob->calan.old_saved_month           =0;
		ob->calan.old_saved_year            =0;
		 

		
		ob->flag.test1day1min_f	=0	;
		ob->flag.testdebug_f	=0			;
		return ob;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
 void debug_obj(STR_MPOBJ *ob,uint8_t * str)
 {
	 
	 //USART_Transmit_str(str);USART_Transmit('\n');// Print the passed string
	 // Simple string                      ;Object print in ASCII formate             ;  for New Line      ;
	// USART_Transmit_str("\t absorption    :");USART_Transmit(ob->flag.absorption  +0x30);USART_Transmit('\n');
	 //USART_Transmit_str("\t boostmode     :");USART_Transmit(ob->flag.boostmode   +0x30);USART_Transmit('\n');
	 //USART_Transmit_str("\t Discharging   :");USART_Transmit(ob->flag.Discharging +0x30);USART_Transmit('\n');
	 //USART_Transmit_str("\t floatmode_on  :");USART_Transmit(ob->flag.floatmode_on+0x30);USART_Transmit('\n');
	 //USART_Transmit_str("\t HeatOn        :");USART_Transmit(ob->flag.HeatOn      +0x30);USART_Transmit('\n');
	 //USART_Transmit_str("\t low_battery   :");USART_Transmit(ob->flag.low_battery +0x30);USART_Transmit('\n');
	 //USART_Transmit_str("\t maxvolt       :");USART_Transmit(ob->flag.maxvolt     +0x30);USART_Transmit('\n');
	 //USART_Transmit_str("\t ReadOn        :");USART_Transmit(ob->flag.ReadOn      +0x30);USART_Transmit('\n');
	// USART_Transmit_str("\t sec           :");USART_Transmit(ob->flag.sec         +0x30);USART_Transmit('\n');
	 
 }
 void debug_num(uint32_t no,uint8_t f,uint8_t digit,uint8_t new_line,int8_t *str)
 {
	 if(str!=NULL)USART_Transmit_str(str);
	 //1234
	 STR_NUMB number;
	 number.d1=no%10;//4
	 no=no/10;//123
	 number.d2=no%10;//3
	 no=no/10;//123
	 number.d3=no%10;//2
	 no=no/10;//123
	 number.d4=no%10;//1
	 no=no/10;//123
	 if(digit==4){USART_Transmit(number.d4+0x30);digit--;}
	 if(digit==3){USART_Transmit(number.d3+0x30);if(f==1)USART_Transmit('.');digit--;}
	 if(digit==2){USART_Transmit(number.d2+0x30);digit--;}
	 if(digit==1){USART_Transmit(number.d1+0x30);}
	 if(new_line==1)USART_Transmit('\n');
 }
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
void USART_Init( unsigned int ubrr )
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS)|(3<<UCSZ0);
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
void USART_Transmit_str( unsigned char * str )
{
	while(*str!='\0')
	{
			/* Wait for empty transmit buffer */
			while ( !( UCSR0A & (1<<UDRE)) );
			/* Put data into buffer, sends the data */
			UDR0 = *str;
			str++;
	}

}
/*===============================================================
// Function Name:
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC)) );
	/* Get and return received data from buffer */
	return UDR0;
}

/*==================== Measure Battery Voltage ==================
// Why Req      :
// Description  :
// Note         :
//===============================================================*/
 
void MeasureBatteryVoltage()
 {
	Battery_ADC2();
}

//********************* Print signature of Product ************
void debug_device_signature(void)
{
	USART_Transmit( '\n'		);
	USART_Transmit_str("======RAIL AUTOCOM TECHNOLOGIES (P) LTD.=======");
	USART_Transmit( '\n'		);
	USART_Transmit_str("==SYSTEM Firmware Info ==>"		);USART_Transmit( '\n'		);
	USART_Transmit_str(SV		);USART_Transmit_str("  ");USART_Transmit( '\n'		);
	USART_Transmit( '\n'		);
 
	USART_Transmit_str("-------------------------------------------");
	USART_Transmit( '\n'		);
}
/******************************************************************************
                          End Of File
******************************************************************************/

