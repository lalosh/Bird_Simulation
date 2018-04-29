//Copyright (C) 2015-2016 by Dragons Team.
//This file is licensed under my License.

#pragma once
#include "Shape.h"
#include "Environment.h"
#include <iostream>
#include <cmath>
#include <freeglut.h>

using namespace std;

// 4 Constants
const double PI   =  3.141592653589793238462643383279502884; 
const double K    = 1.15000000000;				 // induced power factor.
const double CDb  = 0.10000000000;				 // number that refers to how streamlined the bird body is.
const double Cpro = 8.40000000000;		     	 // profile power constant.

class Power : public Shape,public Environment
{
public:

	// 37 Variables
	double 	 discArea,							// sd disc area.
			 massRateOfAirflow,					// rm mass rate of air flow.
			 trueAirSpeed,						// vt true air speed.
			 bodyFrontalArea,					// sb body frontal area.
			 inducedDrag,						// induced Drag because of the wings.
			 inducedVelocity1,					// the induced velocity of the bird in case forwording.
			 inducedVelocity2,					// the induced velocity of the bird in case hovering.
			 dragOfTheBody,						// Drag because of the body's weight.
			 equivalentFlatPlate,				// Equivalent flat-plate area of the body.
			 idealMechanichalPower,				// it actually equals to the mechanical power minus the profile power.
			 profilePowerRatio,					// x1 profile power Ratio.
			 minimumPowerSpeed,					// vmp minimum power speed.
			 absoluteMinimumPower,				// pam absolute  minimum power at Vmp.
			 liftDragRatio,						// Nmech  lift:drag ratio of the mechenical power.
			 ultimateLiftDragRatio,				// Nultimate ultimate lift:frag ratio of the mechenical power.
			 maximumRangeSpeed,					// vmr  maximum range speed.
			 powerAtMaximumRangeSpeed,			// pmr power at Vmr.
			 dynamicPressure,					// The dynamic pressure is the basis of the aerodynamic forces acting on the wings and body.
			 liftCoefficient,					// Lift Coefficient.
			 dragCoefficient,					// the drag coefficient measures the extent to which the aerofoil slows down the air flowing past it.
			 reynoldsNumber,					// Reynold Numbers is calculated to refers how much the bird is stramlined and it has many application later.
			 reWing,							// Reynolds number based on wing mean chord.
			 reBody,							// Reynolds number based on body diameter at widest part.
			 db,								// diameter of the circle with the same area as the body frontal area.
			 liftForce,							// the lift force that the bird muscles has produce.
			 angelOfAttack,						// The angle of attack is the angle between the relative airflow and the chord line.
			 profilePower,						// the drag power that the wings produce.
			 parasitePower,						// the drag power that the body produce.
			 inducePower0,						// the induced power that wing's muscles produce at taking off.
			 inducePower,						// the induced power that wing's muscles produce at gliding.
			 mechanicalTheta,					// the output theta after calculating the mechanical power.
			 xmech,								// X coordinate for the mechanical power.
			 ymech,								// Y coordinate for the mechanical power.
			 mechanicalPower,					// Mechanical power = {inducePower + profilePower + parasitePower}.
			 xRes,								// X coordinate for the Result powers.
			 yRes,								// Y coordinate for the Result powers.
			 wingbeatfrq,
			 totalSpeed;

	bool     _isforward,
			 _ishovering;

	// 1 Constructors
	Power(void);

	// update function reCalculate the values after the user has edit them
	void UpdatePower();

	double DegToRad(double fAngDeg);

	double RadToDeg(double fAngDeg);

	double _Clamp(double fValue, double fMinValue, double fMaxValue);

	void printPower();

	// 1 Destructor
	~Power(void);
};

