//Copyright (C) 2015-2016 by Dragons Team.
//This file is licensed under Dragons License.

#include "Power.h"

Power::Power(void):	 Shape(0,0,0),
					 Environment(),
					 _ishovering(false),
					 _isforward(false),
					 massRateOfAirflow(0),				
					 trueAirSpeed(0),						
					 inducedDrag(0),
					 inducedVelocity1(0),
					 inducedVelocity2(0),
					 dragOfTheBody(0),
					 equivalentFlatPlate(0),
					 idealMechanichalPower(0),
					 profilePowerRatio(0),						
					 minimumPowerSpeed(0),					
					 absoluteMinimumPower(0),					
					 liftDragRatio(0),							
					 ultimateLiftDragRatio(0),				
					 maximumRangeSpeed(0),					
					 powerAtMaximumRangeSpeed(0),				
					 dynamicPressure(0),
					 liftCoefficient(0),
					 dragCoefficient(0),
					 reynoldsNumber(0),
					 reWing(0),
					 reBody(0),
					 db(0),									
					 liftForce(0),
					 angelOfAttack(0),
					 profilePower(0),
					 parasitePower(0),
					 inducePower0(0),
					 inducePower(0),
					 mechanicalTheta(0),
					 xmech(0),
					 ymech(0),
					 mechanicalPower(0),
					 xRes(0),
					 yRes(0),
					 wingbeatfrq(0)
{
	if( this->mass!=0 )
		this->discArea = pow(double(this->wingSpan)/double(2),2)*PI;
	else
		this->discArea = 0;

	if( this->wingSpan!=0 )
		this->bodyFrontalArea = 0.00813*powf(this->mass,0.666);
	else
		this->bodyFrontalArea = 0;

	cout<<"\n <Power default constructor> \n"<<endl;
}

double Power::_Clamp(double fValue, double fMinValue, double fMaxValue){

	if(fValue < fMinValue)
		return fMinValue;

	if(fValue > fMaxValue)
		return fMaxValue;

	return fValue;
}
 
double Power::DegToRad(double fAngDeg){
	
	const double fDegToRad = 3.14159f * 2.0f / 360.0f;
	return fAngDeg * fDegToRad;
}

double Power::RadToDeg(double fAngDeg){
	
	fAngDeg = fAngDeg *  360.0f  ;
	fAngDeg = fAngDeg / 6.28318f ;
	return fAngDeg;
}


void Power::UpdatePower(){

	if( this->mass!=0)
		this->discArea = pow (double (this->wingSpan) / double(2) , 2 ) * PI;
	else
		this->discArea = 0;
		
	if( this->wingSpan!=0)
		this->bodyFrontalArea = (double)0.00813 * powf(this->mass,0.666);
	else
		this->bodyFrontalArea = 0;


	if(this->discArea !=0){
		
				this->massRateOfAirflow = this->discArea * sqrt( (this->mass*this->gravity) / (2*this->discArea*this->airDensity) ) * this->airDensity;

	}
	else{
	     		this->massRateOfAirflow = 0;
				this->trueAirSpeed = 0;
				this->inducedDrag = 0;
				this->inducePower = 0;
	}

	if(this->bodyFrontalArea !=0){
		
		this->minimumPowerSpeed = (0.807*powf(K,0.25)*powf(this->mass,0.5)*powf(this->gravity,0.5)) / (powf(this->airDensity,0.5)*powf(this->wingSpan,0.5)*powf(this->bodyFrontalArea,0.25)*powf(CDb,0.25));
		
		this->absoluteMinimumPower = ((double)1.05*powf(K,0.75)*powf(this->mass,1.5)*powf(this->gravity,1.5)*powf(this->bodyFrontalArea,0.25)*powf(CDb,0.25)) / (double)(powf(this->airDensity,0.5)*powf(this->wingSpan,1.5)) ;
	}
	else{
				this->minimumPowerSpeed = 0;
				this->absoluteMinimumPower = 0;
	}
			
	this->minimumPowerSpeed = (0.807*powf(K,0.25)*powf(this->mass,0.5)*powf(this->gravity,0.5)) / (powf(this->airDensity,0.5)*powf(this->wingSpan,0.5)*powf(this->bodyFrontalArea,0.25)*powf(CDb,0.25));


	if(_isforward &&  !_ishovering){
		trueAirSpeed=minimumPowerSpeed;
		inducedVelocity1=-2*((this->mass*this->gravity)/(2*this->minimumPowerSpeed*this->discArea*this->airDensity));
		inducedVelocity2=0;
	}
	
	if(_ishovering && !_isforward){
		inducedVelocity2= 2*(double)sqrtf(double((this->mass*this->gravity)) / double((2*(this->discArea)*this->airDensity)) );
		trueAirSpeed=0;
		inducedVelocity1=0;
		angelOfAttack=0;

	}

	if(_ishovering && _isforward){
		trueAirSpeed=minimumPowerSpeed;
		inducedVelocity2= 2*(double)sqrtf(double((this->mass*this->gravity)) / double((2*(this->discArea)*this->airDensity)) );
		inducedVelocity1=0;
	}	
		
	if(yRes<=2.0){
		trueAirSpeed=trueAirSpeed/4;
		inducedVelocity1=inducedVelocity1/4;
		inducedVelocity2=inducedVelocity2/4;
	}
		
	if(yRes<=4.0){
		trueAirSpeed=trueAirSpeed/2;
		inducedVelocity1=inducedVelocity1/2;
		inducedVelocity2=inducedVelocity2/2;
	}

	totalSpeed=sqrt(pow(trueAirSpeed,2)+pow(inducedVelocity1,2)+pow(inducedVelocity2,2));

	this->inducedDrag = (2 * K * pow(this->mass * this->gravity,2)) / (pow(this->totalSpeed,2) * PI * pow(this->wingSpan,2) * this->airDensity) ;
		
	this->inducePower =  double(2*K*( pow(this->mass*this->gravity,2))) / double(this->totalSpeed*PI*pow(this->wingSpan,2)*this->airDensity);	
		
	this->dragOfTheBody =  (this->airDensity * pow(this->totalSpeed,2) * this->bodyFrontalArea * CDb)  /  double(2) ;
	
	this->equivalentFlatPlate = this->bodyFrontalArea * CDb; 
	
	this->maximumRangeSpeed = ( (powf(K,0.25)*powf(this->mass,0.5)*powf(this->gravity,0.5)) / ( powf(this->airDensity,0.5) * powf(this->equivalentFlatPlate,0.25) * powf(this->discArea,0.25) ) );
	
	this->idealMechanichalPower= (2*K*( pow(this->mass*this->gravity,2))) / (this->totalSpeed*PI*pow(this->wingSpan,2)*this->airDensity) +
									(this->airDensity*pow(this->totalSpeed,3)*this->bodyFrontalArea*CDb) / (2) ;
	
	this->profilePowerRatio = Cpro / this->aspectRatio;
	
	this->ultimateLiftDragRatio = sqrt( this->discArea / this->equivalentFlatPlate);
	
	this->dynamicPressure = this->airDensity*pow(this->totalSpeed,2) / 2;
	
	this->inducePower0 = sqrtf( ( 2*pow((this->mass*this->gravity),3) ) /  (PI*pow(this->wingSpan,2)*this->airDensity)) * K;
	
	this->parasitePower = (this->airDensity*pow(this->totalSpeed,3)*this->bodyFrontalArea*CDb) / double(2) ;
	
	this->profilePower  = (this->profilePowerRatio * this->absoluteMinimumPower);
	
	this->powerAtMaximumRangeSpeed = ((powf(K,0.75)*powf(this->mass,1.5)*powf(this->gravity,1.5)*powf(this->equivalentFlatPlate,0.25)) /
										(powf(this->airDensity,0.5)*powf(this->discArea,0.75)));
	
	this->dragCoefficient = (2 * this->dragOfTheBody) / (this->airDensity * pow(this->totalSpeed,2)* wingArea );
	
	this->liftForce = this->gravity * this->mass ;
	
	
	// calculate Reynold Numbers
	if(this->kinematicViscosity != 0){
		this->reynoldsNumber = ( double(this->totalSpeed * this->refernceLength * this->airDensity) / double(this->kinematicViscosity) );
		this->reWing = double(this->totalSpeed*this->meanChord) / double(this->kinematicViscosity) ;
		this->reBody = double(this->totalSpeed * this->db) / double(this->kinematicViscosity);
	}
	else{
		this->reynoldsNumber = 0;
		this->reWing = 0;
		this->reBody = 0;
	}
	if(angelOfAttack!=0){
		trueAirSpeed=cos(DegToRad(angelOfAttack))*totalSpeed;
		inducedVelocity1=sin(DegToRad(angelOfAttack))*totalSpeed;
	}

	// calculate the Mechanical Power using X,Y coordinates
	if(this->angelOfAttack == 0){			
		ymech = inducePower - parasitePower ;
		xmech = -profilePower ;
		double t = sqrt(pow(xmech,2)+pow(ymech,2));
		this->mechanicalTheta =- acos ( -xmech / t) ;
		this->mechanicalTheta = RadToDeg ( this->mechanicalTheta ) ;
						if(ymech > 0)
								this->mechanicalTheta = -this->mechanicalTheta ;
	}
	else{
		angelOfAttack = _Clamp(angelOfAttack , -10.0 , +10.0);
		ymech = inducePower - parasitePower ;
					if(this->angelOfAttack > 0)
						ymech = ymech +  sin(DegToRad(angelOfAttack)) *profilePower ;
					if(this->angelOfAttack < 0)
						ymech = ymech +  -1.0*sin(DegToRad(fabs(angelOfAttack))) *profilePower ;
		xmech = cos(DegToRad(angelOfAttack)) * profilePower ;
		double t = sqrt(pow(xmech,2)+pow(ymech,2));
		this->mechanicalTheta = -acos ( xmech / t) ;
		this->mechanicalTheta = RadToDeg ( this->mechanicalTheta ) ;
						if(ymech > 0)
							this->mechanicalTheta = -this->mechanicalTheta ;
	}
	

	this->mechanicalPower=sqrt(pow(this->xmech,2)+pow(this->ymech,2));

	// calculate the result X,Y powers coordinate on the Bird
	if(yRes>=-0.000005){
			xRes+=trueAirSpeed/10000;
			yRes+=inducedVelocity1/10000+inducedVelocity2/10000;
			height=yRes;
	
	}
	else{
			_isforward=false;
			massRateOfAirflow=0;				
			trueAirSpeed=0;						
			inducedDrag=0;
			inducedVelocity1=0;
			inducedVelocity2=0;
			angelOfAttack=0;
			profilePower=0;
			parasitePower=0;
			inducePower0=0;
			inducePower=0;
			mechanicalPower=0;
			yRes=0;
	}
						

	this->liftDragRatio = ( double(this->mass*this->gravity*this->trueAirSpeed) / double(pow(this->xmech,2)+pow(this->ymech,2)));

	this->wingbeatfrq=pow(this->mass,3/8)*pow(this->gravity,1/2)*pow(this->wingSpan,-23/24)*pow(this->wingArea,-1/3)*pow(this->airDensity,-3/8);
	
	//cout<<"------------------------\nPower Update has finished\n------------------------\n"<<endl;
}

void Power::printPower(){
	printf("\n-------------- Power Info ---------------\n");

	//printf(" disc Area = %.4f \n",this->discArea);
	printf(" mass Rate Of Air flow = %.4f \n",this->massRateOfAirflow);
	printf(" true Air Speed = %.4f \n",this->trueAirSpeed);
	//printf(" body Frontal Area = %.4f \n",this->bodyFrontalArea);
	printf(" induced Drag = %.4f \n",this->inducedDrag);
	printf(" induced Velocity = %.4f \n",this->inducedVelocity1);
	printf(" drag Of The Body = %.4f \n",this->dragOfTheBody);
	//printf(" equivalent Flat Plate = %.4f \n",this->equivalentFlatPlate);
	printf(" ideal Mechanichal Power = %.4f \n",this->idealMechanichalPower);
	//printf(" profile Power Ratio = %.4f \n",this->profilePowerRatio);
	printf(" minimum Power Speed = %.4f \n",this->minimumPowerSpeed);
	//printf(" absolute Minimum Power = %.4f \n",this->absoluteMinimumPower);
	//printf(" lift Drag Ratio = %.30f \n",this->liftDragRatio);
	//printf(" ultimate Lift Drag Ratio = %.4f \n",this->ultimateLiftDragRatio);
	printf(" maximum Range Speed = %.4f \n",this->maximumRangeSpeed);
	printf(" power At MaximumRangeSpeed = %.4f \n",this->powerAtMaximumRangeSpeed);
	//printf(" dynamic Pressure = %.4f \n",this->dynamicPressure);
	//printf(" lift Coefficient = %.4f \n",this->liftCoefficient);
	//printf(" drag Coefficient = %.4f \n",this->dragCoefficient);
	//printf(" reynoldsNumber = %.4f \n",this->reynoldsNumber);
	//printf(" reWing = %.4f \n",this->reWing);
	//printf(" reBody = %.4f \n",this->reBody);
	//printf(" db = %.4f \n",this->db);
	//printf(" lift Force = %.4f \n",this->liftForce);
	printf(" angel Of Attack = %.4f \n",this->angelOfAttack);
	printf(" profile Power = %.4f \n",this->profilePower);
	printf(" parasite Power = %.4f \n",this->parasitePower);
	printf(" induce Power0 = %.4f \n",this->inducePower0);
	printf(" induce Power = %.4f \n",this->inducePower);
	//printf(" mechanical Theta = %.4f \n",this->mechanicalTheta);
	//printf(" xmech = %.4f \n",this->xmech);
	//printf(" ymech = %.4f \n",this->ymech);
	printf(" mechanical Power = %.4f \n",this->mechanicalPower);
	printf(" zRes = %.4f \n",this->xRes);
	printf(" yRes = %.4f \n",this->yRes);
	printf( " wingbeatfrq = %.4f \n",this->wingbeatfrq);
}

Power::~Power(void){
cout<<"\n <Power Destructor> \n" <<endl;
}
