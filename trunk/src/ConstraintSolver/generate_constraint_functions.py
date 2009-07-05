"""This module parses the constraint equations in the file constraint_equations.txt and generates
GiNaC functions in C++ for each constraint equation. The generated C++ files are 
ConstraintFunctions.h and ConstraintFunctions.cpp. The existing C++ files are deleted. """

from sympy import sympify,symbols,ccode
from string import split,strip,rstrip
from mako.template import Template

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

    header_template = Template(filename='ConstraintEquations.h.template')
    header_file = open('ConstraintEquations.h','w')
    header_file.writelines(header_template.render(equations=constraint_equation_list))
    header_file.close()

    source_template = Template(filename='ConstraintEquations.cpp.template')
    source_file = open('ConstraintEquations.cpp','w')
    source_file.writelines(source_template.render(equations=constraint_equation_list))
    source_file.close()
