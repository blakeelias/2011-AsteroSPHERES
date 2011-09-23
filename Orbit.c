void Orbit(float center[3], float radius, float myState[3]){

float desiredAngularVelocity;
float velTarget[3];
float angleTarget;
float posTarget[3];
float currentAngle;
float angleStep;

currentAngle = atan2(myState[1] - center[1], myState[0] - center[0]);

  //tweak angleStep as neccesary
angleStep = TAU * radius / 4;

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

}
