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
#define TAU 6.283185307

float opulens[3] = {0, -.6, 0};
float disruptor[3] = {0, 0.2, 0};
float att_target[3];
float direction[3];

if (PdisruptorUpgraded()) {
    mathVecSubtract(att_target, otherState, myState, 3);
    mathVecNormalize(att_target, 3);
    ZRSetAttitudeTarget(att_target);
    
    mathVecSubtract(direction, otherState, myState, 3);
    mathVecNormalize(direction, 3);
    if (acosf(mathVecInner(direction, &myState[6], 3)) < TAU/30) {
        Prepulsor();
        DEBUG(("time: %f, shooting\n", time));
    }
    ZRSetPositionTarget(opulens);
}
else {
    ZRSetPositionTarget(disruptor);
}
//END::PROC::ZRUser
}
void ZRInit01()
{
//BEGIN::PROC::ZRInit

//END::PROC::ZRInit
}
//User-defined procedures

