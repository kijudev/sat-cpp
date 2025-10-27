#include "sat.hpp"
#include <iostream>

int main() {
    sat::Expr   a(sat::Op::Or, 0, 1);
    sat::Expr   x(sat::Op::Or, 0, a);
    sat::Values values {true, false};

    std::cout << "x: " << x.eval(values) << "\n";

    return 0;
}
