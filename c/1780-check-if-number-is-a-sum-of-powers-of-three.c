#include "leetcode.h"

bool checkPowersOfThree(int n) 
{
	const double ERROR_MARGIN = 0.001;
	double closestPower = log(n) / log(3);
	if (closestPower + ERROR_MARGIN >= ceil(closestPower)) closestPower = ceil(closestPower);
	else closestPower = floor(closestPower);
	double lastClosestPower = 0;

	do
	{
		int closestPowerValue = pow(3, closestPower);
		n -= closestPowerValue;
		if (n == 0) return true;

		lastClosestPower = closestPower;
		closestPower = log(n) / log(3);
		if (closestPower + ERROR_MARGIN >= ceil(closestPower)) closestPower = ceil(closestPower);
		else closestPower = floor(closestPower);
	}
	while (closestPower != lastClosestPower);

	return false;
}

