void setup_positions()
{
	light_center[0]=1200;
	light_center[1]=1200;
	
	//Testing setup for 3 robots
	robots[0] = new mykilobot();
	robots[1] = new mykilobot();
	robots[2] = new mykilobot();
	robots[0]->robot_init(400, 400, 0);
	robots[1]->robot_init(450, 450, 0);
	robots[2]->robot_init(425, 425, 0);
	robots[0]->id=1;
	robots[1]->id=2;
	robots[2]->id=3;



	//Setup for 3 shell types below
	// int k = 0;
	// int columns = (int)sqrt((num_robots * arena_width / arena_height));
	// int rows = (int)(num_robots / columns);
	// if (num_robots % columns) rows++;
	// int horizontal_separation =  arena_width / (columns + 1); //radius = 16
	// int vertical_separation = (int)arena_height / (rows + 1);
	// for (int i = 0;i < num_robots;i++)
	// {
	// 	int c = i % columns + 1; //columns
	// 	int r = i / columns + 1; //rows
	// 	int hr = rand() % (horizontal_separation / 2) + horizontal_separation / 4;
	// 	int x = c * horizontal_separation;// + hr;
	// 	int vr = rand() % (vertical_separation / 2) + vertical_separation / 4;
	// 	int y = r * vertical_separation;// + vr;
	// 	robots[k] = new mykilobot();
	// 	double t = rand() * 2 * PI / RAND_MAX;
	// 	robots[k]->robot_init(x, y, t);
	// 	k++;
	// }
	
	//setup for 100 (100)
	// for(int i = 0; i < 100; i++) {
	// 	robots[i]->id=1;
	// }

	//setup for 100 (50/50)
	// for(int i = 0; i < 100; i++) {

	// 	if (i % 2 == 0) {
	// 		robots[i]->id=1;
	// 	}
	// 	else {
	// 		robots[i]->id=2;
	// 	}	
	// }

	//setup for 210 (70/70/70)
	// for(int i = 0; i < 210; i++) {

	// 	if (i % 3 == 0) {
	// 		robots[i]->id=1;
	// 		robots[i+1]->id=2;
	// 		robots[i+2]->id=3;
	// 	}	
	// }

}