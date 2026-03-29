#ifndef polynomial_H_
#define polynomial_H_

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include "polynomial_utils.hpp"
#include "term_order.hpp"

template<typename T>
class polynomial
{
private:
    struct Node
    {
        T coeff;
        std::map<int, Node*> children;
        
        Node()
            : coeff(T{})
        {
        }
        
        Node(const T& c)
            : coeff(c)
        {
        }
        
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
    };
    
    Node* root;
    std::vector<std::string> vars;
    term_order t_order;
    
    Node* clone(Node* node) const
    {
        if (!node)
        {
            return nullptr;
        }
        
        Node* n = new Node(node->coeff);
        
        for (auto& kv : node->children)
        {
            n->children[kv.first] = clone(kv.second);
        }
        
        return n;
    }
    
    void clear(Node* node)
    {
        if (!node)
        {
            return;
        }
        
        for (auto& kv : node->children)
        {
            clear(kv.second);
        }
        
        delete node;
    }
    
    void insert(Node* node, const std::vector<int>& exps, size_t depth, const T& value)
    {
        if (value == T{})
        {
            return;
        }
        
        if (depth == exps.size())
        {
            node->coeff = node->coeff + value;
            return;
        }
        
        int e = exps[depth];
        
        if (node->children.find(e) == node->children.end())
        {
            node->children[e] = new Node();
        }
        
        insert(node->children[e], exps, depth + 1, value);
    }
    
    void addNodes(Node* a, const Node* b)
    {
        if (!b)
        {
            return;
        }

        a->coeff = a->coeff + b->coeff;

        for (auto& kv : b->children)
        {
            auto it = a->children.find(kv.first);
            if (it == a->children.end())
            {
                a->children[kv.first] = clone(kv.second);
            }
            else
            {
                addNodes(it->second, kv.second);
                if (it->second->coeff == T{} && it->second->children.empty())
                {
                    delete it->second;
                    a->children.erase(it);
                }
            }
        }
    }

    void subNodes(Node* a, const Node* b)
    {
        if (!b)
        {
            return;
        }

        a->coeff = a->coeff - b->coeff;

        for (auto& kv : b->children)
        {
            auto it = a->children.find(kv.first);
            if (it == a->children.end())
            {
                Node* tmp = clone(kv.second);
                negate(tmp);
                a->children[kv.first] = tmp;
            }
            else
            {
                subNodes(it->second, kv.second);
                if (it->second->coeff == T{} && it->second->children.empty())
                {
                    delete it->second;
                    a->children.erase(it);
                }
            }
        }
    }

    void scale(Node* node, const T& scalar)
    {
        if (!node)
        {
            return;
        }
        
        node->coeff = node->coeff * scalar;
        
        for (auto& kv : node->children)
        {
            scale(kv.second, scalar);
        }
    }
    
    void negate(Node* node)
    {
        if (!node)
        {
            return;
        }
        
        node->coeff = -node->coeff;
        
        for (auto& kv : node->children)
        {
            negate(kv.second);
        }
    }
    
    void collect(const Node* node, std::vector<int>& cur, std::vector<std::pair<std::vector<int>, T>>& res) const
    {
        if (!node)
        {
            return;
        }
        
        if (node->coeff != T{})
        {
            res.push_back({cur, node->coeff});
        }
        
        for (auto& kv : node->children)
        {
            cur.push_back(kv.first);
            collect(kv.second, cur, res);
            cur.pop_back();
        }
    }
    
    T power(const T& base, int exp) const
    {
        if (exp == 0)
        {
            return T{1};
        }
        
        T result = base;
        for (int i = 1; i < exp; ++i)
        {
            result = result * base;
        }
        return result;
    }
    
    T evaluateNode(const Node* node, const std::vector<T>& point, size_t depth, const T& accum) const
    {
        T result = T{};
        
        if (node->coeff != T{})
        {
            result = result + node->coeff * accum;
        }
        
        if (depth < point.size())
        {
            for (auto& kv : node->children)
            {
                T powVal = power(point[depth], kv.first);
                T val = accum * powVal;
                result = result + evaluateNode(kv.second, point, depth + 1, val);
            }
        }
        
        return result;
    }
    
    bool isHomogeneousNode(const Node* node, int currentSum, int& refSum) const
    {
        if (!node)
        {
            return true;
        }
        
        if (node->coeff != T{})
        {
            if (refSum == -1)
            {
                refSum = currentSum;
            }
            else if (refSum != currentSum)
            {
                return false;
            }
        }
        
        for (auto& kv : node->children)
        {
            if (!isHomogeneousNode(kv.second, currentSum + kv.first, refSum))
            {
                return false;
            }
        }
        
        return true;
    }
    
    void extractHomogeneous(const Node* node, Node* out, int currentSum, int targetDegree) const
    {
        if (!node)
        {
            return;
        }
        
        if (node->coeff != T{} && currentSum == targetDegree)
        {
            out->coeff = out->coeff + node->coeff;
        }
        
        for (auto& kv : node->children)
        {
            if (currentSum + kv.first <= targetDegree)
            {
                if (out->children.find(kv.first) == out->children.end())
                {
                    out->children[kv.first] = new Node();
                }
                extractHomogeneous(kv.second, out->children[kv.first], currentSum + kv.first, targetDegree);
            }
        }
    }
    
    bool equalNodes(const Node* a, const Node* b) const
    {
        if (!a && !b)
        {
            return true;
        }
        
        if (!a || !b)
        {
            return false;
        }
        
        if (a->coeff != b->coeff)
        {
            return false;
        }
        
        if (a->children.size() != b->children.size())
        {
            return false;
        }
        
        for (auto& kv : a->children)
        {
            auto it = b->children.find(kv.first);
            if (it == b->children.end())
            {
                return false;
            }
            
            if (!equalNodes(kv.second, it->second))
            {
                return false;
            }
        }
        
        return true;
    }
    
    bool cleanNode(Node* node)
    {
        if (!node)
        {
            return true;
        }
        
        std::vector<int> toErase;
        for (auto& kv : node->children)
        {
            if (cleanNode(kv.second))
            {
                delete kv.second;
                toErase.push_back(kv.first);
            }
        }
        
        for (int key : toErase)
        {
            node->children.erase(key);
        }
        
        return (node->coeff == T{}) && node->children.empty();
    }
    
    std::vector<std::pair<std::vector<int>, T>> getSortedTerms() const
    {
        std::vector<std::pair<std::vector<int>, T>> terms = supp();
        std::sort(terms.begin(), terms.end(),
            [this](const std::pair<std::vector<int>, T>& a, const std::pair<std::vector<int>, T>& b)
            {
                return t_order.compare(a.first, b.first);
            });
        return terms;
    }

    std::vector<int> reorderExponents(
        const std::vector<int>& exps,
        const std::vector<std::string>& original_vars,
        const std::vector<std::string>& new_vars) const
    {
        std::vector<int> result(new_vars.size(), 0);
        
        for (size_t i = 0; i < original_vars.size(); ++i)
        {
            for (size_t j = 0; j < new_vars.size(); ++j)
            {
                if (original_vars[i] == new_vars[j])
                {
                    result[j] = exps[i];
                    break;
                }
            }
        }
        
        return result;
    }
    
public:

    std::pair<std::vector<polynomial<T>>, polynomial<T>> 
        divide(const polynomial<T>& f, const std::vector<polynomial<T>>& divisors) const
    {
        if (divisors.empty())
        throw std::invalid_argument("At least one divisor required");

        if (f.vars != vars || f.getterm_order_type() != t_order.getType())
            throw std::invalid_argument("Dividend must have same variable names/order as this");

        for (const auto& div : divisors)
        {
            if (div.vars != vars || div.getterm_order_type() != t_order.getType())
                throw std::invalid_argument("All divisors must have same variable names/order as this");
        }

        
        std::vector<polynomial<T>> quotients(divisors.size(), polynomial<T>(vars, t_order.getType()));
        polynomial<T> remainder(vars, t_order.getType());
        polynomial<T> p = f;
        
        while (!p.isZero())
        {
            bool division_occured = false;
            size_t i = 0;
            
            while (i < divisors.size() &&  !division_occured)
            {
                std::vector<int> pLM = p.lm();
                std::vector<int> divLM = divisors[i].lm();
                
                bool divisible = true;
                for (size_t j = 0; j < pLM.size(); ++j)
                {
                    if (pLM[j] < divLM[j])
                    {
                        divisible = false;
                        break;
                    }
                }
                
                if (divisible)
                {
                    std::vector<int> expDiff(pLM.size());
                    for (size_t j = 0; j < pLM.size(); ++j)
                    {
                        expDiff[j] = pLM[j] - divLM[j];
                    }
                    
                    T coeff = p.lc() / divisors[i].lc();
                    if constexpr (std::is_integral<T>::value)
                    {
                        if (coeff * divisors[i].lc() != p.lc())
                            throw std::invalid_argument("Division not exact: leading coefficients do not divide");
                    }
                    
                    polynomial<T> term(vars, t_order.getType());
                    term.addTerm(expDiff, coeff);
                    
                    quotients[i] = quotients[i] + term;
                    p = p - term * divisors[i];
                    division_occured = true;
                }
                else
                {
                    ++i;
                }
            }
            
            if (!division_occured)
            {
                std::vector<int> pLM = p.lm();
                T coeff = p.lc();
                polynomial<T> ltPoly(vars, t_order.getType());
                ltPoly.addTerm(pLM, coeff);
                remainder = remainder + ltPoly;
                p = p - ltPoly;
            }
        }
        
        return {quotients, remainder};
    }


    bool reduceByIdeal(const polynomial<T>& f, const std::vector<polynomial<T>>& basis) const
    {
        return divide(f, basis).second.isZero();
    }


    bool divides(const polynomial<T>& f, const polynomial<T>& g) const
    {
        if (f.isZero()) return false;
        auto result = divide(g, {f});
        return result.second.isZero();
    }

    polynomial reorderVariables(const std::vector<std::string>& new_vars) const
    {
        if (new_vars.size() != vars.size())
        {
            throw std::invalid_argument("Number of variables must match");
        }
        
        polynomial result(new_vars, t_order.getType());
        
        std::vector<std::pair<std::vector<int>, T>> terms = supp();
        
        for (const auto& term : terms)
        {
            std::vector<int> new_exps = reorderExponents(term.first, vars, new_vars);
            result.addTerm(new_exps, term.second);
        }
        
        return result;
    }
    
    polynomial withVariableOrder(const std::vector<std::string>& new_vars) const
    {
        return reorderVariables(new_vars);
    }
    
    void removeZeroCoefficients(Node* node)
    {
        if (node)
        {
            cleanNode(node);
        }
    }
    
    explicit polynomial(const std::vector<std::string>& variables, term_order_type orderType = term_order_type::LEX)
        : root(new Node())
        , vars(variables)
        , t_order(orderType)
    {
        if (variables.empty())
        {
            throw std::invalid_argument("At least one variable required");
        }
    }
    
    polynomial(const polynomial& other)
        : root(clone(other.root))
        , vars(other.vars)
        , t_order(other.t_order)
    {
    }
    
    polynomial(polynomial&& other) noexcept
        : root(other.root)
        , vars(std::move(other.vars))
        , t_order(other.t_order)
    {
        other.root = nullptr;
    }
    
    ~polynomial()
    {
        if (root)
        {
            clear(root);
        }
    }
    
    polynomial& operator=(const polynomial& other)
    {
        if (this != &other)
        {
            polynomial temp(other);
            swap(temp);
        }
        
        return *this;
    }
    
    polynomial& operator=(polynomial&& other) noexcept
    {
        if (this != &other)
        {
            clear(root);
            root = other.root;
            vars = std::move(other.vars);
            t_order = other.t_order;
            other.root = nullptr;
        }
        
        return *this;
    }
    
    void swap(polynomial& other) noexcept
    {
        std::swap(root, other.root);
        std::swap(vars, other.vars);
        std::swap(t_order, other.t_order);
    }
    
    void addTerm(const std::vector<int>& exps, const T& coeff)
    {
        if (exps.size() != vars.size())
        {
            throw std::invalid_argument("Exponent vector size doesn't match number of variables");
        }
        
        for (int e : exps)
        {
            if (e < 0)
            {
                throw std::invalid_argument("Negative exponents not allowed");
            }
        }
        
        insert(root, exps, 0, coeff);
        removeZeroCoefficients(root);
    }
    
    polynomial operator+(const polynomial& other) const
    {
        polynomial result = *this;
        result += other;
        return result;
    }
    
    polynomial operator-() const
    {
        polynomial result(*this);
        result.negate(result.root);
        result.removeZeroCoefficients(result.root);
        return result;
    }
    
    polynomial operator-(const polynomial& other) const
    {
        polynomial result = *this;
        result -= other;
        return result;
    }
    
    polynomial operator*(const polynomial& other) const
    {
        polynomial result = *this;
        result *= other;
        return result;
    }
    
    polynomial operator*(const T& scalar) const
    {
        polynomial result = *this;
        result *= scalar;
        return result;
    }
    
    polynomial& operator*=(const polynomial& other)
    {
        if (vars.size() != other.vars.size())
        {
            throw std::invalid_argument("Cannot multiply polynomials with different number of variables");
        }

        if (vars != other.vars)
        {
            throw std::invalid_argument("Cannot divide polynomials with different variable names");
        }
        
        polynomial result(vars, t_order.getType());
        
        std::vector<std::pair<std::vector<int>, T>> a, b;
        std::vector<int> cur;
        
        collect(root, cur, a);
        collect(other.root, cur, b);
        
        for (const auto& termA : a)
        {
            for (const auto& termB : b)
            {
                std::vector<int> e(vars.size());
                
                for (size_t i = 0; i < vars.size(); ++i)
                {
                    e[i] = termA.first[i] + termB.first[i];
                }
                
                T coeff = termA.second * termB.second;
                if (coeff != T{})
                {
                    result.addTerm(e, coeff);
                }
            }
        }
        
        result.removeZeroCoefficients(result.root);
        *this = std::move(result);
        return *this;
    }
    
    polynomial& operator*=(const T& scalar)
    {
        scale(root, scalar);
        removeZeroCoefficients(root);
        return *this;
    }
    
    friend polynomial operator*(const T& scalar, const polynomial& p)
    {
        return p * scalar;
    }
    
    polynomial& operator+=(const polynomial& other)
    {
        if (vars.size() != other.vars.size())
        {
            throw std::invalid_argument("Cannot add polynomials with different number of variables");
        }
        if (vars != other.vars)
        {
            throw std::invalid_argument("Cannot divide polynomials with different variable names");
        }
        addNodes(root, other.root);
        removeZeroCoefficients(root);
        return *this;
    }
    
    polynomial& operator-=(const polynomial& other)
    {
        if (vars.size() != other.vars.size())
        {
            throw std::invalid_argument("Cannot subtract polynomials with different number of variables");
        }
        if (vars != other.vars)
        {
            throw std::invalid_argument("Cannot divide polynomials with different variable names");
        }
        subNodes(root, other.root);
        removeZeroCoefficients(root);
        return *this;
    }

    polynomial operator/(const polynomial& other) const
    {
        polynomial result = *this;
        result /= other;
        return result;
    }

    polynomial operator/(const T& scalar) const
    {
        if (scalar == T{})
        {
            throw std::invalid_argument("Division by zero scalar");
        }
        polynomial result = *this;
        result /= scalar;
        return result;
    }

    polynomial& operator/=(const T& scalar)
    {
        if (scalar == T{})
        {
            throw std::invalid_argument("Division by zero scalar");
        }
        
        std::vector<std::pair<std::vector<int>, T>> terms = supp();
        polynomial<T> result(vars, t_order.getType());
        
        for (const auto& term : terms)
        {
            T new_coeff = term.second / scalar;
            if constexpr (std::is_integral<T>::value)
            {
                if (new_coeff * scalar != term.second)
                {
                    throw std::invalid_argument("Division is not exact - scalar does not divide coefficient");
                }
            }
            result.addTerm(term.first, new_coeff);
        }
        
        *this = std::move(result);
        return *this;
    }

    polynomial& operator/=(const polynomial& other)
    {
        if (vars.size() != other.vars.size())
        {
            throw std::invalid_argument("Cannot divide polynomials with different number of variables");
        }

        if (vars != other.vars)
        {
            throw std::invalid_argument("Cannot divide polynomials with different variable names");
        }
        
        if (other.isZero())
        {
            throw std::invalid_argument("Division by zero polynomial");
        }
        
        std::vector<polynomial<T>> divisors = {other};
        auto division_result = divide(*this, divisors);
        
        if (!division_result.second.isZero())
        {
            throw std::invalid_argument("Division is not exact - remainder is not zero");
        }
        
        *this = division_result.first[0];
        return *this;
    }
    
    std::vector<std::pair<std::vector<int>, T>> supp() const
    {
        std::vector<std::pair<std::vector<int>, T>> res;
        std::vector<int> cur;
        collect(root, cur, res);
        return res;
    }
    
    bool operator==(const polynomial& other) const
    {
        if (vars.size() != other.vars.size())
        {
            return false;
        }
        
        return equalNodes(root, other.root);
    }
    
    bool operator!=(const polynomial& other) const
    {
        return !(*this == other);
    }
    
    T evaluate(const std::vector<T>& point) const
    {
        if (point.size() != vars.size())
        {
            throw std::invalid_argument("Point dimension doesn't match number of variables");
        }
        
        return evaluateNode(root, point, 0, T{1});
    }
    
    int homogeneousDegree() const
    {
        int refSum = -1;
        
        if (isHomogeneousNode(root, 0, refSum))
        {
            return refSum;
        }
        
        return -1;
    }
    
    bool isHomogeneous() const
    {
        return homogeneousDegree() != -1;
    }
    
    polynomial homogeneousComponent(int degree) const
    {
        if (degree < 0)
        {
            throw std::invalid_argument("Degree must be non-negative");
        }
        
        polynomial result(vars, t_order.getType());
        extractHomogeneous(root, result.root, 0, degree);
        result.removeZeroCoefficients(result.root);
        return result;
    }
    
    size_t numberOfVariables() const
    {
        return vars.size();
    }
    
    const std::vector<std::string>& variables() const
    {
        return vars;
    }
    
    bool isZero() const
    {
        return root->coeff == T{} && root->children.empty();
    }
    
    // leading term data -------------------------------------
    std::vector<int> multideg() const
    {
        auto terms = getSortedTerms();
        if (terms.empty())
        {
            return std::vector<int>(vars.size(), 0);
        }
        return terms[0].first;
    }
    
    T lc() const
    {
        auto terms = getSortedTerms();
        if (terms.empty())
        {
            return T{};
        }
        return terms[0].second;
    }
    
    std::pair<std::vector<int>, T> lt() const
    {
        auto terms = getSortedTerms();
        if (terms.empty())
        {
            return {std::vector<int>(vars.size(), 0), T{}};
        }
        return {terms[0].first, terms[0].second};
    }
    
    std::vector<int> lm() const
    {
        return multideg();
    }
    
    term_order_type getterm_order_type() const
    {
        return t_order.getType();
    }
    
    void setterm_order_type(term_order_type newOrder)
    {
        t_order = term_order(newOrder);
    }
    // ------------------------------------------------------------------

    
    friend std::ostream& operator<<(std::ostream& os, const polynomial& p)
    {
        using namespace polynomial_utils;
        auto terms = p.getSortedTerms();
        
        if (terms.empty())
        {
            os << "0";
            return os;
        }
        
        bool first = true;
        for (const auto& term : terms)
        {
            const auto& exps = term.first;
            const T& coeff = term.second;
            
            bool negative = isNegative(coeff);
            if (!first)
            {
                os << (negative ? " - " : " + ");
            }
            else
            {
                if (negative)
                {
                    os << "-";
                }
                first = false;
            }
            
            T absC = negative ? -coeff : coeff;
            bool allExpsZero = std::all_of(exps.begin(), exps.end(), [](int e) { return e == 0; });
            bool needCoeff = !(isOne(absC) && !allExpsZero);
            
            printCoeff(os, absC, needCoeff);
            
            bool firstVar = true;
            for (size_t i = 0; i < exps.size(); ++i)
            {
                if (exps[i] > 0)
                {
                    if (!firstVar)
                    {
                        os << "*";
                    }
                    os << p.vars[i];
                    if (exps[i] > 1)
                    {
                        os << "^" << exps[i];
                    }
                    firstVar = false;
                }
            }
        }
        return os;
    }
};

#endif