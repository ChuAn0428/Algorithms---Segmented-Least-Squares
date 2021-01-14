/*
CSCI 580 Algorithm Design, Analysis, and Implementation
Segmented Least Squares algorithm
Author: Chu-An Tsai
*/

#include "utility.h"

struct Points
{
	double x;
	double y;
};

struct ABvalues
{
	double a;
	double b;
};

int main()
{
	// Read data from file
	ifstream dataFile;
	dataFile.open("points.txt");
	Points points[16];
	int C = 1, n = 0, i = 0, j = 0, k = 0;

	// Give a warning and exit if fail to read the file
	if (dataFile.fail())
	{
		cout << "Unable to find input file!";
		exit(1);
	}

	// Load the points into the array 
	double tempX, tempY;
	//cout << "The file contains the following points:" << endl; 	// test
	while (dataFile >> tempX >> tempY)
	{
		points[n].x = tempX;
		points[n].y = tempY;
		//cout << points[n].x << "\t" << points[n].y << endl;        // test
		n++;
	}
	//cout << endl;

	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(4);

	// Compute the err(i,j) table
	double errTable[16][16];	// for err(i,j)
	double a[16][16];			// for a(i,j)
	double b[16][16];			// for b(i,j)
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			double tempErr = 0;
			if (i >= j)
			{
				tempErr = 0;
			}
			else
			{
				// Summations of X, Y, X^2, and XY for different (i,j) pairs
				double sumX = 0, sumY = 0, sumXX = 0, sumXY = 0;
				for (k = i; k <= j; k++)
				{
					sumX = sumX + points[k].x;
					sumY = sumY + points[k].y;
					sumXX = sumXX + (points[k].x * points[k].x);
					sumXY = sumXY + (points[k].x * points[k].y);
				}

				// Compute the a and b by using equations (2) and (3)
				int N = j - i + 1;
				a[i][j] = (N * sumXY - sumX * sumY) / (N * sumXX - sumX * sumX);
				b[i][j] = (sumY - a[i][j] * sumX) / N;

				// Compute The err(i,j) by using equation (1)
				for (k = i; k <= j; k++)
				{
					tempErr = tempErr + (points[k].y - a[i][j] * points[k].x - b[i][j]) * (points[k].y - a[i][j] * points[k].x - b[i][j]);
				}
			}
			// Store err(i,j) values in the Table
			errTable[i][j] = tempErr;

			/*
			// Print the err(i,j) table.
			if (j >= i)
			{
				cout << "err(" << i + 1 << "," << j + 1 << ") = " << errTable[i][j] << "\t";
			}
			if (j < i)
			{
				cout << "err(" << i + 1 << "," << j + 1 << ") = " << "X\t\t";
			}
			*/
		}
		//cout << endl; // For printing err table use
	}
	//cout << endl;

	// OPT table
	// OPT(j) and corresponding index (breakpoint(i))
	double opt[17];
	int optIndex[17];
	opt[0] = 0;
	opt[1] = 0;
	optIndex[0] = -858993460;
	optIndex[1] = -858993460;
	for (j = 2; j < n + 1; j++)
	{
		double tempMin = INFINITY;
		int tempIndex = 0;
		for (i = 1; i < j; i++)
		{
			// Compute OPT(j) by using equation (4)
			double min = errTable[i - 1][j - 1] + C + opt[i];
			if (min < tempMin)
			{
				tempMin = min;
				tempIndex = i;
			}
		}

		opt[j] = tempMin;
		optIndex[j] = tempIndex;
	}

	/*
	// Print the OPT(j)
	cout << "OPT(j):\t";
	for (i = 0; i < n + 1; i++)
	{
		cout << opt[i] << " ";
	}
	cout << endl;
	// Print the OPT Index (breakpoint(i))
	cout << "Index:\t";
	for (i = 0; i < n + 1; i++)
	{
		cout << optIndex[i] << "      ";
	}
	cout << endl;
	*/

	cout.unsetf(ios::fixed);
	cout.unsetf(ios::showpoint);
	cout.precision(6);

	// Output OPT table
	cout << "j   OPT(j)   breakpoint(i)" << endl;
	for (i = 0; i < n + 1; i++)
	{
		cout << i << "   " << setw(7) << left << opt[i] << "   " << optIndex[i] << endl;
	}
	
	// Find the breakpoints by walking through the OPT table from back to front
	// And pick out the corresponding a, b values
	ABvalues FinalAB[16];
	int segIJpair[16][16];
	int segNum = 0;
	for (i = n + 1; i > 1; i--)
	{
		if (optIndex[i] > optIndex[i - 1])
		{
			FinalAB[segNum].a = a[optIndex[i]-1][n-1];
			FinalAB[segNum].b = b[optIndex[i]-1][n-1];
			segIJpair[segNum][0] = optIndex[i];
			segIJpair[segNum][1] = n;
			segNum++;
			n = optIndex[i];
		}
	}

	// Print the segmented line equations
	for (i = 0; i < segNum; i++)
	{
		cout << "line for segment " << segIJpair[i][0] << " through " << segIJpair[i][1] << " is y = " << FinalAB[i].a << "x + (" << FinalAB[i].b << ")" << endl;
	}
		
	return 0;
}
