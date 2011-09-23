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
static float pointTotal; //DECL::VAR::pointTotal
static int state; //DECL::VAR::state

void ZRUser01(float *myState, float *otherState, float time)
{
//BEGIN::PROC::ZRUser
#define PI  3.141593
#define TAU (2*PI)
 
#define posTarget         (&procvar[0])
#define center            (&procvar[3])
#define current_radius    (&procvar[6])
#define radius            (procvar[9])
#define angleStep         (procvar[10])
#define angleTarget       (procvar[11])

  int i;
  float speed;
  float angularVelocity;
  float desiredAngularVelocity;
  float points;
  float velTarget[3];
  float distanceToCircle[3];
  float circleStartPoint[3];

  if (time < 1) {
   DEBUG(("time, angleTarget\n"));
  }

  radius = 0.25;
  angleStep = TAU / 16;

  center[0] = 0;
  center[1] = 0.6;
  center[2] = 0;

  circleStartPoint[0] = 0;
  circleStartPoint[1] = 0.35;
  circleStartPoint[2] = 0;

  DEBUG(("target point: %f, %f, %f\n", circleStartPoint[0],circleStartPoint[1],circleStartPoint[2]));
  angleTarget = -TAU / 4;        // and record the initial angular position
  
  
  if (0 == state) {
    ZRSetPositionTarget(circleStartPoint);
    mathVecSubtract(distanceToCircle, myState, circleStartPoint, 3);
    if (mathVecMagnitude(distanceToCircle, 3) < 0.02) {
      state = 1;
      DEBUG(("Got to the circle!\n"));
    }
  }


  if (1 == state) {
    desiredAngularVelocity = 4.0 * PI / 180;

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

    DEBUG(("%f, %f\n",
	   time,
	   angleTarget));
//END::PROC::ZRUser
}
void ZRInit01()
{
//BEGIN::PROC::ZRInit
memset(procvar,0,sizeof(float)*12);
pointTotal = 0.0f;
state = 0;
//END::PROC::ZRInit
}
//User-defined procedures
