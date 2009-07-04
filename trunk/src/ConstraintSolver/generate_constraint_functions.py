"""This module parses the constraint equations in the file constraint_equations.txt and generates
GiNaC functions in C++ for each constraint equation. The generated C++ files are 
ConstraintFunctions.h and ConstraintFunctions.cpp. The existing C++ files are deleted. """

from sympy import sympify,symbols
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
        parameter_list = rstrip(strip(parameter_list),")")
        self.parameter_list = [strip(parameter) for parameter in split(parameter_list,",")]
        
        self.parameter_dictionary = dict()
        for parameter in self.parameter_list:
            self.parameter_dictionary[parameter] = symbols(parameter,each_char=False)
        
        self.expression = sympify(strip(rhs),locals=self.parameter_dictionary)
        
if __name__ == "__main__":
    input_file = open('constraint_equations.txt')
    
    constraint_equation_list = list()
    
    for line in input_file:
        if "=" in line:
            constraint_equation_list.append(ConstraintEquation(line))
    
    print constraint_equation_list[0].parameter_list
    print [(key,value) for key,value in constraint_equation_list[0].parameter_dictionary.iteritems() if not key.startswith("__")] 
    print constraint_equation_list[0].function_name
    print constraint_equation_list[0].expression
