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

static unsigned char state; //DECL::VAR::state
static unsigned char SphereNumber; //DECL::VAR::SphereNumber
static char getShield; //DECL::VAR::getShield
static char asteroid; //DECL::VAR::asteroid
static void shoot (float myState[12], float target[3], unsigned int fire); //DECL::PROC::shoot
static void orbit (float myState[12], float center[3], unsigned char CCW); //DECL::PROC::orbit

void ZRUser01(float *myState, float *otherState, float time)
{
//BEGIN::PROC::ZRUser
float indigens[3] = {0.0, 0.6, 0.0};
float opulens[3] = {0.0, -0.6, 0.0};
float laser1[3] = {0.4, 0.0, 0.0};
float laser2[3] = {-0.4, 0.0, 0.0};
float shield[3] = {0.0, 0.4, 0.0};
float zero[3] = {0.0, 0.0, 0.0};
float difference[3];
float *Asteroid;


switch((int)time)
{
    case 0:
        SphereNumber = !!(myState[0] < 0) + 1;
        break;
    case 30:
        getShield = (otherState[1] <= 0);
        break;
    case 61:
        asteroid = (PinAsteroid(otherState) == 1) ? 1 : 0;
        break;
}

Asteroid = asteroid ? opulens : indigens;


switch(state)
{
    case 0:
        ZRSetPositionTarget(((SphereNumber == 1) - (PotherHasLaser() == SphereNumber)) ? laser1 : laser2);
        shoot(myState, opulens, 0);
        if(PhaveLaser() || PgetPhase() == 2)
            state = 1;
        break;
        
    case 1:
        if(!getShield)
            shield[1] = .3;
        ZRSetPositionTarget(shield);
        shoot(myState, opulens, 0);
        if(PotherHasShield() || PhaveShield() || PgetPhase() == 2)
            state = 2;
        break;
        
    case 2:
        if(PiceHits() < 14 && !asteroid)
            shoot(myState, opulens, (PgetPhase() > 1));
        if(!(PiceMelted()) && asteroid)
        {
            mathVecSubtract(difference, myState, opulens, 3);
            if(mathVecMagnitude(difference, 3) > .8)
                ZRSetPositionTarget(opulens);
            else
                ZRSetVelocityTarget(zero);
            shoot(myState, opulens, (PgetPhase() > 1));
            return;
        }
        orbit(myState, Asteroid, 1);
        break;
        
}
//END::PROC::ZRUser
}
void ZRInit01()
{
//BEGIN::PROC::ZRInit
state = 0;
SphereNumber = 0;
getShield = 1;
asteroid = 0;
//END::PROC::ZRInit
}
//User-defined procedures
static void shoot (float myState[12], float target[3], unsigned int fire)
{
//BEGIN::PROC::shoot
float direction[3];

mathVecSubtract(direction, target, myState, 3);
mathVecNormalize(direction, 3);

ZRSetAttitudeTarget(direction);

if (fire && (acos(mathVecInner(&myState[6], direction, 3) / mathVecMagnitude(&myState[6], 3)) < (0.1)))
    Plaser();
//END::PROC::shoot
}
static void orbit (float myState[12], float center[3], unsigned char CCW)
{
//BEGIN::PROC::orbit
float difference[3], target[3];
float theta;
float thetastep = .66;

mathVecSubtract(difference, myState, center, 3);
theta = atan2(difference[1], difference[0]);

if(CCW)
    thetastep *= -1;

theta += thetastep;
difference[0] = .4 * cos(theta);
difference[1] = .4 * sin(theta);
mathVecAdd(target, center, difference, 3);
ZRSetPositionTarget(target);
//END::PROC::orbit
}
