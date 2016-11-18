void setup_positions()
{
	light_center[0]=1200;
	light_center[1]=1200;
	
	int k = 0;
	int columns = 10; // 10x10 i.e. 10 columns, 10 rows
	int rows = (int)(num_robots / columns);
	if (num_robots % columns) rows++;
 	//robots are touching so seperation should be their diameter
	int horizontal_separation = 15*radius;
	int vertical_separation = 2*radius; 
	//place robots in center of screen/arena, aesthetic preference
	int center_x=arena_width/2-columns/2 * horizontal_separation; 
	int center_y=arena_height/2-rows/2 * vertical_separation;

	
	
	/*//setup for 100 (100)
	for(int i = 0; i < 100; i++) {
		robots[i] = new mykilobot();
		robots[i]->robot_init(rand() % 2400, rand() % 2400, 0);
		robots[i]->id=1;	

	} */

	/*//setup for 100 (50/50)
	for(int i = 0; i < 50; i++) {
		robots[i] = new mykilobot();
		robots[i]->robot_init(rand() % 2400, rand() % 2400, 0);
		robots[i]->id=1;	

	} 
	
	for(int i = 50; i < 100; i++) {
		robots[i] = new mykilobot();
		robots[i]->robot_init(rand() % 2400, rand() % 2400, 0);
		robots[i]->id=2;	

	} */

	//setup for 210 (70/70/70)
	for(int i = 0; i < 70; i++) {
		robots[i] = new mykilobot();
		robots[i]->robot_init(rand() % 2400, rand() % 2400, 0);
		robots[i]->id=1;	

	} 
	
	for(int i = 70; i < 140; i++) {
		robots[i] = new mykilobot();
		robots[i]->robot_init(rand() % 2400, rand() % 2400, 0);
		robots[i]->id=2;	

	}

	for(int i = 140; i < 210; i++) {
		robots[i] = new mykilobot();
		robots[i]->robot_init(rand() % 2400, rand() % 2400, 0);
		robots[i]->id=3;	

	}

}
