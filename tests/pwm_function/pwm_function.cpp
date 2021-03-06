#include <iostream>
#include <cmath>
using namespace std;

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
  else if (lightLvl < 420){
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

// loops over datapoints, computes values for a channel and compare with calibrated (reference) values
void computeChannel(char channel, int ambiant[], int reference[], int length){
  cout << "ambiant\tref\tcpted\terr\n";

  for (int i = 0; i < length; i++){
    int value = -1;

    if (channel == 'r')
      value = (int)computeRedChannel(ambiant[i]);
    else if (channel == 'g')
      value = (int)computeGreenChannel(ambiant[i]);
    else if (channel == 'b')
      value = (int)computeBlueChannel(ambiant[i]);

    // get difference percentage betwwen computed value and calibrated reference
    float err = abs((float)reference[i] - value) / reference[i] * 100;

    cout << ambiant[i] << "\t" << reference[i] << "\t" << value << "\t" << err << "%\n";
  }
}

int main()
{
  cout.precision(2);

  int length = 14;
  int ambiant_blue[length] = {0, 7, 15, 20, 65, 165, 268, 350, 416, 508, 590, 605, 700, 774}; // ambiant values (blue)
  int ref_blue[length] = {40, 40, 80, 100, 100, 280, 400, 600, 700, 800, 1000, 1000, 1000, 1024}; // calibrated blue values

  cout << "\t--  BLUE  --" << endl;
  computeChannel('b', ambiant_blue, ref_blue, length);

  length = 12;
  int ambiant_red[length] = {0, 7, 20, 65, 165, 268, 350, 416, 508, 590, 700, 774}; // ambiant values (red & green)
  int ref_red[length] = {40, 40, 70, 100, 200, 250, 350, 350, 400, 400, 500, 1024}; // calibrated red values

  cout << "\n\n\t--  RED  --" << endl;
  computeChannel('r', ambiant_red, ref_red, length);

  int ref_green[length] = {80, 80, 140, 200, 400, 500, 700, 700, 800, 800, 1000, 1024}; // calibrated green values
  cout << "\n\n\t--  GREEN  --" << endl;
  computeChannel('g', ambiant_red, ref_green, length);
  return 0;
}
