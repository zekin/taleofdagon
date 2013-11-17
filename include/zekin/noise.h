#ifndef NOISE_H
#define NOISE_H
#include <math.h>

double generateNoise(unsigned int x)
{
  x = (x<<13) ^ x;
  return (1.0 - ((x*(x*x*15731+789221)+1376312589) & 0x7fffffff) / 1073741824.0);
}

double generateNoiseXY(unsigned int x, unsigned int y)
{
  unsigned int v = x+y*37;
  v=(v<<13)^v;
  return (1.0 - ((v*(v*v*15731+789221)+1376312589) & 0x7fffffff) / 1073741824.0);
  
}
double getSmoothedNoise(unsigned int x)
{
  double r1=generateNoise(x-1);
  double r2=generateNoise(x);
  double r3=generateNoise(x+1);
  
  return r1 / 4  + r2 / 2 + r3 / 4;
}
double getSmoothedNoiseXY(unsigned int x, unsigned int y)
{
  const double dividerCorners = 16.0;
  const double dividerSides   = 8.0;
  const double dividerMiddle  = 4.0;
  
  double noiseTopLeft = generateNoiseXY(x-1,y-1);
  double noiseTopRight= generateNoiseXY(x+1,y-1);
  double noiseBotLeft = generateNoiseXY(x-1,y+1);
  double noiseBotRight= generateNoiseXY(x+1,y+1);
  
  double noiseTop     = generateNoiseXY(x  ,y-1);
  double noiseLeft    = generateNoiseXY(x-1,y  );
  double noiseRight   = generateNoiseXY(x+1,y  );
  double noiseBot     = generateNoiseXY(x  ,y+1);
  
  double noiseCenter  = generateNoiseXY(x  ,y  );
  
  return (noiseTopLeft + noiseTopRight + noiseBotLeft + noiseBotRight) / dividerCorners +
         (noiseTop + noiseLeft + noiseRight + noiseBot ) / dividerSides +
         (noiseCenter) / dividerMiddle;
//  double noiseMidRight= 
}
double linearInterpolate(double a, double b, double x)
{
  return a*(1-x) + b*x;  
}
double cosineInterpolate(double a, double b, double x)
{
  double ft = x * 3.1415926535;
  double f = (1 - cos(ft)) * 0.5;
  
  return a*(1-f) + b*f;
}
double getInterpolatedNoise(double x)
{
  int intX = (int)x;
  float fracX = x-intX;
  double r1=getSmoothedNoise(intX);
  double r2=getSmoothedNoise(intX+1);  
  
  return cosineInterpolate(r1,r2,fracX);
}
double getInterpolatedNoiseXY(double x, double y)
{
  int intX = (int)x;
  int intY = (int)y;
  double fracX = x - intX;
  double fracY = y - intY;
  double s = getSmoothedNoiseXY(intX  ,intY);
  double t = getSmoothedNoiseXY(intX+1,intY);
  double u = getSmoothedNoiseXY(intX+1,intY+1);
  double v = getSmoothedNoiseXY(intX  ,intY+1);
  
  double l = linearInterpolate(s,t,fracX);
  double b = linearInterpolate(v,u,fracX);
  
  return cosineInterpolate(l,b, fracY);
  
}

double fbm(double x, double persistance, int passes)
{
  double total = 0;
  double multiplier = 1.0;
  double divisor = 0.0;
  int i=0;
  
  for (i=0; i<passes; i++)
  {
    total += getInterpolatedNoise((1<<i)*x)*multiplier;
    divisor += multiplier;
    multiplier /= 2;//pow(persistance,i+1);
  }
  total /= divisor;
  return total;
}
double fbmXY(double x, double y, double persistance, int passes)
{
  double total = 0;
  double multiplier = 1.0;
  double divisor = 0.0;
  int i=0;
  
  for (i=0; i<passes; i++)
  {
    total += getInterpolatedNoiseXY((1<<i)*x,(1<<i)*y)*multiplier;
    divisor += multiplier;
    multiplier /= pow(persistance,i+1);
  }
  total /= divisor;
  return total;
}

#endif