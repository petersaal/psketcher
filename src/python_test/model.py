import sympy
from scipy.optimize import fmin_bfgs
from numpy import array

class Model(object):
    """The Model class maintains a list of primitives and constraints and provides
    methods to solve the constraint equations."""
    
    def __init__(self):
        self.__dof_dict = dict()
        self.__constraint_dict = dict()
        self.__primitive_dict = dict()
        
    def add_constraints(self,*constraints):
        for constraint in constraints:
            # add each primitive to the dictionary
            self.__constraint_dict[constraint.id] = constraint
            
            # add all of the primitives that this constraint depends on to the primitive dict
            for primitive in constraint.source_primitive_set:
                self.__primitive_dict[primitive.id] = primitive
                
            # add all of the DOF's that this constraint depends on to the DOF dict
            for dof in constraint.dof_set:
                self.__dof_dict[dof.id] = dof
        
    def add_primitives(self,*primitives):
        for primitive in primitives:
            # add each primitive to the dictionary
            self.__primitive_dict[primitive.id] = primitive
            
            # add all of the primitives that this primitive depends on to the primitive dict
            for primitive in primitive.source_primitive_set:
                self.__primitive_dict[primitive.id] = primitive
                
            # add all of the DOF's that this primitive depends on to the DOF dict
            for dof in primitive.dof_set:
                self.__dof_dict[dof.id] = dof
        
    def solve_constraints(self):
        # Define error function
        error_expression = None
        for constraint in self.__constraint_dict.itervalues():
            for (constraint_expression, weight) in constraint.constraint_equation_list:
                if error_expression is not None:
                    error_expression += weight*constraint_expression*constraint_expression
                else:
                    error_expression = weight*constraint_expression*constraint_expression
                
        # for each dependent dof, substitute its expression into the error expression
        for dof in (dof for dof in self.__dof_dict.itervalues() if dof.dependent):
            error_expression = error_expression.subs(dof.variable,dof.expression)
        
        # for each fixed dof (free=False), substitute its value into the error expression
        for dof in (dof for dof in self.__dof_dict.itervalues() if not dof.free):
            error_expression = error_expression.subs(dof.variable,dof.value)
        
        # create a list of the free parameters 
        # duplicates are removed by converting to a set and than back to a list
        free_dof_list = list(set([dof for dof in self.__dof_dict.itervalues() if dof.free]))
        
        # create the functions for the error expression value and its gradient
        temp_error_function = sympy.lambdify([dof.variable for dof in free_dof_list], error_expression)
        error_function = lambda x: array(temp_error_function(*x)) # allows the error_function to accept a list as input
        
        error_gradient = list()
        for dof in free_dof_list:
            # calculate the partial derivatives for the gradient
            error_gradient.append(sympy.diff(error_expression,dof.variable))
        
        temp_error_gradient_function = sympy.lambdify([dof.variable for dof in free_dof_list],error_gradient)
        error_gradient_function = lambda x: array(temp_error_gradient_function(*x))
        
        # create list of starting point values for the optimization
        starting_point = array([dof.value for dof in free_dof_list])
        
        print starting_point
        print temp_error_function(*starting_point)
        print temp_error_gradient_function(*starting_point)
        
        def my_callback(x):
            print(x)
        
        # minimize the error function
        solution = fmin_bfgs(error_function, starting_point, fprime=error_gradient_function, \
                             disp=True, callback = my_callback)

        # update the DOF's with the solution
        for (index,value) in enumerate(solution):
            free_dof_list[index].value = value
