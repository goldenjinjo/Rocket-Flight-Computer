float simpsons4(float timeStep, float der0, float der1, float der2, float derCurr, float valCurr){
  float integratedVal = valCurr + timeStep/24*(55*derCurr-59*der2+37*der1-9*der0);
  return integratedVal;
}