//Copyright (C) 2015-2016 by Dragons Team.
//This file is licensed under my License.

#pragma once
#include <iostream>
#include <cmath>

using namespace std;

class Shape
{
public:

	// 14 Variables
	 double	 wingSpan,					//	the distance from one wing tip to the other.
			 wingArea,					//  the area that supports the bird’s weight when it is gliding.
			 meanChord,					//  is the distance from the leading edge of the wing to the trailing edge.
			 mass,						//	All-up mass
			
			 aspectRatio,			    // is the ratio of the wing span to the mean chord.
			 cropMass,					// Mass of crop contents.
			 emptyMass,					// All-up mass with crop empty.
			 fatMass,					// Mass of fat that is consumable as fuel.
			 musclesMass,				// Mass of flight muscles.
		     frameMass,					// Mass of airframe.
			
			 fatFraction,				// Fat fraction.
			 musclesFraction,			// Flight muscle fraction.
			 frameFraction,				// Airframe fraction.
			
			 refernceLength;

	// 1 Constructors
	
	Shape(double mass,double wingSpan,double wingArea);
	
	// update function reCalculate the values after the user has edit them
	void UpdateShape();

	void printShape();
	
	// 1 Destructor
	~Shape(void);
};

