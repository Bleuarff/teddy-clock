/*******************************************************************************

  Computes PWM values for each RGB channel.

  Each channel is computed by using a set of affine functions, taken straight
  from calibration data. See resultats_calibrage.ods for data.

*******************************************************************************/
/* Modulation for hibou */

float computeBlueChannel(int lightLvl){
  float val;

  if (lightLvl < 10)
    val = 14;
  else if (lightLvl <= 50)
    val = lightLvl + 5;
  else if (lightLvl <= 100)
    val = 0.5 * lightLvl + 30;
  else if (lightLvl <= 325)
    val = 1.15 * lightLvl - 35;
  else if (lightLvl <= 770)
    val = 0.8 * lightLvl + 74;
  else
    val = 700;

  return val;
}

float computeRedChannel(int lightLvl){
  float val;

  if (lightLvl < 10)
    val = 10;
  else if (lightLvl <= 50)
    val = 0.37 * lightLvl + 6.7;
  else if (lightLvl <= 110)
    val = 0.3 * lightLvl + 10;
  else if (lightLvl <= 325)
    val = 0.23 * lightLvl + 24.5;
  else if (lightLvl <= 385)
    val = 0.85 * lightLvl - 176;
  else if (lightLvl <= 500)
    val = 2.17 * lightLvl - 687;
  else
    val = 400;

  return val;
}

float computeGreenChannel(int lightLvl){
  float val;

  if (lightLvl < 10)
    val = 1;
  else if (lightLvl <= 50)
    val = 0.1 * lightLvl + 0.12;
  else if (lightLvl <= 110)
    val = lightLvl - 94;
  else if (lightLvl <= 325)
    val = 0.02 * lightLvl + 14;
  else if (lightLvl <= 385)
    val = 0.34 * lightLvl - 90;
  else if (lightLvl <= 500)
    val = 0.52 * lightLvl - 160;
  else
    val = 100;

  return val;
}

/* Modulation for herisson */
// float computeBlueChannel(int lightLvl){
//   float val;
//
  // if (lightLvl <= 10)
  //   val = 30;
  // else if (lightLvl < 20)
  //   val = 5 * lightLvl;
  // else if (lightLvl < 70)
  //   val = 100;
  // else if (lightLvl < 170)
  //   val = lightLvl * 1.8 - 17;
  // else if (lightLvl < 270)
  //   val = lightLvl * 1.165 + 88;
  // else if (lightLvl <= 350)
  //   val = lightLvl * 2.439 - 254;
  // else if (lightLvl <= 420)
  //   val = lightLvl * 1.515 + 70;
  // else if (lightLvl <= 510)
  //   val = lightLvl * 1.087 + 248;
  // else if (lightLvl <= 600)
  //   val = lightLvl * 2.439 - 439;
  // else
  //   val = 1024;
//
//   return val;
// }
//
// float computeRedChannel(int lightLvl){
//   float val;
//
//   if (lightLvl <= 10)
//     val = 30;
//   else if (lightLvl <= 20)
//     val = 2.308 * lightLvl + 23;
//   else if (lightLvl <= 70)
//     val = lightLvl * 0.667 + 56;
//   else if (lightLvl < 170)
//     val = lightLvl  + 35;
//   else if (lightLvl < 270)
//     val = lightLvl * 0.486 + 119;
//   else if (lightLvl <= 420){
//     val = lightLvl * 1.220 - 76;
//     val = min(val, (float)350);
//   }
//   else if (lightLvl < 600){
//     val = lightLvl * 0.543 + 123;
//     val = min(val, (float)400);
//   }
//   else if (lightLvl <= 730)
//     val = lightLvl * 0.909 - 136;
//   else
//     val = 1024;
//
//   return val;
// }
//
// float computeGreenChannel(int lightLvl){
//   float val;
//
//   if (lightLvl <= 10)
//     val = 60;
//   else if (lightLvl <= 20)
//     val = lightLvl * 4.615 + 47;
//   else if (lightLvl <= 70)
//     val = lightLvl * 1.333 + 113;
//   else if (lightLvl < 170)
//     val = lightLvl * 2 + 70;
//   else if (lightLvl < 270)
//     val = lightLvl * 0.971 + 239;
//   else if (lightLvl <= 420){
//     val = lightLvl * 2.439 - 153;
//     val = min(val, (float)700);
//   }
//   else if (lightLvl < 600){
//     val = lightLvl * 1.087 + 247;
//     val = min(val, (float)800);
//   }
//   else
//     val = 1024;
//
//   return val;
// }
