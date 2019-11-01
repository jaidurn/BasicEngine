#pragma once
//==========================================================================================
// File Name: BBMath.h
// Author: Brian Blackmon
// Date Created: 10/31/2019
// Purpose: 
// A header containing the common math functions I use.
//==========================================================================================

float absoluteValue(const float value);
int absoluteValue(const int value);

void clamp(float& value, const float min, const float max);
void clamp(int& value, const int min, const int max);