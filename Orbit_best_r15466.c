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

static float TAU; //DECL::VAR::TAU
static float Score; //DECL::VAR::Score
static void Orbit (float center[3], float radius, float myState[3], float time); //DECL::PROC::Orbit

void ZRUser01(float *myState, float *otherState, float time)
{
//BEGIN::PROC::ZRUser

float center[3];
float radius;

center[0] = 0;
center[1] = 0.6;
center[2] = 0;

radius = .25;

Orbit(center, radius, myState, time


//END::PROC::ZRUser
}
void ZRInit01()
{
//BEGIN::PROC::ZRInit
TAU = 6.28318;
Score = 0;
//END::PROC::ZRInit
}
//User-defined procedures
static void Orbit (float center[3], float radius, float myState[3], float time)
{
//BEGIN::PROC::Orbit

float desiredAngularVelocity;
float velTarget[3];
float angleTarget;
float posTarget[3];
float currentAngle;
float angleStep;
float pointsEarned;
float currentAngularVelocity;

//angular velocity = vsinx / r
    
    
pointsEarned = PgetScore() - Score;

currentAngle = atan2(myState[1] - center[1], myState[0] - center[0]);

currentAngularVelocity = (sqrt(pow(myState[3], 2) + pow(myState[4], 2))) * sin(currentAngle) / radius;

DEBUG(("Time:%f, Points gotten: %f, Current Angular Velocity: %f\n", time, pointsEarned, currentAngularVelocity));

  //tweak angleStep as neccesary
angleStep = TAU * radius / 2;

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

Score = PgetScore();
ZRSetVelocityTarget(velTarget);

//END::PROC::Orbit
}
