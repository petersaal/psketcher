#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "ConstraintSolver.h"
#include "../mmcMatrix/mmcMatrix.h"

using namespace std;

int main(void)
{
	vector<symbol> free_parameters;
	vector<symbol> fixed_parameters;
	vector<double> fixed_values;
	vector<double> weights;
	vector<ex> constraints;

	// Setup problem
	symbol x1("x1"), x2("x2"), x3("x3"), x4("x4");
	symbol y1("y1"), y2("y2"), y3("y3"), y4("y4");
	symbol d1("d1"), d2("d2"), d3("d3"), d4("d4");

	fixed_parameters.push_back(d1);
	fixed_parameters.push_back(d2);
	fixed_parameters.push_back(d3);
	fixed_parameters.push_back(d4);

	fixed_values.push_back(0.0);
	fixed_values.push_back(0.0);
	fixed_values.push_back(5.0);
	fixed_values.push_back(10.0);

	free_parameters.push_back(x1);
	free_parameters.push_back(x2);
	free_parameters.push_back(x3);
	free_parameters.push_back(x4);
	free_parameters.push_back(y1);
	free_parameters.push_back(y2);
	free_parameters.push_back(y3);
	free_parameters.push_back(y4);

	constraints.push_back(sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)) - d3);
	constraints.push_back(sqrt((x2-x3)*(x2-x3) + (y2-y3)*(y2-y3)) - d4);
	constraints.push_back(y2-y1);
	constraints.push_back(x3-x2);
	constraints.push_back(y3-y4);
	constraints.push_back(x4-x1);
	constraints.push_back(x1-d1);
	constraints.push_back(y1-d2);

	weights.push_back(1.0);
	weights.push_back(1.0);
	weights.push_back(1.0);
	weights.push_back(1.0);
	weights.push_back(1.0);
	weights.push_back(1.0);
	weights.push_back(1.0);
	weights.push_back(1.0);

	ConstraintSolver my_constraints(constraints, weights, free_parameters,
                                    fixed_parameters, fixed_values);

	mmcMatrix x(8,1);

	x(0,0) = 0.0;
	x(1,0) = 5.0;
	x(2,0) = 5.0;
	x(3,0) = 0.0;
	x(4,0) = 0.0;
	x(5,0) = 0.0;
	x(6,0) = 10.0;
	x(7,0) = 10.0;

	x(0,0) = 1.0;
	x(1,0) = -4.0;
	x(2,0) = 6.0;
	x(3,0) = -2.0;
	x(4,0) = 2.0;
	x(5,0) = -4.0;
	x(6,0) = 20.0;
	x(7,0) = -10.0;


	cout << "grad magnitude = " << my_constraints.GetMeritGradient(x).GetMagnitude() << endl;
	cout << "grad difference = " << (my_constraints.GetMeritGradient(x) - my_constraints.GetNumericalGradient(x,1e-6)).GetMagnitude() << endl;

	mmcMatrix solution = my_constraints.MinimizeMeritFunction(x, 1000, 1e-10, 1e-15, 50, 1, &cout);

	cout << "solution = " << solution << endl;

	return 0;
}

