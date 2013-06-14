/***************************************************************************
 
    file                 : SimpleDriver.cpp
    copyright            : (C) 2007 Daniele Loiacono
 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "SimpleDriver.h"


/* Gear Changing Constants*/
const int SimpleDriver::gearUp[6]=
    {
        8000,8000,8000,6500,7000,0
    };
const int SimpleDriver::gearDown[6]=
    {
        0,2500,3000,3000,3500,3500
    };

const float SimpleDriver::wheelRadius[4]={0.3306,0.3306,0.3276,0.3276};
const float SimpleDriver::sin5 = 0.08716;
const float SimpleDriver::cos5 = 0.99619;

float SimpleDriver::filterOpponentsAccel(float accel){
    if(isFrontBlocked&&current.getSpeedX()>minSpeed){
        return -0.2;
    } else
        return accel;
}

float SimpleDriver::filterOpponentsSteer(float steer){
    if(!isFrontBlocked){
        return steer;
    } else if(!isFrontLeftBlocked && curTrackPos<0.6 && steer>-0.2){
        return steer + 0.3*steerLock;
    } else if(!isFrontRightBlocked && curTrackPos>-0.6 && steer<0.2){
        return steer - 0.3*steerLock;
    } else if(insideTrack) {
        if(curTrackPos>0)
            return - 0.3*steerLock;
        else
            return 0.3*steerLock;
    } else
        return steer;
}


int
SimpleDriver::getGear()
{

    int gear = current.getGear();
    int rpm  = current.getRpm();

    // if gear is 0 (N) or -1 (R) just return 1 
    if (gear<1)
        return 1;
    // check if the RPM value of car is greater than the one suggested 
    // to shift up the gear from the current one     
    if (gear <6 && rpm >= gearUp[gear-1])
        return gear + 1;
    else
    	// check if the RPM value of car is lower than the one suggested 
    	// to shift down the gear from the current one
        if (gear > 1 && rpm <= gearDown[gear-1])
            return gear - 1;
        else // otherwhise keep current gear
            return gear;
}


float 
SimpleDriver::getSteer()  
{      
	// steering angle is compute by correcting the actual car angle w.r.t. to track 
	// axis [current.getAngle()] and to adjust car position w.r.t to middle of track [curTrackPos*0.5]
    float targetAngle;
    //float rxSensor=maxLeft;   
    //float sxSensor=maxRight;
    float targetPos=0;
    float prop;

    if(!insideTrack){
        targetAngle=(steerTolerance*(current.getAngle()-curTrackPos)); 
    } else
    if(curSeg==straight){

        if(lastSeg==right){
            targetPos=straightTargetOffset;
        } else if(lastSeg==left){
            targetPos=-straightTargetOffset;
        }

        if(lookahead==right){
            targetPos=(targetPos-straightTargetOffset)/2;
        } else if(lookahead==left){
            targetPos=(targetPos+straightTargetOffset)/2;
        }

        targetAngle=(steerTolerance*(current.getAngle()-(curTrackPos-targetPos)));
    }
    else{
        if(curSeg==left){
            //targetAngle=(1-(sxSensor/200));
            targetPos=targetPos+curveTargetOffset;
        } else{
            //argetAngle=-(1-(rxSensor/200));
            targetPos=-curveTargetOffset;
        }
        targetAngle=(steerTolerance*(current.getAngle()-(curTrackPos-targetPos)));
        //prop=fabs((curTrackPos-targetPos)*current.getAngle()/__PI);
        //targetAngle=(1-prop)*targetAngle + prop*steerTolerance*(current.getAngle()-(curTrackPos-targetPos));                 
    }
    
    // at high speed reduce the steering command to avoid loosing the control

    if (current.getSpeedX() > steerSensitivityOffset)
        return targetAngle/(steerLock*(current.getSpeedX()-steerSensitivityOffset)*wheelSensitivityCoeff);
    else
        return (targetAngle)/steerLock;

}
float
SimpleDriver::getAccel()
{
    // checks if car is out of track
    if (insideTrack)
    {

        float targetSpeed;
        float cSensor=current.getTrack(9);
        float bSensor=current.getTrack((leftCurve?8:10));
        //float rxSensor=maxLeft;
        //float sxSensor=maxRight;

        // track is straight and enough far from a turn so goes to max speed
        if (straightSegment){
            targetSpeed = maxSpeed;
            updateSeg(straight);
        }            
        else
        { 
            float sen, coss, aux1, aux2;
            int dif = fabs(maxAngle-9);
           /* sen=sin5;  
            coss=cos5;

            while(dif>1){
                aux1=sen;
                aux2=coss;
                if(dif%2==0){                    
                    sen=2*aux1*aux2;
                    dif/=2;
                } else{
                    sen=aux1*cos5 + aux2*sin5;
                    coss=aux2*cos5-aux1*sin5;
                    dif--;
                }
            }*/

            float h = current.getTrack(9)*sin5;
            float b = bSensor - cSensor*cos5;
            float sinAngle = b*b/(h*h+b*b);
            // estimate the target speed depending on turn and on how close it is
            targetSpeed = maxSpeed*(cSensor*sinAngle/maxSpeedDist);
            // approaching a turn on right
            if(rightCurve)
            {
                updateSeg(right);
            }
            // approaching a turn on left
            else
            {
                updateSeg(left);
            }

        }
        if(targetSpeed<minSpeed)
            targetSpeed=minSpeed;
        // accel/brake command is expontially scaled w.r.t. the difference between target speed and current one
        return 2/(1+exp(accelSensitivity*(current.getSpeedX() - targetSpeed))) - 1;
    }
    else
        return outsideAccel; // when out of track returns a moderate acceleration command

}

CarControl
SimpleDriver::wDrive(CarState cs)
{
    int maxlen=0;
    outside=false;
    for(int i=0;i<19;i++){
        if(current.getTrack(i)>maxlen){
            maxlen=current.getTrack(i);
            maxAngle=i;
        }
        if(current.getTrack(i)<0){
            outside=true;
        }
    }

	// after car is stuck for a while apply recovering policy
    if (isStuck())  {
    	/* set gear and sterring command assuming car is 
    	 * pointing in a direction out of track */
    	
    	// to bring car parallel to track axis
        float steer = - current.getAngle() / steerLock; 
        int gear=-1; // gear R
        
        // if car is pointing in the correct direction revert gear and steer  
        if (current.getAngle()*curTrackPos>0)
        {
            gear = 1;
            steer = -steer;
        }

        // Calculate clutching
        clutching(clutch);

        // build a CarControl variable and return it
        CarControl cc (stuckAccel,0.0,gear,steer,clutch);
        return cc;
    }

    else {// car is not stuck
    	// compute accel/brake command
        float accel_and_brake = filterOpponentsAccel(getAccel());
        // compute gear 
        int gear = getGear();
        // compute steering
        float steer = filterOpponentsSteer(getSteer());
        

        // normalize steering
        if (steer < -1)
            steer = -1;
        if (steer > 1)
            steer = 1;
        
        // set accel and brake from the joint accel/brake command 
        float accel,brake;
        if (accel_and_brake>0)
        {
            accel = filterTCL(accel_and_brake);
            brake = 0;
        }
        else
        {
            accel = 0;
            // apply ABS to brake
            brake = filterABS(-accel_and_brake);
        }

        // Calculate clutching
        clutching(clutch);

        // build a CarControl variable and return it
        /*std::cout.precision(4);

        std::cout<<segString(curSeg)<<" "<<segString(lookahead);
        std::cout<<maxAngle<<" "<<accel<<" "<<brake<<" "<<steer ;
        std::cout<<" ( ";
        for(int i=6;i<14;i++){
            std::cout << current.getTrack(i) << " ";
        }
        std::cout<<")"<<std::endl;*/
        
        CarControl cc(accel,brake,gear,steer,clutch);
        return cc;
    }
}

/* check if the car is stuck */
bool SimpleDriver::isStuck()
{
    // check if car is currently stuck
    if ( fabs(current.getAngle()) > stuckAngle && (current.getSpeedX() < stuckSpeed || fabs(current.getAngle()) > 3*stuckAngle))
    {
        // update stuck counter
        stuck++;
    }
    else
    {
        // if not stuck reset stuck counter
        stuck = 0;
    }
    if( stuck > stuckTime && (fabs(current.getAngle()) > 2*stuckAngle || curTrackPos*current.getAngle() < 0.0)){
        return true;
    } else
        return false;
}



float
SimpleDriver::filterABS(float brake)
{
	// convert speed to m/s
	float speed = current.getSpeedX() / 3.6;
	// when spedd lower than min speed for abs do nothing
    if (speed < absMinSpeed)
        return brake;
    
    // compute the speed of wheels in m/s
    float slip = 0.0f;
    for (int i = 0; i < 4; i++)
    {
        slip += current.getWheelSpinVel(i) * wheelRadius[i];
    }
    // slip is the difference between actual speed of car and average speed of wheels
    slip = speed - slip/4.0f;
    // when slip too high applu ABS
    if (slip > absSlip)
    {
        brake = brake - (slip - absSlip)/absRange;
    }
    
    // check brake is not negative, otherwise set it to zero
    if (brake<0)
    	return 0;
    else
    	return brake;
}

float  SimpleDriver::filterTCL(float accel_brake)
{
  float r=accel_brake;
  double s = ((3.6*current.getWheelSpinVel(0) +
               3.6*current.getWheelSpinVel(1)) * wheelRadius[1] +
              (3.6*current.getWheelSpinVel(2) +
              3.6*current.getWheelSpinVel(3)) * wheelRadius[3]) / 4.0;
                  
  float slipspeed;
  if (current.getSpeedX()>=0) slipspeed = s-current.getSpeedX();
  else slipspeed = s+current.getSpeedX();
  
  float TCL_SLIP = 1.5;
  float TCL_RANGE = 5.0;
  if (slipspeed > TCL_SLIP && accel_brake>0) {
    r = accel_brake - min(accel_brake, (slipspeed - TCL_SLIP)/TCL_RANGE);
  }
    
  return r;
  
} 


void
SimpleDriver::onShutdown()
{
    cout << "Bye bye!" << endl;
}

void
SimpleDriver::onRestart()
{
    cout << "Restarting the race!" << endl;
}

void
SimpleDriver::clutching(float &clutch)
{
  double maxClutch = clutchMax;

  // Check if the current situation is the race start
  if (current.getCurLapTime()<clutchDeltaTime  && stage==RACE && current.getDistRaced()<clutchDeltaRaced)
    clutch = maxClutch;

  // Adjust the current value of the clutch
  if(clutch > 0) 
  {
    double delta = clutchDelta;
    if (current.getGear() < 2)
	{
      // Apply a stronger clutch output when the gear is one and the race is just started
	  delta /= 2;
      maxClutch *= clutchMaxModifier;
      if (current.getCurLapTime() < clutchMaxTime)
        clutch = maxClutch;
	}

    // check clutch is not bigger than maximum values
	clutch = min(maxClutch,double(clutch));

	// if clutch is not at max value decrease it quite quickly
	if (clutch!=maxClutch)
	{
	  clutch -= delta;
	  clutch = max(0.0,double(clutch));
	}
	// if clutch is at max value decrease it very slowly
	else
		clutch -= clutchDec;
  }
}

void
SimpleDriver::init(float *angles)
{

	// set angles as {-90,-75,-60,-45,-30,20,15,10,5,0,5,10,15,20,30,45,60,75,90}

	for (int i=0; i<5; i++)
	{
		angles[i]=-90+i*15;
		angles[18-i]=90-i*15;
	}

	for (int i=5; i<9; i++)
	{
			angles[i]=-20+(i-5)*5;
			angles[18-i]=20-(i-5)*5;
	}
	angles[9]=0;
}


void SimpleDriver::updateSeg(segment s){
    if(s!=curSeg){
        lastSeg=curSeg;
        curSeg=s;        
    }if(straightLookahead){
        lookahead=straight;
    } else if(leftCurveLookahead){
        lookahead=left;
    } else if(rightCurveLookahead){
        lookahead=right;
    } else{
        lookahead=straight;
    }
}

float SimpleDriver::getMax(float a,float b){
    if(a>=b)
        return a;
    else
        return b;
}

float SimpleDriver::getMax3(float a, float b, float c){
    if(a>=b && a>=c)
        return a;
    else if(b>=c)
        return b;
    else
        return c;
}

float SimpleDriver::getMin(float a, float b){
    if(a<=b)
        return a;
    else 
        return b;
}