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
    """Primitive base class, cannot be used directly. 
    
    Child classes need to define self._source_primitive_set and the property dof_set"""
    
    __current_id = 0 # Each instance of _Primitive has a unique ID that is used to 
                     # identify it when it is stored in the SQLite database
    
    def __init__(self):
        self.id = _Primitive.__current_id
        _Primitive.__current_id += 1
        
    def _get_source_primitive_set(self):
        return self._source_primitive_set.copy() # child class needs to supply this attribute
    source_primitive_set=property(fget=_get_source_primitive_set)

        
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
    
    def _get_point1(self):
        return self._point1 # Child class must supply _point1
    
    def _get_point2(self):
        return self._point2 # Child class must supply _point2
    
    point1 = property(fget=_get_point1)
    point2 = property(fget=_get_point2)

class Point2D(_Primitive):
    """Two dimensional point class"""
    
    def __init__(self,x,y,x_free=False,y_free=False):
        # Set of primitives that this primitive depends on, used when deciding which
        # primitives to delete. Expected for all children of _Primitive
        self._source_primitive_set = set() 
        
        self.x = x if isinstance(x,_DOF) else IndependentDOF(x,x_free)
        self.y = y if isinstance(y,_DOF) else IndependentDOF(y,y_free)
        
        _Primitive.__init__(self)  # self.id gets defined
    
    def _get_dof_set(self):
        return set((self.x,self.y))
    dof_set = property(fget=_get_dof_set) # all _Primitive children need this property
    
    def __eq__(self,other):
        """Two Point2D objects are equal if all of their DOF's are identical"""
        return True if self.x is other.x and self.y is other.y else False
            

class Line2D(_Edge):
    """Two dimensional line class"""
    
    def __init__(self,point1=None,point2=None,x1=None,y1=None,x2=None,y2=None,\
                 x1_free=False,y1_free=False,x2_free=False,y2_free=False):
        # Set of primitives that this primitive depends on, used when deciding which
        # primitives to delete. Expected for all children of _Primitive
        self._source_primitive_set = set() 
        
        if x1 is not None and y1 is not None:
            self.x1 = x1 if isinstance(x1,_DOF) else IndependentDOF(x1,x1_free)
            self.y1 = y1 if isinstance(y1,_DOF) else IndependentDOF(y1,y1_free)
        elif point1 is not None:
            self.x1 = point1.x
            self.y1 = point1.y
            self._source_primitive_set.add(point1)
        else:
            raise ValueError("Either x1 and y1 or point1 must be supplied.")
        
        if x2 is not None and y2 is not None:
            self.x2 = x2 if isinstance(x2,_DOF) else IndependentDOF(x2,x2_free)
            self.y2 = y2 if isinstance(y2,_DOF) else IndependentDOF(y2,y2_free)
        elif point2 is not None:
            self.x2 = point2.x
            self.y2 = point2.y
            self._source_primitive_set.add(point2)
        else:
            raise ValueError("Either x2 and y2 or point2 must be supplied.")
        
        if self.x1 is self.x2 and self.y1 is self.y2:
            raise ValueError("Both ends of the line cannot have identical DOF's")
        
        _Primitive.__init__(self)  # self.id gets defined by the base class
        
        # Create the point attributes that are expected for all classes that
        # inherit from _Edge
        self._point1 = Point2D(self.x1,self.y1)
        self._point2 = Point2D(self.x2,self.y2)
        
    def _get_dof_set(self):
        return set((self.x1,self.y1,self.x2,self.y2))
    dof_set = property(fget=_get_dof_set) # all _Primitive children need this property