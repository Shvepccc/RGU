#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <complex>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <type_traits>

// ==================== Концепты для типов коэффициентов ====================

// Концепт для типа, поддерживающего базовые арифметические операции
template<typename T>
concept Arithmetic = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
    { a - b } -> std::convertible_to<T>;
    { a * b } -> std::convertible_to<T>;
    { -a } -> std::convertible_to<T>;
    { a == b } -> std::convertible_to<bool>;
    { a != b } -> std::convertible_to<bool>;
    T{0};
    T{1};
};

// Концепт для типа с поддержкой оператора < (для определения знака)
template<typename T>
concept Ordered = Arithmetic<T> && requires(T a) {
    { a < T{} } -> std::convertible_to<bool>;
};

// Концепт для комплексных чисел
template<typename T>
concept Complex = requires(T a) {
    typename T::value_type;
    { a.real() } -> std::convertible_to<typename T::value_type>;
    { a.imag() } -> std::convertible_to<typename T::value_type>;
};

// ==================== Вспомогательные функции через перегрузку ====================

// Проверка на ноль
template<Arithmetic T>
inline bool isZero(const T& value) {
    return value == T{0};
}

template<Complex T>
inline bool isZero(const T& value) {
    return value == T{0};
}

// Проверка на единицу
template<Arithmetic T>
inline bool isOne(const T& value) {
    return value == T{1};
}

template<Complex T>
inline bool isOne(const T& value) {
    return value == T{1};
}

// Проверка на отрицательность (для вывода)
template<Ordered T>
inline bool isNegative(const T& value) {
    return value < T{0};
}

template<Complex T>
inline bool isNegative(const T& value) {
    return value.real() < 0;
}

template<typename T>
inline bool isNegative(const T& value) {
    return false; // Для типов без оператора <
}

// Возведение в степень
template<Arithmetic T>
T power(const T& base, int exp) {
    if (exp == 0) return T{1};
    T result = base;
    for (int i = 1; i < exp; ++i) {
        result = result * base;
    }
    return result;
}

// ==================== Класс Modular (кольцо вычетов) ====================

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
    
    // Для совместимости с Ordered концептом
    bool operator<(const Modular& other) const { return value < other.value; }
    
    friend std::ostream& operator<<(std::ostream& os, const Modular& m) {
        os << m.value;
        return os;
    }
};

// ==================== Класс Polynomial ====================

template<Arithmetic T>
class Polynomial {
private:
    struct Node {
        T coeff;
        std::map<int, Node*> children;
        
        Node() : coeff(T{0}) {}
        Node(const T& c) : coeff(c) {}
        
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
    };
    
    Node* root;
    std::vector<std::string> vars;
    
    // Глубокое копирование
    Node* clone(Node* node) const {
        if (!node) return nullptr;
        Node* n = new Node(node->coeff);
        for (const auto& [k, v] : node->children) {
            n->children[k] = clone(v);
        }
        return n;
    }
    
    // Рекурсивное удаление
    void clear(Node* node) {
        if (!node) return;
        for (auto& [k, v] : node->children) {
            clear(v);
        }
        delete node;
    }
    
    // Вставка монома
    void insert(Node* node, const std::vector<int>& exps, size_t depth, T value) {
        if (::isZero(value)) return;
        
        if (depth == exps.size()) {
            node->coeff += value;
            return;
        }
        
        int e = exps[depth];
        if (node->children.count(e) == 0) {
            node->children[e] = new Node();
        }
        insert(node->children[e], exps, depth + 1, value);
    }
    
    // Сложение узлов
    void addNodes(Node* a, const Node* b) {
        if (!b) return;
        a->coeff += b->coeff;
        
        for (const auto& [k, v] : b->children) {
            if (a->children.count(k) == 0) {
                a->children[k] = clone(v);
            } else {
                addNodes(a->children[k], v);
            }
        }
    }
    
    // Вычитание узлов
    void subNodes(Node* a, const Node* b) {
        if (!b) return;
        a->coeff -= b->coeff;
        
        for (const auto& [k, v] : b->children) {
            if (a->children.count(k) == 0) {
                Node* tmp = clone(v);
                negateNode(tmp);
                a->children[k] = tmp;
            } else {
                subNodes(a->children[k], v);
            }
        }
    }
    
    // Умножение узла на скаляр
    void scaleNode(Node* node, const T& scalar) {
        if (!node) return;
        node->coeff *= scalar;
        for (auto& [k, v] : node->children) {
            scaleNode(v, scalar);
        }
    }
    
    // Смена знака узла
    void negateNode(Node* node) {
        node->coeff = -node->coeff;
        for (auto& [k, v] : node->children) {
            negateNode(v);
        }
    }
    
    // Сбор всех мономов
    void collect(const Node* node, std::vector<int>& cur,
                 std::vector<std::pair<std::vector<int>, T>>& res) const {
        if (!node) return;
        
        if (!::isZero(node->coeff)) {
            res.push_back({cur, node->coeff});
        }
        
        for (const auto& [k, v] : node->children) {
            cur.push_back(k);
            collect(v, cur, res);
            cur.pop_back();
        }
    }
    
    // Вычисление значения
    T evaluateNode(const Node* node, const std::vector<T>& point,
                   size_t depth, T accum) const {
        T result = T{0};
        
        if (!::isZero(node->coeff)) {
            result = result + node->coeff * accum;
        }
        
        if (depth < point.size()) {
            for (const auto& [k, v] : node->children) {
                T powVal = ::power(point[depth], k);
                T val = accum * powVal;
                result = result + evaluateNode(v, point, depth + 1, val);
            }
        }
        
        return result;
    }
    
    // Определение однородности
    bool isHomogeneousNode(const Node* node, int currentSum, int& refSum) const {
        if (!node) return true;
        
        if (!::isZero(node->coeff)) {
            if (refSum == -1) {
                refSum = currentSum;
            } else if (refSum != currentSum) {
                return false;
            }
        }
        
        for (const auto& [k, v] : node->children) {
            if (!isHomogeneousNode(v, currentSum + k, refSum)) {
                return false;
            }
        }
        
        return true;
    }
    
    // Извлечение однородной компоненты
    void extractHomogeneous(const Node* node, Node* out, int currentSum, int targetDegree) const {
        if (!node) return;
        
        if (!::isZero(node->coeff) && currentSum == targetDegree) {
            out->coeff = node->coeff;
        }
        
        for (const auto& [k, v] : node->children) {
            if (out->children.count(k) == 0) {
                out->children[k] = new Node();
            }
            extractHomogeneous(v, out->children[k], currentSum + k, targetDegree);
        }
    }
    
    // Сравнение узлов
    bool equalNodes(const Node* a, const Node* b) const {
        if (!a && !b) return true;
        if (!a || !b) return false;
        if (a->coeff != b->coeff) return false;
        if (a->children.size() != b->children.size()) return false;
        
        for (const auto& [k, v] : a->children) {
            auto it = b->children.find(k);
            if (it == b->children.end()) return false;
            if (!equalNodes(v, it->second)) return false;
        }
        
        return true;
    }
    
    // Удаление нулевых коэффициентов
    void removeZeroCoefficients(Node* node) {
        if (!node) return;
        
        std::vector<int> toErase;
        for (auto& [k, v] : node->children) {
            removeZeroCoefficients(v);
            if (::isZero(v->coeff) && v->children.empty()) {
                delete v;
                toErase.push_back(k);
            }
        }
        
        for (int k : toErase) {
            node->children.erase(k);
        }
    }
    
public:
    // Конструкторы
    explicit Polynomial(const std::vector<std::string>& variables)
        : root(new Node()), vars(variables) {
        if (variables.empty()) {
            throw std::invalid_argument("At least one variable required");
        }
    }
    
    Polynomial(const Polynomial& other)
        : root(clone(other.root)), vars(other.vars) {}
    
    Polynomial(Polynomial&& other) noexcept
        : root(other.root), vars(std::move(other.vars)) {
        other.root = nullptr;
    }
    
    ~Polynomial() {
        if (root) clear(root);
    }
    
    // Операторы присваивания
    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {
            Polynomial temp(other);
            swap(temp);
        }
        return *this;
    }
    
    Polynomial& operator=(Polynomial&& other) noexcept {
        if (this != &other) {
            clear(root);
            root = other.root;
            vars = std::move(other.vars);
            other.root = nullptr;
        }
        return *this;
    }
    
    void swap(Polynomial& other) noexcept {
        std::swap(root, other.root);
        std::swap(vars, other.vars);
    }
    
    // Добавление монома
    void addTerm(const std::vector<int>& exps, const T& coeff) {
        if (exps.size() != vars.size()) {
            throw std::invalid_argument("Exponent vector size doesn't match number of variables");
        }
        
        for (int e : exps) {
            if (e < 0) {
                throw std::invalid_argument("Negative exponents not allowed");
            }
        }
        
        insert(root, exps, 0, coeff);
    }
    
    // Арифметические операции
    Polynomial operator+(const Polynomial& other) const {
        if (vars.size() != other.vars.size()) {
            throw std::invalid_argument("Cannot add polynomials with different number of variables");
        }
        
        Polynomial result(*this);
        result.addNodes(result.root, other.root);
        result.removeZeroCoefficients(result.root);
        return result;
    }
    
    Polynomial operator-() const {
        Polynomial result(*this);
        result.negateNode(result.root);
        return result;
    }
    
    Polynomial operator-(const Polynomial& other) const {
        if (vars.size() != other.vars.size()) {
            throw std::invalid_argument("Cannot subtract polynomials with different number of variables");
        }
        
        Polynomial result(*this);
        result.subNodes(result.root, other.root);
        result.removeZeroCoefficients(result.root);
        return result;
    }
    
    Polynomial operator*(const Polynomial& other) const {
        if (vars.size() != other.vars.size()) {
            throw std::invalid_argument("Cannot multiply polynomials with different number of variables");
        }
        
        Polynomial result(vars);
        
        std::vector<std::pair<std::vector<int>, T>> a, b;
        std::vector<int> cur;
        
        collect(root, cur, a);
        collect(other.root, cur, b);
        
        for (const auto& [e1, c1] : a) {
            for (const auto& [e2, c2] : b) {
                std::vector<int> e(vars.size());
                for (size_t i = 0; i < vars.size(); ++i) {
                    e[i] = e1[i] + e2[i];
                }
                result.addTerm(e, c1 * c2);
            }
        }
        
        result.removeZeroCoefficients(result.root);
        return result;
    }
    
    Polynomial operator*(const T& scalar) const {
        Polynomial result(*this);
        result.scaleNode(result.root, scalar);
        result.removeZeroCoefficients(result.root);
        return result;
    }
    
    friend Polynomial operator*(const T& scalar, const Polynomial& p) {
        return p * scalar;
    }
    
    // Составные операции
    Polynomial& operator+=(const Polynomial& other) {
        *this = *this + other;
        return *this;
    }
    
    Polynomial& operator-=(const Polynomial& other) {
        *this = *this - other;
        return *this;
    }
    
    Polynomial& operator*=(const Polynomial& other) {
        *this = *this * other;
        return *this;
    }
    
    Polynomial& operator*=(const T& scalar) {
        *this = *this * scalar;
        return *this;
    }
    
    // Получение носителя
    std::vector<std::pair<std::vector<int>, T>> supp() const {
        std::vector<std::pair<std::vector<int>, T>> res;
        std::vector<int> cur;
        collect(root, cur, res);
        return res;
    }
    
    // Сравнение
    bool operator==(const Polynomial& other) const {
        if (vars.size() != other.vars.size()) return false;
        return equalNodes(root, other.root);
    }
    
    bool operator!=(const Polynomial& other) const {
        return !(*this == other);
    }
    
    // Вычисление значения
    T evaluate(const std::vector<T>& point) const {
        if (point.size() != vars.size()) {
            throw std::invalid_argument("Point dimension doesn't match number of variables");
        }
        return evaluateNode(root, point, 0, T{1});
    }
    
    // Определение степени однородности
    int homogeneousDegree() const {
        int refSum = -1;
        if (isHomogeneousNode(root, 0, refSum)) {
            return refSum;
        }
        return -1;
    }
    
    bool isHomogeneous() const {
        return homogeneousDegree() != -1;
    }
    
    // Получение однородной компоненты
    Polynomial homogeneousComponent(int degree) const {
        if (degree < 0) {
            throw std::invalid_argument("Degree must be non-negative");
        }
        
        Polynomial result(vars);
        extractHomogeneous(root, result.root, 0, degree);
        result.removeZeroCoefficients(result.root);
        return result;
    }
    
    // Вспомогательные методы
    size_t numberOfVariables() const {
        return vars.size();
    }
    
    const std::vector<std::string>& variables() const {
        return vars;
    }
    
    bool isZero() const {
        return ::isZero(root->coeff) && root->children.empty();
    }
    
    // Вывод в поток
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
        auto terms = p.supp();
        if (terms.empty()) {
            os << "0";
            return os;
        }
        
        bool first = true;
        for (const auto& [exps, coeff] : terms) {
            // Определяем знак
            bool negative = ::isNegative(coeff);
            
            if (!first) {
                os << (negative ? " - " : " + ");
            } else {
                if (negative) os << "-";
                first = false;
            }
            
            T absCoeff = negative ? -coeff : coeff;
            bool allExpsZero = std::all_of(exps.begin(), exps.end(), [](int e) { return e == 0; });
            bool needCoeff = !(::isOne(absCoeff) && !allExpsZero);
            
            if (needCoeff) {
                os << absCoeff;
            }
            
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

// ==================== Тестирование ====================

int main() {
    try {
        std::vector<std::string> vars = {"x", "y"};
        
        std::cout << "=== Test 1: Polynomial<double> ===\n";
        Polynomial<double> f(vars), g(vars);
        f.addTerm({2, 1}, 3.0);
        f.addTerm({0, 1}, 2.0);
        f.addTerm({1, 0}, 1.0);
        g.addTerm({1, 0}, 4.0);
        g.addTerm({0, 1}, -2.0);
        g.addTerm({0, 0}, 5.0);
        
        std::cout << "f = " << f << "\ng = " << g << std::endl;
        std::cout << "f+g = " << f+g << "\nf-g = " << f-g << "\nf*g = " << f*g << std::endl;
        std::cout << "f(2,3) = " << f.evaluate({2.0, 3.0}) << std::endl;
        
        std::cout << "\n=== Test 2: Polynomial<int> ===\n";
        Polynomial<int> f_int(vars);
        f_int.addTerm({2, 1}, 3);
        f_int.addTerm({0, 1}, 2);
        f_int.addTerm({1, 0}, 1);
        std::cout << "f_int = " << f_int << std::endl;
        std::cout << "f_int(2,3) = " << f_int.evaluate({2, 3}) << std::endl;
        
        std::cout << "\n=== Test 3: Polynomial<std::complex<double>> ===\n";
        Polynomial<std::complex<double>> f_c(vars);
        f_c.addTerm({2, 1}, {3.0, 1.0});
        f_c.addTerm({0, 1}, {2.0, -1.0});
        f_c.addTerm({1, 0}, {1.0, 0.0});
        std::cout << "f_c = " << f_c << std::endl;
        std::cout << "f_c(2,3) = " << f_c.evaluate({{2.0,0.0}, {3.0,0.0}}) << std::endl;
        
        std::cout << "\n=== Test 4: Polynomial<Modular<7>> ===\n";
        Polynomial<Modular<7>> f_mod(vars), g_mod(vars);
        f_mod.addTerm({2, 1}, Modular<7>(3));
        f_mod.addTerm({0, 1}, Modular<7>(2));
        f_mod.addTerm({1, 0}, Modular<7>(1));
        g_mod.addTerm({1, 0}, Modular<7>(4));
        g_mod.addTerm({0, 1}, Modular<7>(5)); // -2 mod 7
        g_mod.addTerm({0, 0}, Modular<7>(5));
        
        std::cout << "f_mod = " << f_mod << "\ng_mod = " << g_mod << std::endl;
        std::cout << "f_mod+g_mod = " << f_mod+g_mod << std::endl;
        std::cout << "f_mod*g_mod = " << f_mod*g_mod << std::endl;
        std::cout << "f_mod(2,3) = " << f_mod.evaluate({Modular<7>(2), Modular<7>(3)}) << std::endl;
        
        std::cout << "\n=== Homogeneity ===\n";
        std::cout << "f homogeneous? " << (f.isHomogeneous() ? "yes" : "no") << std::endl;
        std::cout << "degree: " << f.homogeneousDegree() << std::endl;
        std::cout << "degree 3 component: " << f.homogeneousComponent(3) << std::endl;
        std::cout << "degree 2 component: " << f.homogeneousComponent(2) << std::endl;
        
        std::cout << "\n=== Support ===\n";
        for (const auto& [exps, coeff] : f.supp()) {
            std::cout << "  ";
            for (size_t i = 0; i < exps.size(); ++i) {
                if (exps[i]) std::cout << vars[i] << "^" << exps[i] << " ";
            }
            std::cout << ": " << coeff << std::endl;
        }
        
        std::cout << "\n=== Zero polynomial ===\n";
        Polynomial<double> zero(vars);
        std::cout << zero << " isZero? " << zero.isZero() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}