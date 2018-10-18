//// Created by Group STABBY

//// Basic heirarchy and functions for STABBY. 
//// Note screen size is 160x120 = 19200
//// Total pixel area is 19200
//// Drive can't be less than 0.30, otherwise not enough power to move motors
////////////////CREATE APPROACHREDNEAR( ADD STAB, TURN RIGHT, STAB, TURN LEFT, STAB), APPROACHREDFAR 

//****** Function Declarations ******//
int CheckFrontBumpValueConditions();
void EscapeFrontBump();

int CheckBlobs();												//Check the blob areas (distance away)
void AvoidBlueBlob();											//Avoid the Blue Blob
void ApproachRedBlob1();										//Approach a far red object
void ApproachRedBlob2();										//Approach a near red object
void ApproachRedBlob3();										//Drive up and stab the red object

void EscapeFrontBump();
void Cruise(); 													//Base movement pattern
void Drive(float left, float right, float delaySeconds);		//Motor control function
void show_cam_image(); 											//Will create camera image for screen display

//Global Constants//
const int BLUE_AREA_THRESHOLD = 19200/16;						//Blue Threshold
const int RED_AREA_THRESHOLD1 = 19200/14;						//Smallest Red Threshold, approach far object
const int RED_AREA_THRESHOLD2 = 19200/4;						//Medium Red Threshold, approach near object
const int RED_AREA_THRESHOLD3 = 19200/2;						//Largest Red Threshold, object near stabbing distance

const int PIXEL_CENTER_X = 80;
const int FRONT_BUMP = 0;										//Channel Number for the bump sensor
const int LEFT_MOTOR = 0;										//Channel Number for the left motor
const int RIGHT_MOTOR = 3; 										//Channel Number for the right motor
const int TOP_SPEED = 100;										//Range of motor speed is -100 to 100. Use a decimal between 1 and -1 in Drive()
//Boolean Constants//
const int TRUE = 1;
const int FALSE = 0;


//****** Function Definitions ******//
int main()
{
	int i;
	camera_open(LOW_RES);										//turn on the camera
	graphics_open(160,120);										//create a graphic box on the screeen display
	display_clear();											//remove messages from camera_open()

	while(1)
	{	camera_update();										//take a snapshot for analysis
		show_cam_image();										//turn snapshot into image
		graphics_update();										//display image on the screen
		
		if(CheckFrontBumpValueConditions() == TRUE)				//////if the bumper hits a wall
		{
			EscapeFrontBump();									//////escape the wall
		}
		else if(CheckBlobs() == 5)								//if a large blue object is near
		{
			AvoidBlueBlob();									//avoid the blue object
		}
		else if(CheckBlobs() == 8)								//////if red object is within stabbing distance
		{
			ApproachRedBlob3();									//////STAB IT
		}
		else if(CheckBlobs() == 7)								//if a red object is near
		{
			ApproachRedBlob2();									//slowly approach the red object
		}
		else if(CheckBlobs() == 6)								//////if a red object is far away
		{
			ApproachRedBlob1();									//////drive towards the red object
		}
		else
		{
			Cruise();											//otherwise continue driving
		}
	}
	
	camera_close();
	graphics_close();
	return 0;
}
	
/******************************************************/

int CheckFrontBumpValueConditions()
{
	int bumpThreshold = 250;
	int bumpMax = 400;
	int frontBumpValue = analog10(FRONT_BUMP);
	
	if(frontBumpValue < bumpThreshold)
	{
		return TRUE;
	}
	else if(frontBumpValue >= bumpThreshold && frontBumpValue <= bumpMax)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/******************************************************/

void EscapeFrontBump()
{
	int bumpThreshold = 250;
	int bumpMax = 400;
	int frontBumpValue = analog10(FRONT_BUMP);
	
	if(frontBumpValue < bumpThreshold)
	{
		Drive(-0.5, -0.5, 0.40);
		Drive(-0.5, 0.50, 0.70);
		
	}
	else if(frontBumpValue >= bumpThreshold && frontBumpValue <= bumpMax)
	{
		Drive(-0.5, -0.5, 0.40);
		Drive(0.5, -0.5, 0.70);
	}
}

/******************************************************/

int CheckBlobs()
{
	int bluearea;
	int redarea;
	camera_update();
	bluearea = get_object_area(1,0);									//get the area of the blue object
	redarea = get_object_area(0,0);										//get the area of the red object
	
	if (bluearea >= BLUE_AREA_THRESHOLD && bluearea > redarea)			// if the blue object is closer than the red, go to AvoidBlueBlob function
	{ 
		return 5;
	}
	else if (redarea >= RED_AREA_THRESHOLD3 && redarea > bluearea)		// if the red object is within stabbing distance, go to ApproachRedBlob3 function
	{ 
		return 8;
	}
	else if (redarea >= RED_AREA_THRESHOLD2 && redarea > bluearea)		// if the red object is near, go to ApproachRedBlob2 function
	{ 
		return 7;
	}	
	else if (redarea >= RED_AREA_THRESHOLD1 && redarea > bluearea)		// if the red object is far, go to ApproachRedBlob1 function
	{ 
		return 6;
	}
	else
	{
		return FALSE;													// otherwise move on to Cruise function
	}
}

/******************************************************/

void AvoidBlueBlob()
{
	int bluecenterx;
	camera_update();									//take a snapshot for analysis
	bluecenterx = get_object_center(1,0).x;				//find the center of the blue object
	
	if (bluecenterx < PIXEL_CENTER_X)					// if the blue object is to the right, turn left wheel harder to turn right
	{ 
		Drive(0.60, 0.40, 0.40);				
	}
	else if(bluecenterx > PIXEL_CENTER_X)				// if the blue object is to the left, turn right wheel harder to turn left
	{
		Drive(0.40, 0.60, 0.40);				
	}
}

/******************************************************/

void ApproachRedBlob1() 							//red object is far away
{
	int redarea;	
	int redcenterx;
	
	Drive(0.0, 0.0, 1.0);							//stop the vehicle for one seconds
	camera_update();								//take a snapshot for analysis
	show_cam_image();								
	graphics_update();
	redarea = get_object_area(0,0);					//find the red object area
	redcenterx = get_object_center(0,0).x;			//find the center of the red object
		
	if(redcenterx > PIXEL_CENTER_X)					//red object is to the right
	{
		Drive(0.45, 0.30, 0.50);
	}
	else if(redcenterx < PIXEL_CENTER_X)			//red object is to the left
	{
		Drive(0.30, 0.45, 0.50);	
	}
	else
	{
		Drive(0.40, 0.40, 0.50);
	}
}
	
/******************************************************/

void ApproachRedBlob2() 							//red object is near
{
	int redarea;	
	int redcenterx;
	
	Drive(0.0, 0.0, 1.0);							//stop the vehicle for one seconds
	camera_update();								//take a snapshot for analysis
	show_cam_image();								
	graphics_update();
	redarea = get_object_area(0,0);					//find the red object area
	redcenterx = get_object_center(0,0).x;			//find the center of the red object
		
	if(redcenterx > PIXEL_CENTER_X)					//red object is to the right
	{
		Drive(0.40, 0.30, 0.50);
	}
	else if(redcenterx < PIXEL_CENTER_X)			//red object is to the left
	{
		Drive(0.30, 0.40, 0.50);	
	}
	else
	{
		Drive(0.30, 0.30, 0.50);
	}
}

/******************************************************/

void ApproachRedBlob3() 							//red object is within stabbing distance
{
	int redarea;	
	int redcenterx;
	
	Drive(0.0, 0.0, 1.0);							//stop the vehicle for one seconds
	camera_update();								//take a snapshot for analysis
	show_cam_image();								
	graphics_update();
	redarea = get_object_area(0,0);					//find the red object area
	redcenterx = get_object_center(0,0).x;			//find the center of the red object
	
	Drive(0.350, 0.350, 0.90);						//drive forward slightly
	Drive(0.00, 0.00, 0.40);						//stop in place
	set_servo_position(2,1024);						//STAB
	enable_servos();
	sleep(2);
	set_servo_position(2, 2047);
	sleep(2);
	set_servo_position(2, 1024);
	sleep(2);
	printf("Fuck Yeah!");
	disable_servos();
	
	Drive(0.40, -0.40, 0.60);						//turn in place
	Drive(0.00, 0.00, 0.40);						//stop in place
	set_servo_position(2,1024);						//STAB
	enable_servos();
	sleep(2);
	set_servo_position(2, 2047);
	sleep(2);
	set_servo_position(2, 1024);
	sleep(2);
	disable_servos();
	
	Drive(-0.40, 0.40, 1.20);						//turn in place
	Drive(0.00, 0.00, 0.40);						//stop in place
	set_servo_position(2,1024);						//STAB
	enable_servos();
	sleep(2);
	set_servo_position(2, 2047);
	sleep(2);
	set_servo_position(2, 1024);
	sleep(2);
	disable_servos();	
}

/******************************************************/

void Cruise()
{
	Drive(0.50, 0.400, 0.50);
}
/******************************************************/

void Drive(float left, float right, float delaySeconds)
{
	delaySeconds *= 1000;
	motor(LEFT_MOTOR, TOP_SPEED * left);
	motor(RIGHT_MOTOR, TOP_SPEED * right);
	msleep(delaySeconds);
}

/******************************************************/

void show_cam_image ()
{
	const unsigned char *img=get_camera_frame();	//create variable that will contain image data
	int row, col, rgb[3]={2,1,0}; 					//converts raw data:BGR to usable data:RGB
		for(row=0;row<120;row++)
		{
			for(col=0;col<160; col++)
			{
				graphics_pixel(col,row,				//update pixels in graphic panel opened in main
				img[3*(160*row+col)+rgb[0]],
				img[3*(160*row+col)+rgb[1]],
				img[3*(160*row+col)+rgb[2]]);
			}
		} 
}
