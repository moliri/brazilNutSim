#pragma once
#include "kilolib.h"

class mykilobot : public kilobot
{
	unsigned char distance;
	message_t out_message;
	int rxed=0;
	float theta;
	
	int motion=0;
	long int motion_timer=0;

	int msrx=0;
	struct mydata {
		unsigned int data1;
		unsigned int data2;

	};

	//main loop
	void loop()
	{	
	
		if(id==0)
		{
			
			if(fabs(theta)<.3)
			{
				
				spinup_motors();
				set_motors(50,50);

			}
			else if(theta<0)
			{
				spinup_motors();
				set_motors(kilo_turn_left,0);
			
			

			}
			else
			{
				spinup_motors();
				set_motors(0,kilo_turn_right);
	
			
			}
		}
		else
		{
			
			
			if(fabs(angle_to_light)<.3)
			{
				
				spinup_motors();
				set_motors(50,50);

			}
			else if(angle_to_light<0)
			{
				spinup_motors();
				set_motors(kilo_turn_left,0);
			
			

			}
			else
			{
				spinup_motors();
				set_motors(0,kilo_turn_right);
	
			
			}

		}


		
		
	}

	//executed once at start
	void setup()
	{
		
		out_message.type = NORMAL;
		out_message.crc = message_crc(&out_message);
		set_color(RGB(0,1,0)); //starting color doesn't matter
	}

	//executed on successfull message send
	void message_tx_success()
	{
		//set_color(RGB(1,0,0));
		
	}

	//sends message at fixed rate
	message_t *message_tx()
	{
		static int count = rand();
		count--;
		if (!(count % 10))
		{
			return &out_message;
		}
		return NULL;
	}

	//receives message
	void message_rx(message_t *message, distance_measurement_t *distance_measurement,float t)
	{
		distance = estimate_distance(distance_measurement);
		theta=t;
		
	}
};

