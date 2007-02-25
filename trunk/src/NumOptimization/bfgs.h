#ifndef bfgsH
#define bfgsH

#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include "../mmcMatrix/mmcMatrix.h"

enum LINE_SEARCH {GOLDEN_SECTION, BACK_TRACK}; 

double GetRandomNumber(double lower_limit, double upper_limit);

class MeritFunction
{
public:
	
	//Constructors and destructors (must be overridden by child class)
	MeritFunction(LINE_SEARCH line_search = BACK_TRACK) {LineSearch = line_search;} //The child class virtual constructor must initialize NumDimensions
	MeritFunction(int num_dims, LINE_SEARCH line_search = BACK_TRACK) {NumDimensions = num_dims; LineSearch = line_search;}
	virtual ~MeritFunction() {} 
	
	
	//Virtual methods that must be overridden by the child class
	virtual double GetMeritValue(const mmcMatrix & x)= 0;      //Evaluates merit function at x
	virtual mmcMatrix GetMeritGradient(const mmcMatrix & x) {return GetNumericalGradient(x);} //Evaluate gradient of merit function at x
	virtual void GetMeritValuePlusGradient(const mmcMatrix & x, double &value, mmcMatrix &gradient){value = GetMeritValue(x);
                                                                                                                                                               gradient = GetMeritGradient(x);}
	mmcMatrix GetNumericalGradient(const mmcMatrix & x, double tolerance = 1.0e-10);

	//acessors
	int GetNumDims() const {return NumDimensions;}
	void SetNumDims(int num_dims) {NumDimensions = num_dims;}
	
	//Methods that are not virtual
	mmcMatrix MinimizeMeritFunction(const mmcMatrix &x_init, double search_distance, double tolerance, double mult_gold_resolution, int maxit, int verbose_level, std::ostream *output_buffer = &std::cout, int max_merit_evals = 0);
	mmcMatrix GetNextBfgsSearchDir(const mmcMatrix &current_position, 
									const mmcMatrix &previous_position,
									const mmcMatrix &current_gradient,
									const mmcMatrix &previous_gradient,
									const mmcMatrix &prev_inv_hessian,
									mmcMatrix &new_inv_hessian)const;
    mmcMatrix ConjugateGradient(const mmcMatrix &x_init, double search_distance, double tolerance, double mult_gold_resolution, int maxit, int verbose_level, std::ostream *output_buffer);

	double  GetLambdaLimit(const mmcMatrix & x_ref, const mmcMatrix & search_dir);
	double  GetF(const mmcMatrix & search_dir);
	
	// Golden Section line search function
	void MultGold(double lambda_u_init, double f, const mmcMatrix &search_dir,
		          const mmcMatrix &xref, double *lambda_lower_final, double *lambda_upper_final);
		
	// Backtracking line search function
	double BackTrack(const mmcMatrix &position, const mmcMatrix &gradient, const double &initial_merit, mmcMatrix &search_dir, double max_step,
	                 double step_tol, double alpha, double beta, bool &error, mmcMatrix &new_gradient, double &new_merit);

	mmcMatrix MonteCarloOptimization(const mmcMatrix &x_init, const mmcMatrix &x_delta, int number_iterations, int verbose_level = 0);

	//Exception class
	class MeritFunctionException{};

private: 
	int NumDimensions;
	double SearchDistance;
	double MultGoldResolution;
	int VerboseLevel;
	int MaxMeritEvals;
	int MeritEvals;
	std::ostream *out_buf;
	LINE_SEARCH LineSearch;
};


#endif //bfgsH
