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



void ZRUser01(float *myState, float *otherState, float time)
{
//BEGIN::PROC::ZRUser

//
//SphereTarget[3] can be changed to make a different destination.
/*
 
SPHERE1 spawn at        Distruptor Upgrade      Shield
[0.4, -0.6]         [0.0,0.2]           [0.0,0.4]
[0.4, -0.6, 0.0]        [0.0, 0.0, -0.4]        [0.0,0.0,0.4]
 
Laser 1             Laser 2             Indigens
[0.4,0.0]           [-0.4,0.0]          [0.0,0.6]
[0.4,0.0,0.0]           [-0.4,0.0,0.0]          [0.0, 0.35, 0.2]
 
Opulens             Mining Station 1        Mining Station 2
[0.0,-0.6]          [0.6,0.0]           [-0.6,0.0]
[0.0,-0.35, -0.2]       [0.6,0.0,-0.5]          [-0.6,0.0,0.5]     
 
*/
float laser1_2D[3] = {0.4, 0.0};
float laser2_2D[3] = {-0.4, 0.0};
float shield_2D[3] = {0.0, 0.4};
float distruptor_2D[3] = {0.0, 0.2};
float indigens_2D[3] = {0.0, 0.6};
float opulens_2D[3] = {0.0, -0.6};
float station1_2D[3] = {0.6, 0.0};
float station2_2D[3] = {-0.6, 0.0};
 
float laser1_3D[3] = {0.4, 0.0, 0.0};
float laser2_3D[3] = {-0.4, 0.0, 0.0};
float shield_3D[3] = {0.0, 0.0, 0.4};
float distruptor_3D[3] = {0.0, 0.0, -0.4};
float indigens_3D[3] = {0.0, 0.35, 0.2};
float opulens_3D[3] = {0.0, -.35, -0.2};
float station1_3D[3] = {0.6, 0.0, -0.5};
float station2_3D[3] = {-0.6, 0.0, 0.5};
 
float SphereTarget[3] = {0.0, 0.0, 0.0};
 
int i;
for(i = 0; i < 2; i++){
    SphereTarget[i] = laser1_2D[i];     //laser1_2D[i] can be replaced by any other array to change locations;
     
    }
 
ZRSetPositionTarget(SphereTarget);
 
DEBUG(("time is %f\n fuel is %f\n distance to target is%f\n",
time, PgetPercentFuelRemaining(),
sqrt(mathSquare(myState[0]-SphereTarget[0]) + mathSquare(myState[1]-SphereTarget[1]) + mathSquare(myState[2]-SphereTarget[2]))));

//END::PROC::ZRUser
}
void ZRInit01()
{
//BEGIN::PROC::ZRInit

//END::PROC::ZRInit
}
//User-defined procedures

