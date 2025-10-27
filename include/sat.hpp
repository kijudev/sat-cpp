#include <concepts>
#include <cstdint>
#include <limits>
#include <memory>
#include <utility>
#include <variant>
#include <vector>

namespace sat {
enum class Op {
    Impl,
    Eq,
    Not,
    And,
    Nand,
    Or,
    Xor,
    Nor,
};

using Symbol = uint8_t;
using Values = std::vector<bool>;

class Expr {
private:
    std::variant<Symbol, std::unique_ptr<Expr>> m_p;
    std::variant<Symbol, std::unique_ptr<Expr>> m_q;
    Op                                          m_op;

    // TODO
    // Make it lazy
    static bool eval_op(Op op, bool p, bool q) {
        switch (op) {
        case Op::Impl: return (!p) && q;
        case Op::Eq: return p == q;
        case Op::Not: return !p;
        case Op::And: return p && q;
        case Op::Or: return p || q;
        case Op::Nand: return (!p) || (!q);
        case Op::Xor: return ((!p) && q) || (p && (!q));
        case Op::Nor: return (!p) && (!q);
        }
    }

    static bool eval_part(
        const Values&                                      values,
        const std::variant<Symbol, std::unique_ptr<Expr>>& symbol) {

        if (symbol.index() == 0) {
            return values[std::get<Symbol>(symbol)];
        } else {
            return std::get<std::unique_ptr<Expr>>(symbol).get()->eval(values);
        }
    }

public:
    Expr() = delete;

    Expr(Op op, Symbol p, Symbol q)
        : m_p(p)
        , m_q(q)
        , m_op(op) { }

    Expr(Op op, Expr p, Symbol q)
        : m_p(std::make_unique<Expr>(std::move(p)))
        , m_q(q)
        , m_op(op) { }

    Expr(Op op, Symbol p, Expr q)
        : m_p(p)
        , m_q(std::make_unique<Expr>(std::move(q)))
        , m_op(op) { }

    Expr(Op op, Expr p, Expr q)
        : m_p(std::make_unique<Expr>(std::move(p)))
        , m_q(std::make_unique<Expr>(std::move(q)))
        , m_op(op) { }

    // TODO
    // Throw if op is not `Not`
    Expr(Op op, Symbol p)
        : m_p(p)
        , m_q(std::numeric_limits<Symbol>::max())
        , m_op(op) { }

    // TODO
    // Throw if op is not `Not`
    Expr(Op op, Expr p)
        : m_p(std::make_unique<Expr>(std::move(p)))
        , m_q(std::numeric_limits<Symbol>::max())
        , m_op(op) { }

    Expr(const Expr& other) {
        if (other.m_p.index() == 1) {
            m_p = std::variant<Symbol, std::unique_ptr<Expr>>(
                std::make_unique<Expr>(
                    *std::get<std::unique_ptr<Expr>>(other.m_p)));
        } else {
            m_p = std::variant<Symbol, std::unique_ptr<Expr>>(
                std::get<Symbol>(other.m_p));
        }

        if (other.m_q.index() == 1) {
            m_p = std::variant<Symbol, std::unique_ptr<Expr>>(
                std::make_unique<Expr>(
                    *std::get<std::unique_ptr<Expr>>(other.m_q)));
        } else {
            m_p = std::variant<Symbol, std::unique_ptr<Expr>>(
                std::get<Symbol>(other.m_q));
        }

        m_op = other.m_op;
    }

    bool eval(const Values& values) {
        return eval_op(m_op, eval_part(values, m_p), eval_part(values, m_q));
    }

    // template <typename P, typename Q>
    //     requires(std::same_as<P, Symbol> || std::same_as<P, Expr &&>)
    //             && (std::same_as<Q, Symbol> || std::same_as<Q, Expr &&>)
    // static Expr Impl() {
    // }
};

};
