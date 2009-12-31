
using System;
using dnAnalytics.LinearAlgebra;

namespace pSketcher.NumericalOptimization
{
    enum LineSearchMethod { BackTrack, GoldenSection}

    /// <summary>
    /// EventArgs for MeritFunction's PrintProgressUpdate event
    /// </summary>
    sealed class PrintProgressUpdateEventArgs : EventArgs
    {
        public readonly string ProgressUpdate;

        public PrintProgressUpdateEventArgs (string progress_update)
        {
             ProgressUpdate = progress_update;
        }
    }

    abstract class MeritFunction
    {
        //
        // Field data and properties
        //
        public readonly int NumDimensions;
        public LineSearchMethod LineSearch { get; set; }
        public int VerboseLevel { get; set; }
        public int MaxMeritEvals { get; set; }
        public int NumMeritEvals { get; private set; }

        //
        // Merit function value and gradient methods
        //
        public abstract double GetMeritValue(Vector x);

        public virtual Vector GetMeritGradient(Vector x)
        {
            return GetNumericalGradient(x);
        }

        public virtual void GetMeritValuePlusGradient(Vector x, out double @value, out Vector gradient)
        {
            @value = GetMeritValue(x);
            gradient = GetMeritGradient(x);
        }

        protected Vector GetNumericalGradient(Vector x)
        {
            return GetNumericalGradient(x, 1.0e-10);
        }

        protected Vector GetNumericalGradient(Vector x, double tolerance)
        {
            Vector upper_x = x;
            Vector lower_x = x;
            double upper_y;
            double lower_y;

            Vector result = new DenseVector(NumDimensions);

            //insure that tolerance is a positive non-zero value
            if(tolerance <= 0.0)
                throw new ArgumentException("The parameter tolerance must have a value greater than 0.0");

            //Check to insure that x is of the correct size
            if(x.Count != NumDimensions)
                throw new ArgumentException("The parameter x does not have the correct number of elements.");

            for(int counter = 0; counter < NumDimensions; counter++)
            {
                if(counter != 0)
                    upper_x[counter-1] = x[counter-1];

                upper_x[counter] += 0.5 * tolerance;

                if(counter != 0)
                    lower_x[counter-1] = x[counter-1];

                lower_x[counter] += -0.5 * tolerance;

                upper_y = GetMeritValue(upper_x);
                lower_y = GetMeritValue(lower_x);

                result[counter] = (upper_y - lower_y) / tolerance;
            }

            return result;
        }


        //
        // Constructors
        //

        public MeritFunction (int num_dimensions, LineSearchMethod line_search)
        {
            //  Insure that num_dim is valid
            if(num_dimensions <= 0 )
                throw new ArgumentException("The argument num_dimensions must have a value greater than zero.");

            NumDimensions = num_dimensions;

            LineSearch = line_search;
        }

        //
        // Event handlers
        //

        /// <summary>
        /// Event delegate which handles printing progress updates
        /// </summary>
        public event EventHandler<PrintProgressUpdateEventArgs> PrintProgressUpdate;

        /// <summary>
        /// Method used to fire the PrintProgressUpdate event
        /// </summary>
        /// <param name="progress_update">
        /// A <see cref="System.String"/>
        /// </param>
        protected virtual void OnPrintProgressUpdate (string progress_update)
        {
            if (PrintProgressUpdate != null) PrintProgressUpdate(this, new PrintProgressUpdateEventArgs(progress_update));
        }


        //
        // Methods
        //

        public Vector Minimize(Vector x_init, double search_distance, double tolerance,
                               double mult_gold_resolution, int maxit, int verbose_level,
                               int max_merit_evals)
        {
            this.VerboseLevel = verbose_level;

            this.MaxMeritEvals = max_merit_evals;
            this.NumMeritEvals = 0;

            double SearchDistance = search_distance;
            double MultGoldResolution  = mult_gold_resolution;


            //
            //  Local declarations
            //
            Vector prev_gradient;
            double prev_merit, new_merit = 0;
            Vector current_gradient = null;
            Matrix prev_inv_hessian = new DenseMatrix(NumDimensions);
            Matrix current_inv_hessian;
            Vector x_previous;
            Vector search_dir;
            Vector x_best = x_init;
            double slope;
            double lambda_upper_limit;
            double lambda_l_final, lambda_u_final, lambda_mid;
            double search_vector_mag;
            double f;

            if(VerboseLevel >= 2)
            {
              OnPrintProgressUpdate("Initial x position = \n" + x_init);
            }

            //
            //  Start of the actual implementation
            //

            //  assign prev_inv_hessian_pp to the identity matrix
            prev_inv_hessian.SetDiagonal(new DenseVector(NumDimensions,1.0));

            //  set the current x equal to the initial x
            x_previous = x_init;

            //  Calculate the gradient and merit value at the initial position
            GetMeritValuePlusGradient(x_init, out prev_merit, out prev_gradient);
            NumMeritEvals++;

            if(VerboseLevel >= 1)
                //display the initial merit function value
                OnPrintProgressUpdate("Initial Merit Value = " + GetMeritValue(x_init) + "\n" );

            if(VerboseLevel >= 2)
                //Display the initial gradient
                OnPrintProgressUpdate("Initial Gradient =\n" + prev_gradient);

            //
            //  Set the initial search dir to the negative of the initial gradient
            //

            //  Must first multiply the gradient by -1.0
            search_dir = -1.0*prev_gradient;

            //
            //  Now will enter iteration loop to complete optimization
            //
            for (int count = 0; count < maxit; count++)
            {
                if(LineSearch == LineSearchMethod.GoldenSection)
                {
                    // Use golden section line search


                    //  Define the lambda upper limit
                    lambda_upper_limit = SearchDistance;

                    // Call multgold to do the 1-d optimization
                    f = MultGoldResolution / (Math.Sqrt(search_dir.DotProduct(search_dir)) * SearchDistance);
                    MultGold(lambda_upper_limit, f, search_dir,x_previous,
                             out lambda_l_final, out lambda_u_final);

                    //
                    //  get the value for lambda mid in order to get a better
                    //  x position.
                    //
                    lambda_mid = 0.5 * (lambda_u_final + lambda_l_final);
                } else if(LineSearch == LineSearchMethod.BackTrack) {
                    // Use backtracking line search

                    bool back_track_error;

                    lambda_mid = BackTrack(x_previous, prev_gradient, prev_merit, search_dir, search_distance, MultGoldResolution, 1e-4, 0.9, out back_track_error, out current_gradient, out new_merit);

                    if(back_track_error && VerboseLevel >= 1)
                        OnPrintProgressUpdate("Back tracking line search could not find a satisfactory lambda value.\n");

                } else {
                    // An unimplemented line search method has been chosen
                    throw new NotImplementedException("A line search method " + LineSearch + " has not been implemented");
                }

                // Get the value for the current x position
                x_best = x_previous + lambda_mid*search_dir;

                // If print progress is on, print the current x value
                if(VerboseLevel >= 1)
                    OnPrintProgressUpdate("Iteration = " + count + ", lambda_mid = " +
                                          lambda_mid + ", Merit = " + GetMeritValue(x_best) + "\n");

                if(VerboseLevel >= 2)
                    OnPrintProgressUpdate("Current x = \n"+ x_best + "Current search dir = \n" + search_dir);

                //
                //  Must check for convergence
                //
                //  Need to first get the magnitude of the search vector
                search_vector_mag = Math.Sqrt(search_dir.DotProduct(search_dir));

                // Now the actual convergence check is preformed
                if((lambda_mid * search_vector_mag) < tolerance)
                    // The solution has converged!! Return the solution
                    return x_best;

                // Calculate the gradient at the new x_position
                if(LineSearch != LineSearchMethod.BackTrack)
                    current_gradient = GetMeritGradient(x_best);

                // Calculate the next search direction using bfgs method
                search_dir = GetNextBfgsSearchDir(x_best, x_previous, current_gradient, prev_gradient,
                                                  prev_inv_hessian, out current_inv_hessian);

                // Check that search direction is still pointing in a
                // direction to decrease the merit function.
                slope = search_dir.DotProduct(current_gradient);
                if(slope > 0) {
                    // Started going in the wrong direction,
                    // need to reset the search direction to opposite the
                    // gradient.
                    search_dir = -1.0*current_gradient;

                    // Reset the Hessian matrix to the identity
                    current_inv_hessian.Clear();
                    current_inv_hessian.SetDiagonal(new DenseVector(NumDimensions,1.0));

                    if(VerboseLevel >= 1)
                        OnPrintProgressUpdate("Started going the wrong way!!!  Now going opposite of gradient.\n");
                }


                //
                //  Need to reset prev_gradient, prev_inv_hessian and
                //  x_previous
                //
                prev_gradient = current_gradient;
                prev_merit = new_merit;
                prev_inv_hessian = current_inv_hessian;
                x_previous = x_best;

                // Check to see if the maximum number of evaluations of the merit funciton has been reached
                if(MaxMeritEvals != 0 && NumMeritEvals > MaxMeritEvals)
                {
                    if(VerboseLevel >= 1)
                        OnPrintProgressUpdate("Max number of merit function evaluations reached. Stopping search.\n");

                    break;
                }
            }

            // optimization went to the max number of iterations
            return(x_best);
        }

        protected void MultGold(double lambda_u_init, double f, Vector search_dir,
                             Vector xref, out double lambda_lower_final,
                             out double lambda_upper_final)
        {
            //
            //  Local declarations
            //
            double gold_num;
            double lambda_l_bnd, lambda_u_bnd;
            double lambda_l, lambda_u;
            double lambda_m_l, lambda_m_u;
            int num_additional_computations;
            int counter;

            if(VerboseLevel >= 2)
                //Display the value of f that was input
                OnPrintProgressUpdate( "\nf = " + f + "\n");

            Vector x_trial;

            //
            //  Starts of implementation
            //

            //
            //  Need to insure that the initial upper bound on lambda is actually
            //  greater than the lower bound (the lower bound is taken to be zero).
            //
            if(!(lambda_u_init > 0))
                throw new ArgumentException("lambda_u_init must be greater than zero.");

            //
            // Need to insure that f is positive and non-zero since the
            // natural log of f will later be evaluated.
            //
            if(f <= 0)
                throw new ArgumentException("The parameter f must be greater than zero.");

            //
            // The value of the golden number is evaluated and stored for
            // later use in order to increase efficiency.
            //
            gold_num = 0.5 * (Math.Sqrt(5.0) - 1);

            //
            // Set the current limits on lambda to the initial
            // limits.
            //
            lambda_l_bnd = 0.0;
            lambda_u_bnd = lambda_u_init;

            //
            // Calculate the first two points to evaluate the merit function.
            //
            lambda_l = lambda_u_bnd - gold_num * (lambda_u_bnd - lambda_l_bnd);
            lambda_u = lambda_l_bnd + gold_num *(lambda_u_bnd - lambda_l_bnd);

            // Evaluate the actual merit function for the first two points
            x_trial = xref + lambda_l*search_dir;


            // Can now evaluate merit function at x_trial_p corresponding to the
            // lower limit of lambda
            lambda_m_l = GetMeritValue(x_trial);

            // Will repeat the same procedure for the upper bound of lambda
            x_trial = xref + lambda_u*search_dir;

            lambda_m_u = GetMeritValue(x_trial);

            //
            // Need to calculate the number of additional merit function evaluations
            // that are required.  Two evaluations of the merit function have already
            // been done.  From now on each iteration will only need to evaluate the
            // merit function once.
            //
            num_additional_computations = (int)Math.Ceiling(1 - 2.078 * Math.Log(f)) - 2;

            //
            // If more computations are needed, compute them.  Note that it is
            // possible that no more computations are needed if the user chose the
            // value of f to be 1 or greater.
            //
            if(num_additional_computations > 0)
            {
                //
                // Iteration loop.  Each iteration cuts down the uncertainty interval
                // and evaluates the merit function at one additional point.
                //

                for(counter = 0; counter < num_additional_computations; counter++)
                {
                    //
                    // Need to determine whether to keep r_bnd or l_bnd depending
                    // on which of the values m_l or m_r is smaller.  Will then find the
                    // next point to evaluate the merit function and evaluate it there.
                    //
                    if (lambda_m_l < lambda_m_u)
                    {
                        lambda_u_bnd = lambda_u;
                        lambda_u = lambda_l;
                        lambda_m_u = lambda_m_l;
                        lambda_l = lambda_u_bnd - gold_num * (lambda_u_bnd - lambda_l_bnd);

                        //  Will now evaluate the merit function with lambda_l
                        x_trial = xref + lambda_l * search_dir;

                        // Can now evaluate merit function at x_trial_p corresponding
                        // to thelower limit of lambda
                        lambda_m_l = GetMeritValue(x_trial);
                    }else{
                        lambda_l_bnd = lambda_l;
                        lambda_l = lambda_u;
                        lambda_m_l = lambda_m_u;
                        lambda_u = lambda_l_bnd + gold_num *
                        (lambda_u_bnd - lambda_l_bnd);

                        //
                        //  Will now evaluate the merit function with lambda_u
                        //  will first find out where to go from xref_p
                        // will first find out where to go from xref_p
                        //
                        x_trial = xref + lambda_u * search_dir;

                        // Can now evaluate merit function at x_trial_p corresponding
                        // to the uper limit of lambda
                        lambda_m_u = GetMeritValue(x_trial);
                    }
                }
            }

            //
            //  Will now narrow down the interval of uncertainty one last time.
            //  Will not evaluate the merit function an additional time though.
            //  Will return a value of 1 to indicate a successful computation.
            //

            // Now will return results
            if(lambda_m_l < lambda_m_u)
            {
                lambda_lower_final = lambda_l_bnd;
                lambda_upper_final = lambda_u;
            }else{
                lambda_lower_final = lambda_l;
                lambda_upper_final = lambda_u_bnd;
            }
        }

        protected double BackTrack(Vector position, Vector gradient, double initial_merit, Vector search_dir, double max_step,
                                double step_tol, double alpha, double beta, out bool error, out Vector new_gradient, out double new_merit)
        {
            error = false;

            // Calculate the current merit value
            double merit_value = initial_merit;

            // Insure that the length of the search vector is not too long
            double newton_length = search_dir.PNorm(2);

            if(newton_length > max_step)
            {
                if(VerboseLevel >= 1)
                    OnPrintProgressUpdate("Back track line search had to shorten search vector.\n");
                search_dir = (max_step / newton_length)*search_dir;
                newton_length = max_step;
            }

            // Calculate the initial slope
            double init_slope = gradient.DotProduct(search_dir);

            // Calculated the relative length of the search diriction vector
            double rel_length = search_dir[0] / position[0];
            for(int i = 1; i < NumDimensions; i++)
            {
                double temp = search_dir[i] / position[i];
                if(temp > rel_length)
                    rel_length = temp;
            }

            double min_lambda = step_tol / rel_length;

            double lambda = 1.0;

            Vector new_position;
            double prev_new_merit_value = 0;
            double lambda_temp, lambda_prev = 0, disc;
            Matrix temp1 = new DenseMatrix(2,2);
            Vector temp2 = new DenseVector(2,1);
            Vector result;
            double a,b;

            bool finished_first_backtrack = false;

            while(true) // loop is broken when lambda condition is satisfied by a return statement
            {
                new_position = position + lambda*search_dir;
                GetMeritValuePlusGradient(new_position, out new_merit, out new_gradient);
                NumMeritEvals++;

                // Check to see whether this lambda is acceptable
                if( new_merit <= merit_value + alpha*lambda*init_slope)
                {
                    // Satisfactory value of lambda found
                    error = false;
                    return lambda;

                } else if (lambda < min_lambda) {
                    // no satisfactory lambda can be found far enough away from starting position
                    error = true;
                    return 0.0;

                } else {
                    // Need to reduce lambda
                    if(!finished_first_backtrack)
                    {
                        // For fist backtrack use quadratic fit
                        lambda_temp = -init_slope / (2 * (new_merit - merit_value - init_slope));

                        finished_first_backtrack = true;

                    } else {
                        // for subsequent backtracks used cubic fit
                        temp2[0] = new_merit - merit_value - lambda*init_slope;
                        temp2[1] = prev_new_merit_value - merit_value - lambda_prev * init_slope;

                        temp1[0,0] = 1 / (lambda * lambda);
                        temp1[0,1] = -1 / (lambda_prev * lambda_prev);
                        temp1[1,0] = -lambda_prev / (lambda * lambda);
                        temp1[1,1] = lambda / (lambda_prev * lambda_prev);

                        result = ( 1/ (lambda - lambda_prev))* (temp1 * temp2);

                        a = result[0];
                        b = result[1];

                        disc = b*b - 3*a*init_slope;

                        if(a == 0)
                            // the cubic is a quadratic
                            lambda_temp = -init_slope / (2*b);
                        else
                            // the cubic is a cubic!
                            lambda_temp = (-b + Math.Sqrt(disc)) / (3.0 * a);

                        if(lambda_temp > 0.5 * lambda)
                            lambda_temp = 0.5 * lambda;
                    }

                    lambda_prev = lambda;
                    prev_new_merit_value = new_merit;

                    if(lambda_temp <= 0.1*lambda)
                        lambda = 0.1 * lambda;
                    else
                        lambda = lambda_temp;
                }

                if(MaxMeritEvals != 0 && NumMeritEvals > MaxMeritEvals)
                {
                    // Reached maximum number of merit function calls, must return whatever lamba is now
                    error = false;
                    return lambda;
                }
            } // End of while loop
        }



        protected Vector GetNextBfgsSearchDir(Vector current_position, Vector previous_position,
                                              Vector current_gradient, Vector previous_gradient,
                                              Matrix prev_inv_hessian, out Matrix new_inv_hessian)
        {
            //
            //  Local declarations
            //
            Matrix d;
            Vector p;
            Vector y;
            Vector new_search_dir;
            double sigma;
            double tau;

            //
            //  Will first calculate p
            //  p = current_position - previous_position
            //
            p = current_position;
            p -= previous_position;

            //
            //  Now will evaluate y
            //  y = current_gradient - previous_gradient
            //
            y = current_gradient;
            y -= previous_gradient;

            //
            //  Will now calculate sigma
            //  sigma = p dot y
            //
            sigma = y.DotProduct(p);

            //
            //  Will now calculate tau
            //  tau = y_transpose * prev_inv_hessian * y
            //
            tau = y.DotProduct (prev_inv_hessian * y);

            // Must insure that sigma is not zero becase it is used in the
            // denominator of some calculations
            if(sigma == 0)
            {
                sigma = 1.0e-100;
                //throw MeritFunctionException();
            }

            //
            //  will now calculate the d matrix
            //  d = (sigma + tau)/(sigma^2) * p * p_transpose -
            //      (1/sigma)(h*y*p_transpose + p * (h*y)transpose)
            //
            d = (((sigma + tau) / (Math.Pow(sigma,2)))*p).Multiply(p);
            d -= (prev_inv_hessian * ((1/sigma)*y)).Multiply(p);
            d -= ((1/sigma)*p).Multiply(prev_inv_hessian * y);

            //
            //  Can now calculate the current inv hessian
            //  new_inv_hessian = prev_inv_hessan + d
            //
            new_inv_hessian = prev_inv_hessian;
            new_inv_hessian += d;

            //
            //  Can now calculate the new search direction
            //  search_dir = -1.0*inv_hessian * gradient
            //
            new_search_dir = new_inv_hessian * (-1.0*current_gradient);

            return new_search_dir;
        }
    }
}
