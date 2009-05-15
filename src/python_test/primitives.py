# doctest examples
"""
>>> point1 = Point2D(1.0,2.0)
>>> point2 = Point2D(2.0,3.0)
>>> point3 = Point2D(4.0,5.0)
>>> point4 = Point2D(point3.x,point3.y)
>>> point3 == point4
True
>>> point3 == point2
False
>>> line1 = Line2D(point1,point2)
>>> line2 = Line2D(point3,point4)
Traceback (most recent call last):
    ...
ValueError: Both ends of the line cannot have identical DOF's
>>> line2 = Line2D(point2,point3)
>>> line3 = Line2D(x1=1.0,y1=2.0,x2=2.0,y2=3.0)
>>> line3 = Line2D(x1=point1.x,y1=point1.y,x2=point2.x,y2=point2.y)
>>> line2.point1 == line3.point2
True
>>> line2.point2 == line3.point2
False
>>> set((line1.x1,line1.y1,line1.x2,line1.y2)) == point1.dof_set
False
>>> set((line1.x1,line1.y1,line1.x2,line1.y2)) == line1.dof_set
True
>>> set((line1.x1,line1.y1,line1.x2,line1.y2)) == line2.dof_set
False
>>> set((point1.x,point1.y)) == point1.dof_set
True
>>> set((point1.x,point1.y)) == point2.dof_set
False
>>> line1.source_primitive_set == set((point1,point2))
True
>>> line1.source_primitive_set == set((point1,point3))
False
>>> print(line1.get_tangent1_expression())[0:2]
((dof0 - dof2)/((dof0 - dof2)**2 + (dof1 - dof3)**2)**(1/2), (dof1 - dof3)/((dof0 - dof2)**2 + (dof1 - dof3)**2)**(1/2))
>>> print(line1.get_tangent2_expression())[0:2]
((dof2 - dof0)/((dof0 - dof2)**2 + (dof1 - dof3)**2)**(1/2), (dof3 - dof1)/((dof0 - dof2)**2 + (dof1 - dof3)**2)**(1/2))
"""

import sympy

class _DOF(object):
    """DOF base class, cannot be used directly. """
    
    __current_id = 0 # Each instance of DOF has a unique ID that is used to 
                     # identify it in expressions and when it is stored in the 
                     # SQLite database
    
    def __init__(self):
        self.id = _DOF.__current_id
        _DOF.__current_id += 1


class _Primitive(object):
    """Primitive base class, cannot be used directly. """
    
    __current_id = 0 # Each instance of _Primitive has a unique ID that is used to 
                            # identify it when it is stored in the SQLite database
    
    def __init__(self):
        self.id = _Primitive.__current_id
        _Primitive.__current_id += 1
        
class IndependentDOF(_DOF):
    """Independent DOF class"""

    def __init__(self,value,free=False):
        _DOF.__init__(self)  # self.id gets defined by the base class
        self.expression = sympy.Symbol("dof"+str(self.id));
        self.value = value
        self.free = free     # If free is True, the constraint solver is able 
                             # to modify this DOF


class DependentDOF(_DOF):
    """A DOF that depends on other DOF's through a sympy expression. """
    
    def __init__(self,expression,dof_list):
        _DOF.__init__(self)  # self.id gets defined by the base class

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
    def _get_x(self):
        return self.__x
    x = property(fget=_get_x)

    def _get_y(self):
        return self.__y
    y= property(fget=_get_y)

    def _get_dof_set(self):
        return set((self.__x,self.__y))
    dof_set = property(fget=_get_dof_set) # all _Primitive children need this property

    def _get_source_primitive_set(self):
        return self.__source_primitive_set.copy()
    source_primitive_set=property(fget=_get_source_primitive_set) # all _Primitive children need this property

    def __eq__(self,other):
        """Two Point2D objects are equal if all of their DOF's are identical"""
        return True if self.__x is other.x and self.__y is other.y else False

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
    def _get_x1(self):
        return self.__x1
    x1 = property(fget=_get_x1)
    
    def _get_x2(self):
        return self.__x2
    x2 = property(fget=_get_x2)
    
    def _get_y1(self):
        return self.__y1
    y1 = property(fget=_get_y1)
    
    def _get_y2(self):
        return self.__y2
    y2 = property(fget=_get_y2)
        
    def _get_point1(self):
        return self.__point1
    point1 = property(fget=_get_point1)
    
    def _get_point2(self):
        return self.__point2
    point2 = property(fget=_get_point2)
        
    def _get_dof_set(self):
        return set((self.__x1,self.__y1,self.__x2,self.__y2))
    dof_set = property(fget=_get_dof_set) # all _Primitive children need this property
    
    def _get_source_primitive_set(self):
        return self.__source_primitive_set.copy()
    source_primitive_set=property(fget=_get_source_primitive_set) # all _Primitive children need this property

if __name__ == "__main__":
    import doctest
    doctest.testmod()
