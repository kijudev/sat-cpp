#include <iostream>

#include "sat.hpp"

int main() {
    sat::Symbol p = 0;
    sat::Symbol q = 1;
    sat::Symbol r = 2;
    sat::Values values{true, false, true};

    sat::Expr x = sat::Expr::And(p, q);
    sat::Expr y = sat::Expr(sat::Op::And, p, sat::Expr(sat::Op::And, q, r));

    std::cout << "y: " << y.eval(values) << "\n";

    return 0;
}
