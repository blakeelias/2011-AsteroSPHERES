void Orbit(float center[3], float radius){

float desiredAngularVelocity;
float velTarget[3];
float angleTarget;

    angleStep = TAU * radius / 4;

desiredAngularVelocity = 2.0 * TAU / 180;

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
