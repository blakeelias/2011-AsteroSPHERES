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

static float procvar[13]; //DECL::VAR::procvar

void ZRUser01(float *myState, float *otherState, float time)
{
//BEGIN::PROC::ZRUser
#define PI 3.141593
#define TAU (2*PI)

#define posTarget         (&procvar[0])
#define center            (&procvar[3])
#define current_radius    (&procvar[6])
#define radius            (procvar[9])
#define angleStep         (procvar[10])
#define angleTarget       (procvar[11])
#define score             (procvar[12])
 
int i;
float speed;
float desiredAngularVelocity;
float points;
float velTarget[3];
 
if (time < 1) {
    memset(procvar, 0, 13);
    radius = 0.2;
    angleStep = TAU / 16;
    center[0] = 0;
    center[1] = 0.35;
    center[2] = 0.2 + 0.3;
    DEBUG(("time, current_radius, angleTarget, points\n"));
}
 
desiredAngularVelocity = 4.0 * PI / 180;
 
posTarget[0] = radius * cos(angleTarget) + center[0];
posTarget[1] = radius * sin(angleTarget) + center[1];
posTarget[2] = center[2];
 
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
 
// connect radius from center to current position
for (i = 0; i < 3; i++) {
     current_radius[i] = myState[i] - center[i];
}


 
DEBUG(("%f, %f, %f %f\n",
       time,
       mathVecMagnitude(current_radius, 3),
       angleTarget,
       PgetScore() - score));
       
score = PgetScore();
//END::PROC::ZRUser
}
void ZRInit01()
{
//BEGIN::PROC::ZRInit
memset(procvar,0,sizeof(float)*13);
//END::PROC::ZRInit
}
//User-defined procedures

