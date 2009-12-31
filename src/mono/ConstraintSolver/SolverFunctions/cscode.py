"""
C# code printer (based on ccode.py)

The CSCodePrinter converts single sympy expressions into single C# expressions,
using the functions defined in System.Math where possible.
"""

from sympy.printing.str import StrPrinter
from sympy.printing.precedence import precedence, PRECEDENCE
from sympy.core.basic import S

class CSCodePrinter(StrPrinter):
    """A printer to convert python expressions to strings of c code"""
    printmethod = "_cscode_"

    def _print_Pow(self, expr):
        PREC = precedence(expr)
        if expr.exp is S.NegativeOne:
            return '1.0/%s'%(self.parenthesize(expr.base, PREC))
        else:
            return 'Math.Pow(%s,%s)'%(self.parenthesize(expr.base, PREC),
                                 self.parenthesize(expr.exp, PREC))

    def _print_Rational(self, expr):
        p, q = int(expr.p), int(expr.q)
        return '%d.0/%d.0' % (p, q)

    def _print_Exp1(self, expr):
        return "Math.E"

    def _print_Pi(self, expr):
        return 'Math.PI'

    def _print_Infinity(self, expr):
        return 'double.PositiveInfinity'

    def _print_NegativeInfinity(self, expr):
        return 'double.NegativeInfinity'

    def _print_Piecewise(self, expr):
        ecpairs = ["(%s) {\n%s\n}\n" % (self._print(c), self._print(e)) \
                       for e, c in expr.args[:-1]]
        last_line = ""
        if expr.args[-1].cond == True:
            last_line = "else {\n%s\n}" % self._print(expr.args[-1].expr)
        else:
            ecpairs.append("(%s) {\n%s\n" % \
                           (self._print(expr.args[-1].cond),
                            self._print(expr.args[-1].expr)))
        code = "if %s" + last_line
        return code % "else if ".join(ecpairs)

    def _print_Function(self, expr):
        return 'Math.'+expr.func.__name__[0].upper()+expr.func.__name__[1:]+'(%s)' % self.stringify(expr.args, ", ")
        # return StrPrinter._print_Function(self, expr)

def cscode(expr):
    r"""Converts an expr to a string of c code

        Works for simple expressions using math.h functions.

        >>> from sympy import *
        >>> from sympy.abc import *

        >>> cscode((2*tau)**Rational(7,2))
        '8*pow(2,(1.0/2.0))*pow(tau,(7.0/2.0))'
    """
    return CSCodePrinter().doprint(expr)

def print_cscode(expr):
    """Prints C representation of the given expression."""
    print cscode(expr)
