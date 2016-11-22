#pragma once
#include "kilolib.h"

class mykilobot : public kilobot
{
	unsigned char distance;
	message_t out_message;
	int rxed=0;
	float theta;
	float thetaDelta;
	float theta_distance;

	float vtaxis;
	float vrand;
	float vrepulMag;
	float vrepulSum;
	float vmotion;
	
	int motion=0;
	long int motion_timer=0;

	int state = 0;
	bool transmissionComplete = 0;

	int msrx=0;
	struct mydata {
		unsigned int data1;
		unsigned int data2;

	};

	//main loop
	void loop()
	{	
		if(state == 0) { //listening to neighbors
			printf("state: %d\n", state);
			set_motors(0, 0);
			if (transmissionComplete) {
				state = 1;
				// printf("state 0 angle to light: %f\n", angle_to_light);
				// printf("state 0 mod angle to light: %f\n", fabs(fmod(angle_to_light, 2*PI)));
				
				//calculate theta
				vrand = calculateRand();
				vtaxis = calculateTaxis(fabs(fmod(angle_to_light, 2*PI)));
				if (vrepulSum > 6.4) { //temper with max magnitude = 6.4
					vrepulSum = 6.4;
				}

				//calculate x,y of unit vector
				float xposRand = cos(vrand)*0.2; //adding weight of .2
				float yposRand = sin(vrand)*0.2;
				float xposTaxis = cos(vtaxis);
				float yposTaxis = sin(vtaxis);
				float xposRepul = cos(vrepulSum);
				float yposRepul = sin(vrepulSum);

				//add rand + taxis
				float xposSum = xposRand + xposTaxis;
				float yposSum = yposRand + yposTaxis;

				//add (rand+taxis) + repul
				xposSum = xposSum + xposRepul;
				yposSum = yposSum + yposRepul;
				
				float thetaSum = atan2(yposSum, xposSum);

				// if (thetaSum <0 || thetaSum > 2*PI) {
				// 	thetaSum = fmod(thetaSum, 2*PI);
				// }

				thetaDelta = fabs(fmod(angle_to_light, 2*PI)) - thetaSum;
				printf("theta sum: %f\n", thetaSum);
				printf("theta delta: %f\n", thetaDelta);
				theta_distance = 100; //convert vector length to # ticks to move using some scalar 
			}
		}

		else if (state == 1) { //rotate
			printf("state: %d, angle: %f\n", state, fabs(fmod(angle_to_light, 2*PI)));

			if(fabs(fmod(angle_to_light, 2*PI) - thetaDelta)<.3) { //if within threshold, switch states and move forward
				state = 2;

			}

			else if(fabs(fmod(angle_to_light, 2*PI)) - thetaDelta <0) {
				spinup_motors();
				set_motors(kilo_turn_left, 0);
			}

			else {
				spinup_motors();
				set_motors(0, kilo_turn_right);
			}
		}

		else if (state == 2) { //move forward
			printf("state: %d\n", state);
			spinup_motors();
			set_motors(50, 50);
			theta_distance--;

			if (theta_distance <=0) {
				//reset
				transmissionComplete = 0;
				state = 0;
				vrepulSum = 0;
			}
			
		}
	
	// 	//default code
	// 	if(id==1)
	// 	{
	// 		if(fabs(theta)<.3)
	// 		{
	// 			spinup_motors();
	// 			set_motors(50,50);
	// 		}
	// 		else if(theta<0)
	// 		{
	// 			spinup_motors();
	// 			set_motors(kilo_turn_left,0);
	// 		}
	// 		else
	// 		{
	// 			spinup_motors();
	// 			set_motors(0,kilo_turn_right);	
	// 		}
	// 	}
	// 	else
	// 	{
	// 		if(fabs(angle_to_light)<.3)
	// 		{
	// 			spinup_motors();
	// 			set_motors(50,50);
	// 		}
	// 		else if(angle_to_light<0)
	// 		{
	// 			spinup_motors();
	// 			set_motors(kilo_turn_left,0);
	// 		}
	// 		else
	// 		{
	// 			spinup_motors();
	// 			set_motors(0,kilo_turn_right);
	// 		}
	// 	}	
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
		transmissionComplete = 1;
		
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
		updateColor();

		//reverse direction and magnitude
		theta = fabs(fmod(theta+PI, 2*PI));
		distance = 1;

		vrepulMag = calculateRepul(theta, (float)distance/10); //converting mm to cm, calculating maginitude of repulsion vector

		//sum vector to total repulsion vector
		float xposA = vrepulMag*cos(theta);
		float yposA = vrepulMag*sin(theta);
		float xposB = cos(vrepulSum);
		float yposB = sin(vrepulSum);

		//add repulsion to repulsion sum
		float xposSum = xposA + xposB;
		float yposSum = yposA + yposB;
		
		vrepulSum = atan2(yposSum, xposSum);

		printf("vrepulMag: %f, vrepulSum: %f\n", vrepulMag, vrepulSum);
		
	}

	float calculateTaxis(float theta) {
		//vector is estimate of angular position to the light source (aka point of attraction)

		vtaxis = theta;
		return vtaxis;
	}

	float calculateRand() {
		//vector is random direction between (0, 2pi]
		float a = 2*PI;		
		float vrand = ((float)rand()/(float)(RAND_MAX)) * a;
		return vrand;
	}

	float calculateRepul(float theta, float distance) {
		//don't look at all neighbors, look at the ones close enough

		//vector is a calculated repulsion force from a nearby neighbor

		//assume virtual radius of all other robots = my radius
		//estimate angular position to & distance from nearby robots
		//magnitude(repulsion between robot i and j) = 0 when distance >= 2*R, and k(2*R - dist) otherwise
		//sum magnitudes of all repulsion vectors to get vrepulMag
	
		float t = theta;
		float d = distance;

		float k = 0.8;
		float virtualRadius = radius;

		if (id == 1) {
			virtualRadius = radius;		
		}

		else if (id == 2) {
			virtualRadius = 2*radius;		
		}

		else if (id == 3) {
			virtualRadius = 3*radius;		
		}

		if (distance >= virtualRadius*2) {
			vrepulMag =  0.0;
			printf("distance too large \n");
			printf("Dist: %f ", distance);
			printf(" r: %f \n", virtualRadius);
		}

		else {
			vrepulMag = k*(2*virtualRadius - distance);	
		}
		return vrepulMag;
	}

	void updateColor() {
		if (id == 1) {
			set_color(RGB(1, 0, 0));
		}
		else if (id == 2) {
			set_color(RGB(0, 1, 0));
		}
		else if (id == 3) {
			set_color(RGB(0, 0, 1));
		}
	}
};

