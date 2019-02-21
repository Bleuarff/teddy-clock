/*******************************************************************************

  Computes PWM values for each RGB channel.

  Each channel is computed by using a set of affine functions, taken straight
  from calibration data. See resultats_calibrage.ods for data.

*******************************************************************************/

float computeBlueChannel(int lightLvl){
  float val;

  if (lightLvl < 10)
    val = 40;
  else if (lightLvl < 20)
    val = 5 * lightLvl;
  else if (lightLvl < 70)
    val = 100;
  else if (lightLvl < 170)
    val = lightLvl * 1.8 - 17;
  else if (lightLvl < 270)
    val = lightLvl * 1.165 + 88;
  else if (lightLvl <= 350)
    val = lightLvl * 2.439 - 254;
  else if (lightLvl <= 420)
    val = lightLvl * 1.515 + 70;
  else if (lightLvl <= 510)
    val = lightLvl * 1.087 + 248;
  else if (lightLvl <= 600)
    val = lightLvl * 2.439 - 439;
  else
    val = 1024;

  return val;
}

float computeRedChannel(int lightLvl){
  float val;

  if (lightLvl < 10)
    val = 40;
  else if (lightLvl <= 20)
    val = 2.308 * lightLvl + 23;
  else if (lightLvl <= 70)
    val = lightLvl * 0.667 + 56;
  else if (lightLvl < 170)
    val = lightLvl  + 35;
  else if (lightLvl < 270)
    val = lightLvl * 0.486 + 119;
  else if (lightLvl <= 420){
    val = lightLvl * 1.220 - 76;
    val = min(val, (float)350);
  }
  else if (lightLvl < 600){
    val = lightLvl * 0.543 + 123;
    val = min(val, (float)400);
  }
  else if (lightLvl <= 730)
    val = lightLvl * 0.909 - 136;
  else
    val = 1024;

  return val;
}

float computeGreenChannel(int lightLvl){
  float val;

  if (lightLvl < 10)
    val = 80;
  else if (lightLvl <= 20)
    val = lightLvl * 4.615 + 47;
  else if (lightLvl <= 70)
    val = lightLvl * 1.333 + 113;
  else if (lightLvl < 170)
    val = lightLvl * 2 + 70;
  else if (lightLvl < 270)
    val = lightLvl * 0.971 + 239;
  else if (lightLvl <= 420){
    val = lightLvl * 2.439 - 153;
    val = min(val, (float)700);
  }
  else if (lightLvl < 600){
    val = lightLvl * 1.087 + 247;
    val = min(val, (float)800);
  }
  else
    val = 1024;

  return val;
}
