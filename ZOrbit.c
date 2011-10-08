float center[3];
float radius;
float TAU;

//float desiredAngularVelocity; 4 deg per sec, speed for max pts

float velTarget[3];          // vector btwn you and posTarget, where you wanna be
float angleTarget;          // angle on circle of where you wanna be
float posTarget[3];        // position along circle at angleTarget
float currentAngle;       // angle along circle that you are at
float angleStep;         // angle that is increment for each second

TAU = 6.28318;
center[0] = 0;
center[1] = 0.6;
center[2] = 0;
radius = .3;

currentAngle = atan2f(myState[1] - center[1], myState[0] - center[0]);

  //tweak angleStep as neccesary
angleStep = TAU / 90;

//desiredAngularVelocity = TAU / 90;

angleTarget = currentAngle + angleStep;

posTarget[0] = radius * cosf(angleTarget) + center[0];
posTarget[1] = radius * sinf(angleTarget) + center[1];

velTarget[0] = posTarget[0] - myState[0];
velTarget[1] = posTarget[1] - myState[1];
velTarget[2] = posTarget[2] - myState[2];

//Makes velTarget a unit vector?
//mathVecNormalize(velTarget, 3);

//??
//velTarget[0] *= radius * desiredAngularVelocity;
//velTarget[1] *= radius * desiredAngularVelocity;
//velTarget[2] *= radius * desiredAngularVelocity;

ZRSetVelocityTarget(velTarget);
