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

static void spin (float myState[12]); //DECL::PROC::spin
static void OrbitDirectionally (float center[3], float radius, float myState[3], int counterclockwise); //DECL::PROC::OrbitDirectionally
static void TangentFinder (float myState[12], float center[3], float radius, float tangentPoint[3]); //DECL::PROC::TangentFinder
static void doStrategy (char asteroidIsIndigens, char actionIsSpin, char stationIs1, float time, float myState[12]); //DECL::PROC::doStrategy

void ZRUser01(float *myState, float *otherState, float time)
{
//BEGIN::PROC::ZRUser
doStrategy(1, 0, 1, time, myState);
//END::PROC::ZRUser
}
void ZRInit01()
{
//BEGIN::PROC::ZRInit

//END::PROC::ZRInit
}
//User-defined procedures
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
#define TAU 6.283185307179586

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
static void TangentFinder (float myState[12], float center[3], float radius, float tangentPoint[3])
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
float target[3] = {0.0,0.0,0.0};
float distvec[3] = {0.0, 0.0, 0.0};
float dist;

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
}
else
{
    
}
//END::PROC::doStrategy
}
