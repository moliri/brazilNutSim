//calculating segregation error
void Measure_metric() {
	int numerator = 0;
	int denominator = 0;
	for (int i = 0; i < num_robots; i++) {
		for (int j = 0; j < num_robots; j++) {
			double xipos = robots[i]->pos[0];
			double yipos = robots[i]->pos[1];
			double xjpos = robots[j]->pos[0];
			double yjpos = robots[j]->pos[1];
			double xlight = 1200;
			double ylight = 1200;
			int roboti_id = robots[i]->id;
			int robotj_id = robots[j]->id;
	
			double xdistance_ri_to_light = xipos - xlight;
			double ydistance_ri_to_light = yipos - ylight;

			double xdistance_rj_to_light = xjpos - xlight;			
			double ydistance_rj_to_light = yjpos - ylight;
			
			//distance between ri and light
			double distancei = sqrt(xdistance_ri_to_light*xdistance_ri_to_light + ydistance_ri_to_light*ydistance_ri_to_light);
			
			//distance between rj and light
			double distancej = sqrt(xdistance_rj_to_light*xdistance_rj_to_light + ydistance_rj_to_light*ydistance_rj_to_light);

			if ((roboti_id < robotj_id) && (distancei >= distancej)) {
				numerator = numerator + 1;
			}
			else if ((roboti_id > robotj_id) && (distancei <= distancej)) {
				numerator = numerator + 1;
			}
			else {
				numerator = numerator + 0;
			}
			denominator = denominator +1;
		}	
	}
	
	double segregation_error = numerator/denominator;
	printf("segErr: %f", segregation_error);
	//return segregation_error;

}
