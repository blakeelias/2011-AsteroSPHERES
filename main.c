B1;2600;0c#include <math.h>
#include <string.h>
#include "ZR_API.h"
#include "ZRGame.h"
#include "math_matrix.h"
#include "spheres_constants.h"

#ifdef ZRSIMULATION
extern void _init(void);
void *_start = &_init;
#endif

static float TAU; //DECL::VAR::TAU
static float Points; //DECL::VAR::Points
static void leaveOrbit (float myState[12], float asteroid[3], float mPanel[3]); //DECL::PROC::leaveOrbit
static void spin (float myState[12]); //DECL::PROC::spin
static void OrbitDirectionally (float center[3], float radius, float myState[3], int counterclockwise); //DECL::PROC::OrbitDirectionally
static char readyToLeaveOrbit (float myState[12], float asteroid[3], float mBase[3]); //DECL::PROC::readyToLeaveOrbit
static char TangentFinder (float myState[12], float center[3], float radius, float tangentPoint[3]); //DECL::PROC::TangentFinder
static void doStrategy (char asteroidIsIndigens, char actionIsSpin, char stationIs1, float time, float myState[12]); //DECL::PROC::doStrategy

void ZRUser01(float *myState, float *otherState, float time)
	{
	//BEGIN::PROC::ZRUser

	#define VLen(a) mathVecMagnitude((a), 3) //finds magnitude, returns float
	#define VAdd(a, b, result) mathVecAdd(result, (a), (b), 3)  //adds vectors a and b and returns it in result
	#define VSub(a, b, result) mathVecSubtract(result, (a), (b), 3) //subtracts vectors a and b and returns it in result
	#define VUnit(a) mathVecNormalize((a), 3) //turns the vector into a unit vector
	#define VMult(a, b, result) Vfunc(4, (a), NULL, (result), (b) //multiplies vector a by scalar b and returns it in result
	#define VDot(a, b) mathVecInner((a), (b), 3) //finds dot product of vectors a and b
	#define VCross(a, b, result) mathVecCross(result, (a), (b)) //finds a x b and returns it in result
	#define VDist(a, b) Vfunc(6, (a), (b), NULL, 0) //finds the distance between points a and b (which is just the magnitude of a - b)
	#define VCopy(a, result) memcpy((result), (a), sizeof(float)*3) //copies array a into array b
	#define VAngle(a, b) Vfunc(8, (a), (b), NULL, 0) //returns the angle between two vectors
	#define VPoint(a, b, result) Vfunc(9, (a), (b), result, 0) //creates a unit vector pointing from point a to point b and returns it in result
	#define VRotate(a, b, result, deg) Vfunc(10, (a), (b), result, deg) //rotates vector a towards vector b by (deg) degrees, result is in result


	doStrategy(0, 0, 0, time, myState);

	//000   opulens, orbit, station2
	//001   opulens, orbit, station1    
	//010   opulens, spin, station2
	//011   opulens, spin, station1
	//100   indigens, orbit, station2
	//101   indigens, orbit, station1
	//110   indigens, spin, station2
	//111   indigens, spin, station1
	//END::PROC::ZRUser
	}
void ZRInit01()
	{
		//BEGIN::PROC::ZRInit
		TAU = 6.283185307179586;
		Points = 0;
		//END::PROC::ZRInit
	}
	//User-defined procedures
static void leaveOrbit (float myState[12], float asteroid[3], float mPanel[3])
	{
		//BEGIN::PROC::leaveOrbit
		#define Angle(v1, v2) acos(mathVecInner((v1), (v2), 3)/(mathVecMagnitude((v1), 3) * mathVecMagnitude((v2), 3)))
		float targetVel[3];
		float r, t;
		float desiredMag;
		float vecToAst[3];
		int i;

		for(i = 0; i < 3; i++)
		vecToAst[i] = asteroid[i] - myState[i];


		r = sqrt(mathVecInner(vecToAst, vecToAst, 3));

		if(r > .6)
		ZRSetPositionTarget(mPanel);
		else
			{
				for(i = 0; i < 3; i++)
				targetVel[i] = mPanel[i] - myState[i];
				desiredMag = r*3.141592/(45*sin(Angle(vecToAst, targetVel)));
				for(i = 0; i < 3; i++)
				targetVel[i] *= sqrt(desiredMag);    
				ZRSetVelocityTarget(targetVel);
			}
		//END::PROC::leaveOrbit
	}
static void spin (float myState[12])
	{
		//BEGIN::PROC::spin
		float dirs[3];
		dirs[2] = 1.872459067 + atan2(myState[7], myState[6]);
		dirs[0] = cos(dirs[2]);
		dirs[1] = sin(dirs[2]);
		dirs[2] = 0;

		ZRSetAttitudeTarget(dirs);
		//END::PROC::spin
	}
static void OrbitDirectionally (float center[3], float radius, float myState[3], int counterclockwise)
	{
		//BEGIN::PROC::OrbitDirectionally
		float desiredAngularVelocity;
		float velTarget[3];
		float angleTarget;
		float posTarget[3];
		float currentAngle;
		float angleStep;

		currentAngle = atan2(myState[1] - center[1], myState[0] - center[0]);

		//tweak angleStep as neccesary
		angleStep = TAU * radius / 2;

		//If counterclockwise is 1, don't change anything, because then this
		//method functions exactly as Blake's original one does. Otherwise,
		//it should be 0, so we multiply angleStep by -1.
		angleStep *= ((2 * counterclockwise) - 1);

		desiredAngularVelocity = TAU / 90;

		angleTarget = currentAngle + angleStep;

		posTarget[0] = radius * cos(angleTarget) + center[0];
		posTarget[1] = radius * sin(angleTarget) + center[1];

		velTarget[0] = posTarget[0] - myState[0];
		velTarget[1] = posTarget[1] - myState[1];
		velTarget[2] = posTarget[2] - myState[2];
		mathVecNormalize(velTarget, 3);
		velTarget[0] *= radius * desiredAngularVelocity;
		velTarget[1] *= radius * desiredAngularVelocity;
		velTarget[2] *= radius * desiredAngularVelocity;

		ZRSetVelocityTarget(velTarget);
		//END::PROC::OrbitDirectionally
	}
static char readyToLeaveOrbit (float myState[12], float asteroid[3], float mBase[3])
	{
		//BEGIN::PROC::readyToLeaveOrbit
		#define Angle(v1, v2) acos(mathVecInner((v1), (v2), 3)/(mathVecMagnitude((v1), 3) * mathVecMagnitude((v2), 3)))

		float myPos[3];
		float myVel[3];
		float toMBase[3];
		float r;
		float hypotenuse;
		int i;

		for(i = 0; i < 3; i++)
			myPos[i] = myState[i];
			
		for(; i < 6; i++)
			myVel[i - 3] = myState[i];
			
		r = sqrt(mathSquare(asteroid[0] - myPos[0])
			   + mathSquare(asteroid[1] - myPos[1])
			   + mathSquare(asteroid[2] - myPos[2]));

		hypotenuse = sqrt(mathSquare(asteroid[0] - mBase[0])
						+ mathSquare(asteroid[1] - mBase[1])
						+ mathSquare(asteroid[2] - mBase[2]));
			
		mathVecSubtract(toMBase, mBase, myPos, 3);

		//If (going in right direction) and (current distance to the mining base == length of tangent line to m base)
		if((mathVecInner(myVel, toMBase, 3) > 0) &&
		   (fabsf(mathVecMagnitude(toMBase, 3) - sqrt(hypotenuse*hypotenuse - r*r)) < 0.01))
			return 'y';
		else
			return 'n';
		//END::PROC::readyToLeaveOrbit
	}
static char TangentFinder (float myState[12], float center[3], float radius, float tangentPoint[3])
	{
		//BEGIN::PROC::TangentFinder
		float dist, theta, tandist, xangle;


		dist = sqrt(mathSquare(myState[0] - center[0]) +
						  mathSquare(myState[1] - center[1]) +
						  mathSquare(myState[2] - center[2]));


		theta = asin(radius/dist);
		tandist = dist * sin(theta);
		xangle = atan2(center[1] - myState[1], center[0] - myState[0]);


		tangentPoint[0] = myState[0] + (tandist * cos(theta + xangle));
		tangentPoint[1] = myState[1] + (tandist * sin(theta + xangle));
		tangentPoint[2] = 0;
		//END::PROC::TangentFinder
	}
static void doStrategy (char asteroidIsIndigens, char actionIsSpin, char stationIs1, float time, float myState[12])
	{
		//BEGIN::PROC::doStrategy
		float indigens[3] = {0.0, 0.6, 0.0};
		float opulens[3] = {0.0, -0.6, 0.0};
		float station1[3] = {0.6, 0.0, 0.0};
		float station2[3] = {-0.6, 0.0, 0.0};
		float laser1[3] = {0.4, 0, 0}; 
		float laser2[3] = {-0.4, 0, 0}; 
		float target[3] = {0.0,0.0,0.0};
		float distvec[3] = {0.0, 0.0, 0.0};
		float dist;
		float PointsEarned;

		PointsEarned = PgetScore() - Points;

		DEBUG(("Time: %f, Points: %f, Fuel Used: %f\n", time, PointsEarned, (100-PgetPercentFuelRemaining())));


		if(time >= 140)
			{
				if(stationIs1)
					{
						ZRSetPositionTarget(station1);
					}
				else
					{
						ZRSetPositionTarget(station2);
					}
				return;
			}

		if(asteroidIsIndigens)
			{
				if(actionIsSpin)
					{
						if(time >= 48)
							{
								spin(myState);
							}
						ZRSetPositionTarget(indigens);
					}
				else
					{
						mathVecSubtract(distvec, indigens, myState, 3);
						dist = mathVecMagnitude(distvec, 3);
						if(dist > 0.60)
							{
								TangentFinder(myState, indigens, 0.40, target);
								ZRSetPositionTarget(target);
							}
						else
							{
								OrbitDirectionally(indigens, 0.40, myState, 0);
							}
					}
				if(readyToLeaveOrbit(myState, indigens, station1) == 'y')
					{
						leaveOrbit(myState, indigens, station1);
					}
			}
		else//opulens
			{
				if (!PhaveLaser())
					{
						float difference1[3];
						float difference2[3];
						float dist1, dist2;
						  
						mathVecSubtract(difference1, myState, laser1, 3);
						mathVecSubtract(difference2, myState, laser2, 3);
						dist1 = mathVecMagnitude(difference1);
						dist2 = mathVecMagnitude(difference2);
						if (dist1 < dist2 && PotherHasLaser() != 1)
							{
								ZRSetPositionTarget(laser1);
							}
						else if (PotherHasLaser() != 2)
							{
								ZRSetPositionTarget(laser2);
							}
					}
				else if (!PiceMelted())
					{
						float att_target[3];
						ZRSetPositionTarget(opulens);
						VPoint(myState, opulens, att_target);
						ZRSetAttitudeTarget(att_target);
						if (VAngle(att_target, &myState[6]) <= 12)
							{
								Plaser();
							}
					}

				if(actionIsSpin)
					{
						if(time >= 48)
							{
								spin(myState);
							}
						ZRSetPositionTarget(opulens);
					}
				else//problem is definitely in here
					{
						mathVecSubtract(distvec, opulens, myState, 3);
						dist = mathVecMagnitude(distvec, 3);
						if(dist > 0.60)
							{
								TangentFinder(myState, opulens, 0.40, target);
								ZRSetPositionTarget(target);
							}
						else
							{
								OrbitDirectionally(opulens, 0.40, myState, 0);
							}
					}
					if(readyToLeaveOrbit(myState, opulens, station1) == 'y')
						{
							leaveOrbit(myState, opulens, station1);
						}
			}




			
		//END::PROC::doStrategy
	}
float Vfunc(int which, float *v1, float *v2, float *vresult, float scalar)
	{
	  
	int i;
	
	if (which == 4)
		{ // vresult = scalar * v1
			for (i = 0; i < 3; ++i)
			vresult[i] = scalar * v1[i];
			return 0;
		}
	  
	if (which == 6)
		{ // returns distance between v1 and v2
			float v3[3];
			mathVecSubtract(v3, v1, v2, 3);  // v3 = v1 - v2
			return mathVecMagnitude(v3,3);
		}
	  
	if (which == 8)
		{ // angle between two vectors
			float dot = mathVecInner(v1, v2, 3)/(mathVecMagnitude(v1,3)*mathVecMagnitude(v2,3));
			return acos(dot)*180.0/3.14159265;
		}
	  
	if (which == 9)
		{ // unit vector pointing from v1 toward v2
			float v9[3];
			mathVecSubtract(v9, v2, v1, 3);
			memcpy(vresult, v9, sizeof(float)*3);
			mathVecNormalize(vresult,3);
			return 0;
		}
	  
	if (which == 10)
		{ // rotated vector v1 towards v2 at scalar degrees
			float ang = scalar*3.14159265/180;
			float u[3];
			float c;
			float s;
			float rot[3];
			int k;
			mathVecNormalize(v1,3);
			mathVecNormalize(v2,3);
			
			if (scalar > Vfunc(8,v1,v2,NULL,0)) {
			return 1;
			}
			
			c = cosf(ang);
			s = sinf(ang);
			mathVecCross(u,v1,v2);
			mathVecNormalize(u,3);
			
			for(k=0;k<2;k++)
			{
				if ((k == 1) && (fabs(Vfunc(8,rot,v1,NULL,0)) > fabs(Vfunc(8,v1,v2,NULL,0)) || fabs(Vfunc(8,rot,v2,NULL,0)) > fabs(Vfunc(8,v1,v2,NULL,0))))
					{
						c = cosf(-1*ang);
						s = sinf(-1*ang);
					}
				rot[0] = (v1[0]*(c + u[0] * u[0] * (1-c)) + v1[1]*(u[0] * u[1] * (1-c) - u[2] * s) + v1[2]*(u[0] * u[2] * (1-c) + u[1] * s));
				rot[1] = (v1[0]*(u[0] * u[1] * (1-c) + u[2] * s) + v1[1]*(c + u[1] * u[1] * (1-c)) + v1[2]*(u[1] * u[2] * (1-c) - u[0] * s));
				rot[2] = (v1[0]*(u[0] * u[2] * (1-c) - u[1] * s) + v1[1]*(u[1] * u[2] * (1-c) + u[0] * s) + v1[2]*(c + u[2] * u[2] * (1-c))); 
			}
			
			memcpy(vresult,rot,sizeof(float)*3);
			return 0;
		}
  
}
