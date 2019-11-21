#include "BBMath.h"

//=============================================================================
// Function: float absoluteValue(const float)
// Description:
// An absolute value function.
// Parameters:
// const float value - The value to check.
// Output:
// float
// Returns the absolute value.
//=============================================================================
float absoluteValue(const float value)
{
	if (value < 0.0f)
	{
		return value * -1.0f;
	}

	return value;
}

//=============================================================================
// Function: int absoluteValue(const int)
// Description:
// An absolute value function.
// Parameters:
// const int value - The value to check.
// Output:
// int
// Returns the absolute value.
//=============================================================================
int absoluteValue(const int value)
{
	if (value < 0)
	{
		return value * -1;
	}

	return value;
}

//=============================================================================
// Function: void clamp(float&, const float, const float)
// Description:
// Clamps the value to make sure it's between the min and max.
// Parameters:
// float& value - The value to clamp.
// const float min - The min value.
// const float max - The max value.
//=============================================================================
void clamp(float& value, const float min, const float max)
{
	if (value < min)
	{
		value = min;
	}
	else if (max < value)
	{
		value = max;
	}
}

//=============================================================================
// Function: void clamp(int&, const int, const int)
// Description:
// Clamps the value to make sure it's between the min and max.
// Parameters:
// int& value - The value to clamp.
// const int min - The min value.
// const int max - The max value.
//=============================================================================
void clamp(int& value, const int min, const int max)
{
	if (value < min)
	{
		value = min;
	}
	else if (max < value)
	{
		value = max;
	}
}

//=============================================================================
// Function: void fMax(float&, const float, const float)
// Description:
// Sets the value to the larger value of the two.
// Parameters:
// float& value - The value to set.
// const float a - The first value to compare.
// const float b - The second value to compare.
//=============================================================================
void fMax(float& value, const float a, const float b)
{
	if (a < b)
	{
		value = b;
	}
	else
	{
		value = a;
	}
}

//=============================================================================
// Function: void iMax(int&, const int, const int)
// Description:
// Sets the value to the larger value of the two.
// Parameters:
// int& value - The value to set.
// const int a - The first value to compare.
// const int b - The second value to compare.
//=============================================================================
void iMax(int& value, const int a, const int b)
{
	if (a < b)
	{
		value = b;
	}
	else
	{
		value = a;
	}
}

//=============================================================================
// Function: void fMin(float& value, const float, const float)
// Description:
// Compares two values and sets the value to the smaller of the two.
// Parameters:
// float& value - The value to set.
// const float a - The first value to compare.
// const float b - The second value to compare.
//=============================================================================
void fMin(float& value, const float a, const float b)
{
	if (a < b)
	{
		value = a;
	}
	else
	{
		value = b;
	}
}

//=============================================================================
// Function: void iMin(int&, const int, const int)
// Description:
// Compares two values and sets the value to the smaller of the two.
// Parameters:
// int& value - The value to set.
// const int a - The first value to compare.
// const int b - The second value to compare.
//=============================================================================
void iMin(int& value, const int a, const int b)
{
	if (a < b)
	{
		value = a;
	}
	else
	{
		value = b;
	}
}

//=============================================================================
// Function: float roundThousand(const float)
// Description:
// Rounds a float to the thousands place.
// Parameters:
// const float value - The value to round.
// Output:
// float
// Returns the rounded float.
//=============================================================================
float roundThousand(const float value)
{
	int temp = (int)(value * 1000.0f + 0.5f);
	float huh = temp / 1000.0f;
	return (float)temp / 1000.0f;
}

//=============================================================================
// Function: float roundHundred(const float)
// Description:
// Rounds a float to the hundredths place.
// Parameters:
// const float value - The value to round.
// Output:
// float
// Returns the rounded float.
//=============================================================================
float roundHundred(const float value)
{
	int temp = (int)(value * 100.0f + 0.5f);

	return (float)((double)temp / 100.0);
}