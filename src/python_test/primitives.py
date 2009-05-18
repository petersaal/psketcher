# doctest examples
"""
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
"""

import sympy
from scipy import pi

class _DOF(object):
    """DOF base class, cannot be used directly. """
    
    __current_id = 0 # Each instance of DOF has a unique ID that is used to 
                     # identify it in expressions and when it is stored in the 
                     # SQLite database
    
    def __init__(self):
        self.__id = _DOF.__current_id
        _DOF.__current_id += 1
        
    @property
    def id(self): return self.__id


class _Primitive(object):
    """Primitive base class, cannot be used directly. """
    
    __current_id = 0 # Each instance of _Primitive has a unique ID that is used to 
                            # identify it when it is stored in the SQLite database
    
    def __init__(self):
        self.__id = _Primitive.__current_id
        _Primitive.__current_id += 1
        
    @property
    def id(self): return self.__id
        
class IndependentDOF(_DOF):
    """Independent DOF class"""

    def __init__(self,value,free=False):
        _DOF.__init__(self)  # self.id gets defined by the base class
        self.__variable = sympy.Symbol("dof"+str(self.id));
        self.value = value
        self.free = free     # If free is True, the constraint solver is able 
                             # to modify this DOF
    @property 
    def variable(self): return self.__variable

    @property
    def expression(self): return self.__variable

    @property
    def dependent(self): return False


class DependentDOF(_DOF):
    """A DOF that depends on other DOF's through a sympy expression. """
    
    def __init__(self,expression,dof_list):
        _DOF.__init__(self)  # self.id gets defined by the base class
        self.__variable = sympy.Symbol("dof"+str(self.id));
        self.__dof_list = list(set(dof_list)) # Remove any duplicates
        self.__expression = expression
        # Create a python function to speed up evaluation of the expression
        # Because this function relies on the order of __dof_list, a set cannot be used for __dof_list
        self.__value_function = sympy.lambdify([dof.variable for dof in self.__dof_list], self.__expression)
        
    @property
    def value(self):
        return self.__value_function(*[dof.value for dof in self.__dof_list])

    @property 
    def variable(self): return self.__variable

    @property
    def expression(self):
        expression = self.__expression
        expression = expression.subs( \
                     [(dof.variable, dof.expression) for dof in self.__dof_list if dof.dependent])
        return expression

    @property
    def dof_list(self): return self.__dof_list

    @property
    def free(self): return False
        
    @property
    def dependent(self): return True


class _Edge(_Primitive):
    """ Edge base class used to group edge primitives (Line2D, Arc2D, etc.). This
    class cannot be used on its own."""
    pass


class Point2D(_Primitive):
    """Two dimensional point class"""
    
    def __init__(self,x,y,x_free=False,y_free=False):
        self.__source_primitive_set = set()  # Expected for all children of _Primitive
        
        self.__x = x if isinstance(x,_DOF) else IndependentDOF(x,x_free)
        self.__y = y if isinstance(y,_DOF) else IndependentDOF(y,y_free)
        
        _Primitive.__init__(self)  # self.id gets defined
    
    # __x and __y are read only
    @property
    def x(self): return self.__x

    @property
    def y(self): return self.__y

    @property
    def dof_set(self): return set((self.__x,self.__y))

    @property
    def source_primitive_set(self): return self.__source_primitive_set

    def __eq__(self,other):
        """Two Point2D objects are equal if all of their DOF's are identical"""
        return True if self.__x is other.__x and self.__y is other.__y else False


class Line2D(_Edge):
    """Two dimensional line class"""
    
    def __init__(self,point1=None,point2=None,x1=None,y1=None,x2=None,y2=None,\
                 x1_free=False,y1_free=False,x2_free=False,y2_free=False):
        
        self.__source_primitive_set = set()  # Expected for all children of _Primitive
        
        if x1 is not None and y1 is not None:
            self.__x1 = x1 if isinstance(x1,_DOF) else IndependentDOF(x1,x1_free)
            self.__y1 = y1 if isinstance(y1,_DOF) else IndependentDOF(y1,y1_free)
        elif point1 is not None:
            self.__x1 = point1.x
            self.__y1 = point1.y
            self.__source_primitive_set.add(point1)
        else:
            raise ValueError("Either x1 and y1 or point1 must be supplied.")
        
        if x2 is not None and y2 is not None:
            self.__x2 = x2 if isinstance(x2,_DOF) else IndependentDOF(x2,x2_free)
            self.__y2 = y2 if isinstance(y2,_DOF) else IndependentDOF(y2,y2_free)
        elif point2 is not None:
            self.__x2 = point2.x
            self.__y2 = point2.y
            self.__source_primitive_set.add(point2)
        else:
            raise ValueError("Either x2 and y2 or point2 must be supplied.")
        
        if self.__x1 is self.__x2 and self.__y1 is self.__y2:
            raise ValueError("Both ends of the line cannot have identical DOF's")
        
        _Primitive.__init__(self)  # self.id gets defined by the base class
        
        # Create the point attributes that are expected for all classes that
        # inherit from _Edge
        self.__point1 = Point2D(self.__x1,self.__y1)
        self.__point2 = Point2D(self.__x2,self.__y2)

    def get_tangent1_expression(self):
        length = sympy.sqrt((self.__x1.expression-self.__x2.expression)**2+\
                            (self.__y1.expression-self.__y2.expression)**2)
        x_component = (self.__x1.expression-self.__x2.expression)/length
        y_component = (self.__y1.expression-self.__y2.expression)/length
        dof_set = set((self.__x1, self.__x2, self.__y1, self.__y2))
        return (x_component, y_component, dof_set)
        
    def get_tangent2_expression(self):
        length = sympy.sqrt((self.__x1.expression-self.__x2.expression)**2+\
                            (self.__y1.expression-self.__y2.expression)**2)
        x_component = (self.__x2.expression-self.__x1.expression)/length
        y_component = (self.__y2.expression-self.__y1.expression)/length
        dof_set = set((self.__x1, self.__x2, self.__y1, self.__y2))
        return (x_component, y_component, dof_set)

    # x1, x2, y1, and y2 are all read only so define property attributes
    # they are read only since __point1, __point2, and __source_primitve_set all 
    # depend on their particular instance
    @property
    def x1(self): return self.__x1

    @property
    def x2(self): return self.__x2

    @property
    def y1(self): return self.__y1
    
    @property
    def y2(self): return self.__y2

    @property
    def point1(self): return self.__point1
    
    @property
    def point2(self): return self.__point2
    
    @property
    def dof_set(self): return set((self.__x1,self.__y1,self.__x2,self.__y2))
    
    @property
    def source_primitive_set(self): return self.__source_primitive_set


class Arc2D(_Edge):
    """Two dimensional arc class"""
    
    def __init__(self, x_center, y_center, theta1, theta2, radius, \
                 x_center_free=False, y_center_free=False, theta1_free=False, \
                 theta2_free=False, radius_free=False):
        self.__source_primitive_set = set()  # Expected for all children of _Primitive
        
        self.__x_center = x_center if isinstance(x_center,_DOF) else IndependentDOF(x_center,x_center_free)
        self.__y_center = y_center if isinstance(y_center,_DOF) else IndependentDOF(y_center,y_center_free)
        self.__theta1 = theta1 if isinstance(theta1,_DOF) else IndependentDOF(theta1,theta1_free)
        self.__theta2 = theta2 if isinstance(theta2,_DOF) else IndependentDOF(theta2,theta2_free)
        self.__radius = radius if isinstance(radius,_DOF) else IndependentDOF(radius,radius_free)

        _Primitive.__init__(self)  # self.id gets defined by the base class
        
        # Create the point attributes that are expected for all classes that
        # inherit from _Edge
        self.__point1 = self.__generate_point1()
        self.__point2 = self.__generate_point2()
        self.__center_point = Point2D(self.__x_center,  self.__y_center)

    def __generate_point1(self):
        x = self.__x_center.variable + self.__radius.variable*sympy.cos(self.__theta1.variable)
        y = self.__y_center.variable + self.__radius.variable*sympy.sin(self.__theta1.variable)
        
        x_dof = DependentDOF(x, set( (self.__x_center,self.__radius,self.__theta1) ))
        y_dof = DependentDOF(y, set( (self.__y_center,self.__radius,self.__theta1) ))
        
        return Point2D(x_dof, y_dof)
        
    def __generate_point2(self):
        x = self.__x_center.variable + self.__radius.variable*sympy.cos(self.__theta2.variable)
        y = self.__y_center.variable + self.__radius.variable*sympy.sin(self.__theta2.variable)
        
        x_dof = DependentDOF(x, set( (self.__x_center,self.__radius,self.__theta2) ))
        y_dof = DependentDOF(y, set( (self.__y_center,self.__radius,self.__theta2) ))
        
        return Point2D(x_dof, y_dof)

    def get_tangent1_expression(self):
        x_component = sympy.sin(self.__theta1.variable);
        y_component = -sympy.cos(self.__theta1.variable);
        
        return (x_component, y_component, set((self.__theta1,)))
        
    def get_tangent2_expression(self):
        x_component = -sympy.sin(self.__theta2.variable);
        y_component = sympy.cos(self.__theta2.variable);

        return (x_component, y_component, set((self.__theta2,)))
        
    @property
    def x_center(self): return self.__x_center

    @property
    def y_center(self): return self.__y_center

    @property
    def theta1(self): return self.__theta1
    
    @property
    def theta2(self): return self.__theta2

    @property
    def point1(self): return self.__point1
    
    @property
    def point2(self): return self.__point2

    @property
    def center_point(self): return self.__center_point

    @property
    def dof_set(self): return set((self.__x_center,self.__y_center,self.__theta1, \
                                   self.__theta2,self.__radius))
    @property
    def source_primitive_set(self): return self.__source_primitive_set


class _Constraint(object):
    """Constraint base class, cannot be used directly. """
    
    __current_id = 0 # Each instance of _Constraint has a unique ID that is used to 
                     # identify it when it is stored in the SQLite database
    
    def __init__(self):
        self.__id = _Constraint.__current_id
        _Constraint.__current_id += 1
        
    @property
    def id(self): return self.__id

class DistancePoint2D(_Constraint):
    """Generates distance constraint between two Point2D objects"""
    
    def __init__(self, point1, point2, distance):
        _Constraint.__init__(self)  # self.id gets defined by the base class
        self.__point1 = point1
        self.__point2 = point2
        self.__distance = distance if isinstance(distance,_DOF) else IndependentDOF(distance,free=False)
        
        # Store a set of dof's and a set of primitives that this constraint depends on
        self.__source_primitive_set = set((point1,point2))  # Expected for all children of _Constraint
        self.__dof_set = set((self.__distance, point1.x, point1.y, point2.x, point2.y))
        
        # create the constraint equation expression
        constraint_equation = sympy.sqrt((point1.x.variable-point2.x.variable)**2+ \
                                         (point1.y.variable-point2.y.variable)) - \
                              self.__distance.variable
    
        # Create the constraint equation list, each element of the list is a tuple of a 
        # constraint expression and a constraint weight. 
        self.__constraint_equation_list = list()
        self.__constraint_equation_list.append( (constraint_equation,1.0) )

    @property
    def constraint_equation_list(self): return self.__constraint_equation_list

    @property
    def dof_set(self): return self.__dof_set
    
    @property
    def source_primitive_set(self): return self.__source_primitive_set
    
class ParallelLine2D(_Constraint):
    """Generates parallelism constraint between two Line2D objects"""
    
    def __init__(self, line1, line2):
        _Constraint.__init__(self)  # self.id gets defined by the base class
        self.__line1 = line1
        self.__line2 = line2
        
        # Store a set of dof's and a set of primitives that this constraint depends on
        self.__source_primitive_set = set((line1,line2))  # Expected for all children of _Constraint
        self.__dof_set = set((line1.x1, line1.y1, line1.x2, line1.y2, \
                              line2.x1, line2.y1, line2.x2, line2.y2))
        
        # create the constraint equation expression
        line1_dx = line1.x1.variable - line1.x2.variable;
        line1_dy = line1.y1.variable - line1.y2.variable;
        line1_length = sympy.sqrt(line1_dx**2+line1_dy**2);

        line2_dx = line2.x1.variable - line2.x2.variable;
        line2_dy = line2.y1.variable - line2.y2.variable;
        line2_length = sympy.sqrt(line2_dx**2+line2_dy**2);

        # Calculate the dot product normalized by the vector lengths and subtract one.
        # This expression will be zero when the lines are parallel
        # Ideally, I would use abs() instead of ()**2 but abs is not differentiable. 
        constraint_equation = ((1/(line1_length*line2_length))*(line1_dx*line2_dx + line1_dy*line2_dy)**2)-1

        # Create the constraint equation list, each element of the list is a tuple of a 
        # constraint expression and a constraint weight. 
        self.__constraint_equation_list = list()
        self.__constraint_equation_list.append( (constraint_equation,1.0) )

    @property
    def constraint_equation_list(self): return self.__constraint_equation_list

    @property
    def dof_set(self): return self.__dof_set
    
    @property
    def source_primitive_set(self): return self.__source_primitive_set
    
class AngleLine2D(_Constraint):
    """Generates angle constraint between two Line2D objects"""
    
    def __init__(self, line1, line2, angle, interior_angle):
        _Constraint.__init__(self)  # self.id gets defined by the base class
        self.__line1 = line1
        self.__line2 = line2
        self.__angle = angle if isinstance(angle,_DOF) else IndependentDOF(angle,free=False)
        self.__interior_angle = interior_angle
        
        # Store a set of dof's and a set of primitives that this constraint depends on
        self.__source_primitive_set = set((line1,line2))  # Expected for all children of _Constraint
        self.__dof_set = set((line1.x1, line1.y1, line1.x2, line1.y2, \
                              line2.x1, line2.y1, line2.x2, line2.y2, self.__angle))
        
        # create the constraint equation expression
        line1_dx = line1.x1.variable - line1.x2.variable;
        line1_dy = line1.y1.variable - line1.y2.variable;
        line1_length = sympy.sqrt(line1_dx**2+line1_dy**2);

        line2_dx = line2.x1.variable - line2.x2.variable;
        line2_dy = line2.y1.variable - line2.y2.variable;
        line2_length = sympy.sqrt(line2_dx**2+line2_dy**2);

        # Calculate the dot product normalized by the vector lengths and subtract the cos of the desired angle
        # this expression will be zero when the lines are at the desired angle
        if self.__interior_angle:
            constraint_equation = (1/(line1_length*line2_length))* \
                                  (line1_dx*line2_dx + line1_dy*line2_dy)-sympy.cos(self.__angle.variable)
        else:
            constraint_equation = (1/(line1_length*line2_length))* \
                                  (line1_dx*line2_dx + line1_dy*line2_dy)-sympy.cos(pi-self.__angle.variable)

        # Create the constraint equation list, each element of the list is a tuple of a 
        # constraint expression and a constraint weight. 
        self.__constraint_equation_list = list()
        self.__constraint_equation_list.append( (constraint_equation,1.0) )

    @property
    def constraint_equation_list(self): return self.__constraint_equation_list

    @property
    def dof_set(self): return self.__dof_set
    
    @property
    def source_primitive_set(self): return self.__source_primitive_set
    
    @property
    def interior_angle(self): return self.__interior_angle
    
    @property
    def angle(self): return self.__angle
    
class TangentEdge2D(_Constraint):
    """Generates a tangent constraint between two _Edge objects"""
    
    def __init__(self, edge1, edge2, edge1_point_num=None, edge2_point_num=None):
        _Constraint.__init__(self)  # self.id gets defined by the base class
        self.__edge1 = edge1
        self.__edge2 = edge2
        
        # Store a set of primitives that this constraint depends on
        self.__source_primitive_set = set((edge1,edge2))  # Expected for all children of _Constraint
        
        if edge1_point_num == None or edge2_point_num == None:
            if edge1.point1 == edge2.point1:
                edge1_point_num = 1
                edge2_point_num = 1
            elif edge1.point1 == edge2.point2:
                edge1_point_num = 1
                edge2_point_num = 2
            elif edge1.point2 == edge2.point1:
                edge1_point_num = 2
                edge2_point_num = 1
            elif edge1.point2 == edge2.point2:
                edge1_point_num = 1
                edge2_point_num = 1
            else:
                raise ValueError("If the edges do not share a common point both edge1_point_num and \
                                 edge2_point_num must be supplied")
        
        if edge1_point_num == 1:
            (x1,y1,dof_set1) = edge1.get_tangent1_expression()
        elif edge1_point_num == 2:
            (x1,y1,dof_set1) = edge1.get_tangent2_expression()
        else:
            raise ValueError("edge1_point_num input parameter must be either 1 or 2")
            
        if edge2_point_num == 1:
            (x2,y2,dof_set2) = edge2.get_tangent1_expression()
        elif edge2_point_num == 2:
            (x2,y2,dof_set2) = edge2.get_tangent2_expression()
        else:
            raise ValueError("edge2_point_num input parameter must be either 1 or 2")
        
        # Store a set of DOF's that this constraint depends on
        self.__dof_set = set()
        self.__dof_set.update(dof_set1,dof_set2)
        
        # Calculate the dot product between the two tangent vectors and subtract one.
        # This expression will be zero when the lines are parallel.
        # Ideally, I would use abs() instead of ()**2 but abs is not differentiable. 
        constraint_equation = (x1*x2-y1*y2)**2-1

        # Create the constraint equation list, each element of the list is a tuple of a 
        # constraint expression and a constraint weight. 
        self.__constraint_equation_list = list()
        self.__constraint_equation_list.append( (constraint_equation,1.0) )

    @property
    def constraint_equation_list(self): return self.__constraint_equation_list

    @property
    def dof_set(self): return self.__dof_set
    
    @property
    def source_primitive_set(self): return self.__source_primitive_set
    
if __name__ == "__main__":
    import doctest
    doctest.testmod()
