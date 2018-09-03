#pragma once
#include "common.h"

#ifndef M_PI	// cmath doesn't define M_PI if it's C99 comformant
#define M_PI 3.14159265358979323846
#endif

int getSin(uint8_t deg)  attrConst;
int getCos(uint8_t deg)  attrConst;
uint8_t getAtan(int x, int y) attrConst;

int random(int32_t mi, int32_t ma);
int sign(int x)  attrConst;
int clamp(int x, int mi, int ma)  attrConst;

double sinc(double x) attrConst;
int getGCD(int x, int y) attrConst;
