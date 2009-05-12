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

    def __init__(self,value):
        self.expression = sympy.Symbol("dof"+str(self.id));
        self.value = value
        
        _DOF.__init__(self)  # self.id gets defined


class DependentDOF(_DOF):
    """A DOF that depends on other DOF's through a sympy expression. """
    pass


class Point2D(_Primitive):
    """Two dimensional point class"""
    
    def __init__(self,x,y):
        self.source_primitives = set() # Set of primitives that this primitive
                                       # depends on, used when deciding which
                                       # primitives to delete.
        
        self.x = x if isinstance(x,_DOF) else IndependentDOF(x)
        self.y = y if isinstance(y,_DOF) else IndependentDOF(y)
        
        _Primitive.__init__(self)  # self.id gets defined
        

class Line2D(_Primitive):
    """Two dimensional line class"""
    
    def __init__(self,x1=None,y1=None,x2=None,y2=None,point1=None,point2=None):
        self.source_primitives = set() # Set of primitives that this primitive
                                       # depends on, used when deciding which
                                       # primitives to delete.
        
        if x1 is not None and y1 is not None:
            self.x1 = x1 if isinstance(_DOF) else IndependentDOF(x1)
            self.y1 = y1 if isinstance(_DOF) else IndependentDOF(y1)
        elif point1 is not None:
            self.x1 = point1.x
            self.y1 = point1.y
            self.source_primitives.add(point1)
        else:
            raise ValueError("Either x1 and y1 or point1 must be supplied.")
        
        if x2 is not None and y2 is not None:
            self.x2 = x2 if isinstance(_DOF) else IndependentDOF(x2)
            self.y2 = y2 if isinstance(_DOF) else IndependentDOF(y2)
        elif point2 is not None:
            self.x2 = point2.x
            self.y2 = point2.y
            self.source_primitives.add(point2)
        else:
            raise ValueError("Either x2 and y2 or point2 must be supplied.")
        
        if self.x1 is self.x2 and self.y1 is self.y2:
            raise ValueError("Both ends of the line cannot have identical DOF's")
        
        _Primitive.__init__(self)  # self.id gets defined