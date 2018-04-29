//Copyright (C) 2015-2016 by Dragons Team.
//This file is licensed under my License.

#pragma once
#include <iostream>
#include <cmath>

using namespace std;

// 3 Constants
const double pressureOnSeaSurface=1013.2;		//  pressure On Sea Surface     =  1013.2
const double airDensityOnSeaSurface=1.226;		//  air Density On Sea Surface  =  1.226
const double accelerationDueToGravity[10][11] =
{
	{ 9.780, 9.777, 9.774, 9.771, 9.768, 9.765, 9.762, 9.759, 9.756, 9.753, 9.749 },
	{ 9.782, 9.779, 9.776, 9.773, 9.770, 9.766, 9.763, 9.760, 9.757, 9.754, 9.751 },
	{ 9.786, 9.783, 9.780, 9.777, 9.774, 9.771, 9.768, 9.765, 9.762, 9.759, 9.755 },
	{ 9.793, 9.790, 9.787, 9.784, 9.781, 9.778, 9.775, 9.772, 9.769, 9.765, 9.762 },
	{ 9.802, 9.799, 9.795, 9.792, 9.789, 9.786, 9.783, 9.780, 9.777, 9.774, 9.771 },
	{ 9.811, 9.808, 9.804, 9.801, 9.798, 9.795, 9.792, 9.789, 9.786, 9.783, 9.780 },
	{ 9.819, 9.816, 9.813, 9.810, 9.807, 9.804, 9.801, 9.798, 9.794, 9.791, 9.788 },
	{ 9.826, 9.823, 9.820, 9.817, 9.814, 9.811, 9.808, 9.804, 9.801, 9.798, 9.795 },
	{ 9.831, 9.827, 9.824, 9.821, 9.818, 9.815, 9.812, 9.809, 9.806, 9.803, 9.800 },
	{ 9.832, 9.829, 9.826, 9.823, 9.820, 9.817, 9.814, 9.811, 9.807, 9.804, 9.801 }
};

class Environment
{
public:

															//		Units				//
	// 7 Variables										    //--------------------------//	
	double height,										   	//	    meter				//
		   temperature,									    //		C					//
		   pressure,										//		millibar			//
		   airDensity,										//		Kg m^-3				//
		   kinematicViscosity,								//		m^2 S^-1 * 10^-5	//
		   gravity,											//		m S^-2				//
		   latitude;										//		Degree				//


	// 1 Constructors
	Environment(void);

	// height must be between (0-->10000) & latitude must be between (0--90)
	void setGravity(double latitude,double height);
	
	float Clamp(float fValue, float fMinValue, float fMaxValue);

	// update function reCalculate the values after the user has edit them
	void UpdateEnvironment();

	void printEnvironment();

	// 1 Destructor
	~Environment(void);
};

