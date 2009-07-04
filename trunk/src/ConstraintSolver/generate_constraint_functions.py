"""This module parses the constraint equations in the file constraint_equations.txt and generates
GiNaC functions in C++ for each constraint equation. The generated C++ files are 
ConstraintFunctions.h and ConstraintFunctions.cpp. The existing C++ files are deleted. """

from sympy import sympify
from string import split,strip,rstrip

class ConstraintEquation:
    def __init__(self,line):
        self.parse_equation(line)
    
    def parse_equation(self,line):
        """Takes one line from the constraint_equations.txt file and parses it. Defines the
        following parameters (function_name, paramater_list, parameter_dictionary, expression). """
        
        (lhs,rhs) = split(line,"=")
        (function_name,parameter_list) = split(lhs,"(")
        self.function_name = strip(function_name)
        parameter_list = rstrip(parameter_list,")")
        self.parameter_list = [strip(parameter) for parameter in split(parameter_list,",")]
        
        self.parameter_dictionary = dict()
        
        self.expression = sympify(strip(rhs),locals=self.parameter_dictionary)
        
    def generate_header_lines(self):
        lines = list()
        
        #add declaration for the constraint equation
        lines.append("DECLARE_FUNCTION_"+len(self.parameter_list)+"P("+self.function_name+")")
        
        #add the declarations for the partials of the constraint equation
        for index,parameter in enumerate(self.parameter_list):
            lines.append("DECLARE_FUNCTION_"+len(self.parameter_list)+"P("+self.function_name+"_partial"+index+")")
        
        return lines
        
    def generate_source_lines(self):
        pass

