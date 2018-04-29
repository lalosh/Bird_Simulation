//Copyright (C) 2015-2016 by Dragons Team.
//This file is licensed under my License.

#include "Shape.h"
#include <iostream>
#include <cmath>

using namespace std;

Shape::	Shape(double mass,double wingSpan,double wingArea) :				 aspectRatio(0),
																			 cropMass(0),		
																			 emptyMass(0),					
																			 fatMass(0),			
																			 musclesMass(0),				
																			 frameMass(0),	
																			 fatFraction(0),	
																			 frameFraction(0),	
																			 musclesFraction(0),	
																			 refernceLength(0)
{
		this->mass = mass;
		this->wingSpan = wingSpan;
		this->wingArea = wingArea;
		this->meanChord = (double)this->wingArea / (double)this->wingSpan;
}

void Shape::UpdateShape(){
	
	this->meanChord = (double)this->wingArea / (double)this->wingSpan;

	if( this->wingArea != 0)
		this->aspectRatio = double(  pow(this->wingSpan,2)   / double(this->wingArea)  );
	else
		cout<<" Division by Zero!! couldn't calculate Aspect Ratio!! \n \t( because (wing area or wing span ) = 0 )!! "<<endl;

	if( mass!= 0 ){
		this->fatFraction = double(fatMass) / double(mass);
		this->frameFraction = double(frameMass) / double(mass);
		this->musclesFraction = double(musclesMass) / double(mass);
	}
	else
		cout<<" Division by Zero!! couldn't calculate Fractions \n \t( because mass=0 )!! "<<endl;

	//cout<<"------------------------\nShape Update has finished\n------------------------\n"<<endl;
}

void Shape::printShape(){
	printf("\n------------- Shape Info ----------------\n");

	printf(" mass = %.4f \n",this->mass);
	printf(" wingArea = %.4f \n",this->wingArea);
	printf(" wingSpan = %.4f \n",this->wingSpan);
	printf(" meanChord = %.4f \n",this->meanChord);
	printf(" aspectRatio = %.4f \n",this->aspectRatio);
	printf(" refernceLength = %.4f \n",this->refernceLength);
	printf(" cropMass = %.4f \n",this->cropMass);
	printf(" emptyMass = %.4f \n",this->emptyMass);
	printf(" fatMass = %.4f \n",this->fatMass);
	printf(" musclesMass = %.4f \n",this->musclesMass);
	printf(" frameMass = %.4f \n",this->frameMass);
	printf(" fatFraction = %.4f \n",this->fatFraction);
	printf(" musclesFraction = %.4f \n",this->musclesFraction);
	printf(" frameFraction = %.4f \n",this->frameFraction);
}

Shape::~Shape(void){
	cout<<"\n <Shape Destructor> \n" <<endl;
}
