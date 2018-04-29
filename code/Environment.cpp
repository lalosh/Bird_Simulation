//Copyright (C) 2015-2016 by Dragons Team.
//This file is licensed under my License.

#include "Environment.h"

Environment::Environment(void):	height(0),
								temperature (15),
								pressure (pressureOnSeaSurface),
								airDensity(1.22623),
								kinematicViscosity(1.466),
								latitude(0),
								gravity(9.780)
{
cout<<"\n <Environment default constructor> \n"<<endl;
}

 float Environment::Clamp(float fValue, float fMinValue, float fMaxValue){

	if(fValue < fMinValue)
		return fMinValue;

	if(fValue > fMaxValue)
		return fMaxValue;

	return fValue;
}

void Environment::setGravity(double latitude,double height){
	
	if((latitude>=0 && latitude<=90) && (height>=0 && height<=10000))
			
		gravity = accelerationDueToGravity[(int) ( ceil(  latitude/10  ))][(int) ( ceil(  height/1000  ))];
	
}

void Environment::UpdateEnvironment(){

	this->height = Clamp(this->height,0,10000);

	if(this->height < 11000 && this->height >= 0)	
		this->temperature = 15 - (0.0065 * this->height);
	else						
		this->temperature = -56.5;


	this->pressure = pressureOnSeaSurface * powf(   (1.0- (0.0000226 * this->height))  ,5.256 ) ;

	this->airDensity = airDensityOnSeaSurface * ( double(this->pressure) / (double)pressureOnSeaSurface) * ( double(288) / double(this->temperature + 273) );
	
	this->kinematicViscosity = ( 1.466 + (0.09507 * this->height/(double)1000) + ( 0.01047 * pow(this->height/(double)1000,2) ))/pow(10,5);

	setGravity(this->latitude,this->height);

	//cout<<"------------------------\nEnvironment Update has finished\n------------------------\n"<<endl;
}

void Environment::printEnvironment(){
	printf("\n------------ Environment Info ------------\n");

	printf(" height = %.4f \n",this->height);
	printf(" temperature = %.4f \n",this->temperature);
	printf(" pressure = %.4f \n",this->pressure);
	printf(" air Density = %.4f \n",this->airDensity);
	printf(" kinematic Viscosity = %.10f \n",this->kinematicViscosity);
	printf(" gravity = %.4f \n",this->gravity);
	//printf(" latitude = %.4f \n",this->latitude);
}

Environment::~Environment(void)
{
cout<<"\n <Environment Destructor> \n" <<endl;
}
