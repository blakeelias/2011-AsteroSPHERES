#include <math.h>
#include <string.h>
#include "ZR_API.h"
#include "ZRGame.h"
#include "math_matrix.h"
#include "spheres_constants.h"

#ifdef ZRSIMULATION
extern void _init(void);
void *_start = &_init;
#endif

static float procvar[12]; //DECL::VAR::procvar
static int state; //DECL::VAR::state

void ZRUser01(float *myState, float *otherState, float time)
{
//BEGIN::PROC::ZRUser
#define TAU (2*PI)
 
#define angleTarget (procvar[0])

  float center[3]           = {0, 0.35, 0};
  float circleStartPoint[3] = {0, 0.6,  0};
  float radius;
  float desiredAngularVelocity;
  float angleStep;

  float posTarget[3];
  float velTarget[3];
  float distanceToCircle[3];


  radius = 0.25;
  angleStep = TAU / 16;
  angleTarget = TAU / 4; // where we start on the circle, because of circleStartPoint
  desiredAngularVelocity = TAU / 90;

  if (time < 1) {
    DEBUG(("time, current_radius, angularVelocity, angleTarget, pointTotal, points rate\n"));
  }
  
  if (1 == state) { 
    posTarget[0] = radius * cos(angleTarget) + center[0];
    posTarget[1] = radius * sin(angleTarget) + center[1];

    if (sqrt(mathSquare(myState[0] - posTarget[0]) +
         mathSquare(myState[1] - posTarget[1]) +
	     mathSquare(myState[2] - posTarget[2])) < 0.05) {
      angleTarget += angleStep;
    }
    
    velTarget[0] = posTarget[0] - myState[0];
    velTarget[1] = posTarget[1] - myState[1];
    velTarget[2] = posTarget[2] - myState[2];
    mathVecNormalize(velTarget, 3);
    velTarget[0] *= radius * desiredAngularVelocity;
    velTarget[1] *= radius * desiredAngularVelocity;
    velTarget[2] *= radius * desiredAngularVelocity;

    ZRSetVelocityTarget(velTarget);
  }
  else {
    ZRSetPositionTarget(circleStartPoint);
    mathVecSubtract(distanceToCircle, myState, circleStartPoint, 3);
    if (mathVecMagnitude(distanceToCircle, 3) < 0.02) {
      state = 1;
      DEBUG(("Got to the circle!"));
    }
  }

  DEBUG(("%f, %f\n",
	 time,
	 angleTarget));
//END::PROC::ZRUser
}
void ZRInit01()
{
//BEGIN::PROC::ZRInit
memset(procvar,0,sizeof(float)*12);
state = 0;
pointTotal = 0.0f;
//END::PROC::ZRInit
}
//User-defined procedures
