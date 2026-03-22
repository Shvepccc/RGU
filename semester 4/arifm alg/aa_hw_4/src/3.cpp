#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <complex>
#include <stdexcept>
#include <algorithm>

namespace polynomial_utils {
    
    template<typename T>
    bool isZero(const T& v) { return v == T{}; }
    
    template<typename U>
    bool isZero(const std::complex<U>& v) { return v == std::complex<U>{}; }
    
    template<typename T>
    bool isOne(const T& v) { return v == T{1}; }
    
    template<typename U>
    bool isOne(const std::complex<U>& v) { return v == std::complex<U>{1, 0}; }
    
    template<typename T>
    bool isNegative(const T& v) { return v < T{}; }
    
    template<typename U>
    bool isNegative(const std::complex<U>& v) { return v.real() < 0; }
    
    template<typename T>
    T absValue(const T& v) { return v < T{} ? -v : v; }
    
    template<typename U>
    std::complex<U> absValue(const std::complex<U>& v) { return v; }
    
    template<typename T>
    T power(const T& base, int exp) {
        if (exp == 0) return T{1};
        T res = base;
        for (int i = 1; i < exp; ++i) res = res * base;
        return res;
    }
    
    template<typename U>
    std::complex<U> power(const std::complex<U>& base, int exp) {
        if (exp == 0) return std::complex<U>{1, 0};
        std::complex<U> res = base;
        for (int i = 1; i < exp; ++i) res = res * base;
        return res;
    }
    
    template<typename T>
    void printCoeff(std::ostream& os, const T& coeff, bool needCoeff) {
        if (needCoeff) os << coeff;
    }
    
    template<typename U>
    void printCoeff(std::ostream& os, const std::complex<U>& coeff, bool needCoeff) {
        if (needCoeff) {
            os << coeff;
        }
    }
}

template<int MOD>
class Modular {
private:
    int value;
    int normalize(int v) const {
        v %= MOD;
        if (v < 0) v += MOD;
        return v;
    }
public:
    Modular(int v = 0) : value(normalize(v)) {}
    Modular(long long v) : value(normalize(static_cast<int>(v % MOD))) {}
    
    int getValue() const { return value; }
    
    Modular operator+(const Modular& other) const { return Modular(value + other.value); }
    Modular operator-(const Modular& other) const { return Modular(value - other.value); }
    Modular operator*(const Modular& other) const { return Modular(static_cast<long long>(value) * other.value); }
    Modular operator-() const { return Modular(-value); }
    Modular& operator+=(const Modular& other) { value = normalize(value + other.value); return *this; }
    Modular& operator-=(const Modular& other) { value = normalize(value - other.value); return *this; }
    Modular& operator*=(const Modular& other) { value = normalize(static_cast<long long>(value) * other.value); return *this; }
    
    bool operator==(const Modular& other) const { return value == other.value; }
    bool operator!=(const Modular& other) const { return value != other.value; }
    bool operator<(const Modular& other) const { return value < other.value; }
    bool operator>(const Modular& other) const { return value > other.value; }
    
    friend std::ostream& operator<<(std::ostream& os, const Modular& m) {
        os << m.value;
        return os;
    }
};

template<typename T>
class Polynomial
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

    bool cleanNode(Node* node) {
        if (!node) return true;
        std::vector<int> toErase;
        for (auto& kv : node->children) {
            if (cleanNode(kv.second)) {
                delete kv.second;
                toErase.push_back(kv.first);
            }
        }
        for (int key : toErase) {
            node->children.erase(key);
        }
        return (node->coeff == T{}) && node->children.empty();
    }

public:
    void removeZeroCoefficients(Node* node) {
        if (node) cleanNode(node);
    }

    explicit Polynomial(const std::vector<std::string>& variables)
        : root(new Node())
        , vars(variables)
    {
        if (variables.empty())
        {
            throw std::invalid_argument("At least one variable required");
        }
    }

    Polynomial(const Polynomial& other)
        : root(clone(other.root))
        , vars(other.vars)
    {
    }

    Polynomial(Polynomial&& other) noexcept
        : root(other.root)
        , vars(std::move(other.vars))
    {
        other.root = nullptr;
    }

    ~Polynomial()
    {
        if (root)
        {
            clear(root);
        }
    }

    Polynomial& operator=(const Polynomial& other)
    {
        if (this != &other)
        {
            Polynomial temp(other);
            swap(temp);
        }

        return *this;
    }

    Polynomial& operator=(Polynomial&& other) noexcept
    {
        if (this != &other)
        {
            clear(root);
            root = other.root;
            vars = std::move(other.vars);
            other.root = nullptr;
        }

        return *this;
    }

    void swap(Polynomial& other) noexcept
    {
        std::swap(root, other.root);
        std::swap(vars, other.vars);
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
    }

    Polynomial operator+(const Polynomial& other) const
    {
        Polynomial result = *this;
        result += other;
        return result;
    }

    Polynomial operator-() const
    {
        Polynomial result(*this);
        result.negate(result.root);
        result.removeZeroCoefficients(result.root);
        return result;
    }

    Polynomial operator-(const Polynomial& other) const
    {
        Polynomial result = *this;
        result -= other;
        return result;
    }

    Polynomial operator*(const Polynomial& other) const
    {
        Polynomial result = *this;
        result *= other;
        return result;
    }

    Polynomial operator*(const T& scalar) const
    {
        Polynomial result = *this;
        result *= scalar;
        return result;
    }

    Polynomial& operator*=(const Polynomial& other)
    {
        if (vars.size() != other.vars.size())
        {
            throw std::invalid_argument("Cannot multiply polynomials with different number of variables");
        }

        Polynomial result(vars);

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

                result.addTerm(e, termA.second * termB.second);
            }
        }

        result.removeZeroCoefficients(result.root);
        *this = std::move(result);
        return *this;
    }

    Polynomial& operator*=(const T& scalar)
    {
        scale(root, scalar);
        removeZeroCoefficients(root);
        return *this;
    }

    friend Polynomial operator*(const T& scalar, const Polynomial& p)
    {
        return p * scalar;
    }

    Polynomial& operator+=(const Polynomial& other)
    {
        if (vars.size() != other.vars.size())
        {
            throw std::invalid_argument("Cannot add polynomials with different number of variables");
        }
        addNodes(root, other.root);
        removeZeroCoefficients(root);
        return *this;
    }

    Polynomial& operator-=(const Polynomial& other)
    {
        if (vars.size() != other.vars.size())
        {
            throw std::invalid_argument("Cannot subtract polynomials with different number of variables");
        }
        subNodes(root, other.root);
        removeZeroCoefficients(root);
        return *this;
    }

    std::vector<std::pair<std::vector<int>, T>> supp() const
    {
        std::vector<std::pair<std::vector<int>, T>> res;
        std::vector<int> cur;
        collect(root, cur, res);
        return res;
    }

    bool operator==(const Polynomial& other) const
    {
        if (vars.size() != other.vars.size())
        {
            return false;
        }

        return equalNodes(root, other.root);
    }

    bool operator!=(const Polynomial& other) const
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

    Polynomial homogeneousComponent(int degree) const
    {
        if (degree < 0)
        {
            throw std::invalid_argument("Degree must be non-negative");
        }

        Polynomial result(vars);
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

    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
        using namespace polynomial_utils;
        auto terms = p.supp();
        if (terms.empty()) {
            os << "0";
            return os;
        }
        bool first = true;
        for (const auto& term : terms) {
            const auto& exps = term.first;
            const T& coeff = term.second;
            
            bool negative = isNegative(coeff);
            if (!first)
                os << (negative ? " - " : " + ");
            else {
                if (negative) os << "-";
                first = false;
            }
            
            T absC = negative ? -coeff : coeff;
            bool allExpsZero = std::all_of(exps.begin(), exps.end(), [](int e) { return e == 0; });
            bool needCoeff = !(isOne(absC) && !allExpsZero);
            
            printCoeff(os, absC, needCoeff);
            
            bool firstVar = true;
            for (size_t i = 0; i < exps.size(); ++i) {
                if (exps[i] > 0) {
                    if (!firstVar) os << "*";
                    os << p.vars[i];
                    if (exps[i] > 1) os << "^" << exps[i];
                    firstVar = false;
                }
            }
        }
        return os;
    }
};

int main() {
    try {
        // -------------------------------------------------------------
        // 1. Многочлены от двух переменных "x", "y" с коэффициентами double
        // -------------------------------------------------------------
        std::vector<std::string> vars2 = {"x", "y"};
        Polynomial<double> p(vars2), q(vars2);

        p.addTerm({2, 1}, 3.5);
        p.addTerm({0, 1}, 2.3);
        p.addTerm({1, 0}, 1.7);

        q.addTerm({1, 0}, 4.0);
        q.addTerm({0, 1}, -2.0);
        q.addTerm({0, 0}, 5.0);

        std::cout << "=== Демонстрация для double (2 переменные) ===\n";
        std::cout << "p = " << p << "\n";
        std::cout << "q = " << q << "\n";
        std::cout << "p + q = " << p + q << "\n";
        std::cout << "p - q = " << p - q << "\n";
        std::cout << "p * q = " << p * q << "\n";
        std::cout << "2.0 * p = " << 2.0 * p << "\n";
        std::cout << "p * 2.0 = " << p * 2.0 << "\n";

        std::vector<double> point = {2.0, 3.0};
        std::cout << "p(2,3) = " << p.evaluate(point) << "\n";

        auto supp_p = p.supp();
        std::cout << "supp(p):\n";
        for (const auto& term : supp_p) {
            std::cout << "  ";
            for (size_t i = 0; i < term.first.size(); ++i)
                if (term.first[i]) std::cout << vars2[i] << "^" << term.first[i] << " ";
            std::cout << ": " << term.second << "\n";
        }

        std::cout << "p == p? " << (p == p ? "да" : "нет") << "\n";
        std::cout << "p == q? " << (p == q ? "да" : "нет") << "\n";

        // -------------------------------------------------------------
        // 2. Однородность и выделение компонент
        // -------------------------------------------------------------
        Polynomial<double> homogeneous(vars2);
        homogeneous.addTerm({3, 0}, 1.0);
        homogeneous.addTerm({2, 1}, 2.0);
        homogeneous.addTerm({1, 2}, 3.0);
        homogeneous.addTerm({0, 3}, 4.0);

        Polynomial<double> inhomogeneous(vars2);
        inhomogeneous.addTerm({2, 0}, 1.0);
        inhomogeneous.addTerm({1, 1}, 2.0);
        inhomogeneous.addTerm({0, 2}, 3.0);
        inhomogeneous.addTerm({0, 0}, 5.0);

        std::cout << "\n=== Однородность ===\n";
        std::cout << "h = " << homogeneous << "\n";
        std::cout << "h однороден? " << (homogeneous.isHomogeneous() ? "да" : "нет") << "\n";
        std::cout << "степень однородности h: " << homogeneous.homogeneousDegree() << "\n";
        std::cout << "g = " << inhomogeneous << "\n";
        std::cout << "g однороден? " << (inhomogeneous.isHomogeneous() ? "да" : "нет") << "\n";
        std::cout << "степень однородности g: " << inhomogeneous.homogeneousDegree() << "\n";

        std::cout << "\n=== Выделение однородных компонент ===\n";
        for (int deg = 0; deg <= 3; ++deg) {
            auto comp = homogeneous.homogeneousComponent(deg);
            std::cout << "Компонента степени " << deg << ": " << comp << "\n";
        }

        // -------------------------------------------------------------
        // 3. Многочлены от трёх переменных (int)
        // -------------------------------------------------------------
        std::vector<std::string> vars3 = {"a", "b", "c"};
        Polynomial<int> r(vars3);
        r.addTerm({1, 0, 2}, 3);
        r.addTerm({0, 2, 1}, -2);
        r.addTerm({0, 0, 0}, 7);
        r.addTerm({2, 1, 0}, 5);

        Polynomial<int> s(vars3);
        s.addTerm({1, 0, 2}, 3);
        s.addTerm({0, 2, 1}, 1);
        s.addTerm({0, 0, 1}, 4);

        std::cout << "\n=== Многочлены от трёх переменных (int) ===\n";
        std::cout << "r = " << r << "\n";
        std::cout << "s = " << s << "\n";
        std::cout << "r + s = " << r + s << "\n";
        std::cout << "r - s = " << r - s << "\n";
        std::cout << "r * s = " << r * s << "\n";

        std::vector<int> pt3 = {2, 1, 3};
        std::cout << "r(2,1,3) = " << r.evaluate(pt3) << "\n";

        // -------------------------------------------------------------
        // 4. Многочлены от одной переменной (complex<double>)
        // -------------------------------------------------------------
        std::vector<std::string> vars1 = {"z"};
        Polynomial<std::complex<double>> f(vars1);
        f.addTerm({2}, std::complex<double>{1.0, 1.0});
        f.addTerm({1}, std::complex<double>{2.0, -1.0});
        f.addTerm({0}, std::complex<double>{0.0, 3.0});

        std::cout << "\n=== Многочлен от одной переменной (комплексные) ===\n";
        std::cout << "f(z) = " << f << "\n";
        std::complex<double> z0 = {1.0, 2.0};
        std::cout << "f(" << z0 << ") = " << f.evaluate({z0}) << "\n";

        // -------------------------------------------------------------
        // 5. Многочлены с кольцом вычетов Modular<7>
        // -------------------------------------------------------------
        std::vector<std::string> vars_mod = {"uppi", "vappi"};
        Polynomial<Modular<7>> A(vars_mod), B(vars_mod);

        A.addTerm({2, 1}, Modular<7>(3));
        A.addTerm({0, 1}, Modular<7>(2));
        A.addTerm({1, 0}, Modular<7>(1));

        B.addTerm({1, 0}, Modular<7>(4));
        B.addTerm({0, 1}, Modular<7>(5));
        B.addTerm({0, 0}, Modular<7>(5));

        std::cout << "\n=== Многочлены над Z/7Z ===\n";
        std::cout << "A = " << A << "\n";
        std::cout << "B = " << B << "\n";
        std::cout << "A + B = " << A + B << "\n";
        std::cout << "A * B = " << A * B << "\n";

        std::vector<Modular<7>> pt_mod = {Modular<7>(2), Modular<7>(3)};
        std::cout << "A(2,3) mod 7 = " << A.evaluate(pt_mod) << "\n";

        // -------------------------------------------------------------
        // 6. Граничные случаи: нулевой многочлен, отрицательные степени,
        //    несовпадение размерностей (исключения)
        // -------------------------------------------------------------
        std::cout << "\n=== Граничные случаи ===\n";
        Polynomial<double> zero(vars2);
        std::cout << "Нулевой многочлен: " << zero << " (isZero? " << zero.isZero() << ")\n";
        std::cout << "supp(zero) размер: " << zero.supp().size() << "\n";

        std::cout << "p + zero = " << p + zero << "\n";

        std::cout << "zero.isHomogeneous() = " << zero.isHomogeneous() << "\n";
        std::cout << "zero.homogeneousDegree() = " << zero.homogeneousDegree() << "\n";

        try {
            zero.addTerm({1, 0, 1}, 5.0);
        } catch (const std::exception& e) {
            std::cout << "Ожидаемое исключение при addTerm: " << e.what() << "\n";
        }

        try {
            p.addTerm({-1, 2}, 1.0);
        } catch (const std::exception& e) {
            std::cout << "Ожидаемое исключение (отрицательная степень): " << e.what() << "\n";
        }

        Polynomial<double> oneVar(vars1);
        try {
            p + oneVar;
        } catch (const std::exception& e) {
            std::cout << "Ожидаемое исключение (разное число переменных): " << e.what() << "\n";
        }

        // -------------------------------------------------------------
        // 7. Умножение на скаляр и проверка равенства после преобразований
        // -------------------------------------------------------------
        Polynomial<double> p2 = p;
        p2 *= 2.0;
        std::cout << "\n=== Умножение на скаляр с присваиванием ===\n";
        std::cout << "p = " << p << "\n";
        std::cout << "p2 (p*2) = " << p2 << "\n";
        std::cout << "p2 == p*2 ? " << (p2 == p * 2.0 ? "да" : "нет") << "\n";

        // -------------------------------------------------------------
        // 8. Многочлен от 7 переменных с длинными названиями
        //    и различными степенями (4 различные степени)
        // -------------------------------------------------------------
        std::cout << "\n=== Многочлен от 7 переменных ===\n";
        std::vector<std::string> many_vars = {
            "temperature", "pressure", "volume", 
            "entropy", "enthalpy", "density", "viscosity"
        };

        Polynomial<double> complexPoly(many_vars);

        complexPoly.addTerm({0, 0, 0, 0, 0, 0, 0}, 3.14159);
        complexPoly.addTerm({2, 0, 1, 0, 0, 0, 0}, 2.71828);
        complexPoly.addTerm({0, 1, 0, 1, 0, 0, 0}, 1.41421);
        complexPoly.addTerm({1, 0, 2, 0, 1, 0, 0}, 0.57721);
        complexPoly.addTerm({0, 0, 0, 2, 0, 1, 0}, 1.61803);
        complexPoly.addTerm({3, 0, 0, 0, 0, 0, 2}, 0.69314);
        complexPoly.addTerm({0, 2, 0, 0, 2, 0, 1}, 0.30102);
        complexPoly.addTerm({1, 1, 1, 1, 1, 0, 0}, 0.47712);
        complexPoly.addTerm({0, 0, 3, 0, 0, 2, 1}, 0.84510);
        complexPoly.addTerm({2, 0, 0, 2, 0, 0, 2}, 0.12345);
        complexPoly.addTerm({0, 0, 0, 0, 0, 0, 4}, 0.98765);
        complexPoly.addTerm({4, 0, 0, 0, 0, 0, 0}, 0.45678);
        complexPoly.addTerm({0, 0, 0, 0, 0, 3, 0}, 0.78901);
        complexPoly.addTerm({0, 0, 0, 0, 0, 0, 0}, 2.71828);

        std::cout << "Многочлен от 7 переменных:\n";
        std::cout << complexPoly << "\n\n";

        std::cout << "Носитель (supp) с группировкой по суммарной степени:\n";
        auto supp_complex = complexPoly.supp();
        std::map<int, std::vector<std::pair<std::vector<int>, double>>> byDegree;
        for (const auto& term : supp_complex) {
            int totalDegree = 0;
            for (int exp : term.first) totalDegree += exp;
            byDegree[totalDegree].push_back(term);
        }

        for (const auto& [deg, terms] : byDegree) {
            std::cout << "  Степень " << deg << " (" << terms.size() << " членов):\n";
            for (const auto& term : terms) {
                std::cout << "    ";
                bool first = true;
                for (size_t i = 0; i < term.first.size(); ++i) {
                    if (term.first[i] > 0) {
                        if (!first) std::cout << " * ";
                        std::cout << many_vars[i];
                        if (term.first[i] > 1) std::cout << "^" << term.first[i];
                        first = false;
                    }
                }
                if (first) std::cout << "1";
                std::cout << " : " << term.second << "\n";
            }
        }
        std::cout << "\n";

        std::cout << "Многочлен однороден? " << (complexPoly.isHomogeneous() ? "да" : "нет") << "\n";
        std::cout << "Степень однородности: " << complexPoly.homogeneousDegree() << "\n";

        std::cout << "\nОднородные компоненты:\n";
        for (const auto& [deg, _] : byDegree) {
            auto component = complexPoly.homogeneousComponent(deg);
            std::cout << "  Компонента степени " << deg << ": " << component << "\n";
        }

        Polynomial<double> sumComponents(many_vars);
        for (const auto& [deg, _] : byDegree) {
            sumComponents = sumComponents + complexPoly.homogeneousComponent(deg);
        }
        std::cout << "\nСумма всех однородных компонент:\n" << sumComponents << "\n";
        std::cout << "Сумма компонент == исходный многочлен? " 
                << (sumComponents == complexPoly ? "ДА" : "НЕТ") << "\n";

        std::vector<double> point7(many_vars.size(), 1.0);
        std::cout << "\nЗначение в точке (1,1,1,1,1,1,1): " 
                << complexPoly.evaluate(point7) << "\n";
        std::cout << "Ожидаемое значение (сумма всех коэффициентов): ";
        double expected = 0;
        for (const auto& term : supp_complex) expected += term.second;
        std::cout << expected << "\n";

        std::vector<double> point7b = {2.0, 3.0, 1.5, 0.5, 2.5, 1.0, 4.0};
        std::cout << "Значение в точке (2,3,1.5,0.5,2.5,1,4): " 
                << complexPoly.evaluate(point7b) << "\n";

        std::cout << "\nПроверка операций с самим собой:\n";
        auto doubled = complexPoly + complexPoly;
        std::cout << "f + f = " << doubled << "\n";
        auto squared = complexPoly * complexPoly;
        std::cout << "Количество членов в f*f: " << squared.supp().size() << "\n";

        auto scaled = complexPoly * 2.5;
        std::cout << "f * 2.5 = " << scaled << "\n";

        std::cout << "\nВсе тесты успешно завершены.\n";

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}