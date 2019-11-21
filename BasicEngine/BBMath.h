#pragma once
//==========================================================================================
// File Name: BBMath.h
// Author: Brian Blackmon
// Date Created: 10/31/2019
// Purpose: 
// A header containing the common math functions I use.
//==========================================================================================

#define EPSILON 0.1f

float absoluteValue(const float value);
int absoluteValue(const int value);

void clamp(float& value, const float min, const float max);
void clamp(int& value, const int min, const int max);

void fMax(float& value, const float a, const float b);
void iMax(int& value, const int a, const int b);

void fMin(float& value, const float a, const float b);
void iMin(int& value, const int a, const int b);

float roundThousand(const float value);
float roundHundred(const float value);