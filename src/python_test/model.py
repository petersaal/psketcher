#doctest examples
"""
>>> from primitives import *
>>> from model import *
>>> from scipy import pi
>>> from math import fabs

>>> point1 = Point2D(0.0,0.0,False,False)
>>> point2 = Point2D(10.0,0.0,True,False)
>>> point3 = Point2D(10.0,10.0,True,True)

>>> arc1 = Arc2D(1.5,6.0,(pi/2.0)*0.8,pi*1.2,2.0,True,True,True,True,False)

>>> line1 = Line2D(point1,point2)
>>> line2 = Line2D(point2,point3)
>>> line3 = Line2D(point3,arc1.point1)
>>> line4 = Line2D(arc1.point2,point1)

>>> constraint1 = DistancePoint2D(point1,point2,6.0)
>>> constraint2 = DistancePoint2D(point2,point3,12.0)
>>> constraint3 = ParallelLine2D(line1,line3)
>>> constraint4 = ParallelLine2D(line2,line4)
>>> constraint5 = AngleLine2D(line1,line2,pi*0.5,False)
>>> constraint6 = TangentEdge2D(line3,arc1)  #point2,point1
>>> constraint7 = TangentEdge2D(line4,arc1)  #point1,point2

>>> model = Model()
>>> model.add_primitives(point1,point2,point3,arc1,line1,line2,line3,line4)
>>> model.add_constraints(constraint1,constraint2,constraint3,constraint4,constraint5,constraint6,constraint7)

>>> # solve the constraint equations
... model.solve_constraints() # doctest: +ELLIPSIS, +NORMALIZE_WHITESPACE
Optimization terminated successfully. 
    ...

>>> # check that the constraint solver found the correct solution
... epsilon = 1.0e-3

>>> print("point2 location test", fabs(point2.x.value-6.0) < epsilon)
('point2 location test', True)
>>> print("point3 location test", fabs(point3.x.value-6.0) < epsilon and fabs(point3.y.value-12.0) < epsilon)
('point3 location test', True)
>>> print("arc1 center location test", fabs(arc1.center_point.x.value-2.0) < epsilon and fabs(arc1.center_point.y.value-10.0) < epsilon)
('arc1 center location test', True)
>>> print("theta1 and theta2 test", fabs(arc1.theta1.value-pi/2.0) < epsilon and fabs(arc1.theta2.value-pi) < epsilon)
('theta1 and theta2 test', True)
"""

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
        # only proceed if there is at least one constraint equation defined
        if not len(self.__constraint_dict) > 0:
            return
        
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
        
        # Only proceed if there is at least one free parameter to be solved for
        if not len(free_dof_list) > 0:
            return
        
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
        
        # minimize the error function
        solution = fmin_bfgs(error_function, starting_point, fprime=error_gradient_function, \
                             disp=True, callback = None, gtol = 1.0e-10, maxiter=150)

        # update the DOF's with the solution
        for (index,value) in enumerate(solution):
            free_dof_list[index].value = value
            
    @property
    def primitive_dict(self): return self.__primitive_dict
    
    @property
    def constraint_dict(self): return self.__constraint_dict
    
    @property
    def dof_dict(self): return self.__dof_dict

if __name__ == "__main__":
    import doctest
    doctest.testmod()
