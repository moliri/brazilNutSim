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
	float vrepul;
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

				//calculate theta
				vrand = calculateRand();
				vtaxis = fmod(angle_to_light, 2*PI);

				//calculate x,y of unit vector
				float xposRand = 0*cos(vrand);
				float yposRand = 0*sin(vrand);
				float xposTaxis = cos(vtaxis);
				float yposTaxis = sin(vtaxis);
				float xposSum = xposRand + xposTaxis;
				float yposSum = yposRand + yposTaxis;
				
				float thetaSum = atan2(yposSum, xposSum);

				// if (thetaSum <0 || thetaSum > 2*PI) {
				// 	thetaSum = fmod(thetaSum, 2*PI);
				// }

				thetaDelta = fmod(angle_to_light, 2*PI) - thetaSum;
				printf("theta delta: %f\n", thetaDelta);
				theta_distance = 100; //convert vector length to # ticks to move using some scalar 
			}
		}

		else if (state == 1) { //rotate
			printf("state: %d, angle: %f\n", state, fmod(angle_to_light, 2*PI));

			if(fabs(fmod(angle_to_light, 2*PI) - thetaDelta)<.3) { //if within threshold, switch states and move forward
				state = 2;

			}

			else if(fmod(angle_to_light, 2*PI) - thetaDelta <0) {
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
				transmissionComplete = 0;
				state = 0;
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

		
		// float vrand = calculateRand();
		// printf("vrand: %f\n", vrand);
		
		// float vrepul = calculateRepul(theta, (float)distance/10); //converting mm to cm
		// printf("vrepul: %f\n", vrepul);

		// float vmotion = vtaxis + 0.6*vrand + vrepul;
		// printf("vmotion: %f\n", vmotion); 

		// out_message.data[0] = vmotion;
		// out_message.crc=message_crc(&out_message);
		
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

		//angle to light between -pi, pi (here, can check if it's negative, pos) or 0 to 2pi
			//every time I add/subtract values to theta

		//vector is a calculated repulsion force

		//assume virtual radius of all other robots = my radius
		//estimate angular position to & distance from nearby robots
		//magnitude(repulsion between robot i and j) = 0 when distance >= 2*R, and k(2*R - dist) otherwise
		//sum magnitudes of all repulsion vectors to get vrepul

		float vrepul;		
		float t = theta;
		float d = distance;
		float k = 0.2;
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
			vrepul = vrepul + 0.0;
			printf("distance too large \n");
			printf("Dist: %f ", distance);
			printf(" r: %f \n", virtualRadius);
		}

		else {
			vrepul = vrepul+ k*(2*virtualRadius - distance);	
		}

		if (vrepul > 6.4) { //max magnitude = 6.4
			vrepul = 6.4;
			return vrepul;
		}
		else {
			return vrepul;
		}
		
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

