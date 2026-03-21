#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <complex>
#include <cmath>
#include <stdexcept>
#include <algorithm>

template<typename T>
class Polynomial
{
private:
    struct Node
    {
        T coeff;
        std::map<int, Node*> children;

        Node() : coeff(T{}) {}
        Node(const T& c) : coeff(c) {}
        
        // Запрещаем копирование узлов напрямую
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
    };

    Node* root;
    std::vector<std::string> vars;

    // Глубокое копирование узла
    Node* clone(Node* node) const
    {
        if (!node) return nullptr;

        Node* n = new Node(node->coeff);
        
        for (auto& [k, v] : node->children)
        {
            n->children[k] = clone(v);
        }
        
        return n;
    }

    // Рекурсивное удаление узлов
    void clear(Node* node)
    {
        if (!node) return;
        
        for (auto& [k, v] : node->children)
        {
            clear(v);
        }
        
        delete node;
    }

    // Вставка монома с проверкой на нулевой коэффициент
    void insert(Node* node, const std::vector<int>& exps, size_t depth, T value)
    {
        if (value == T{}) return;  // Не храним нулевые коэффициенты
        
        if (depth == exps.size())
        {
            node->coeff += value;
            // Если коэффициент стал нулевым, удаляем узел?
            // В текущей реализации оставляем, т.к. удаление сложно
            return;
        }
        
        int e = exps[depth];
        
        if (node->children.count(e) == 0)
        {
            node->children[e] = new Node();
        }
        
        insert(node->children[e], exps, depth + 1, value);
    }

    // Сложение двух узлов с оптимизацией
    void addNodes(Node* a, const Node* b)
    {
        if (!b) return;
        
        a->coeff += b->coeff;
        
        for (auto& [k, v] : b->children)
        {
            if (a->children.count(k) == 0)
            {
                a->children[k] = clone(v);
            }
            else
            {
                addNodes(a->children[k], v);
            }
        }
    }

    // Вычитание узлов
    void subNodes(Node* a, const Node* b)
    {
        if (!b) return;
        
        a->coeff -= b->coeff;
        
        for (auto& [k, v] : b->children)
        {
            if (a->children.count(k) == 0)
            {
                Node* tmp = clone(v);
                negate(tmp);
                a->children[k] = tmp;
            }
            else
            {
                subNodes(a->children[k], v);
            }
        }
    }

    // Умножение узла на скаляр
    void scale(Node* node, const T& scalar)
    {
        if (!node) return;
        
        node->coeff *= scalar;
        
        for (auto& [k, v] : node->children)
        {
            scale(v, scalar);
        }
    }

    // Смена знака
    void negate(Node* node)
    {
        node->coeff = -node->coeff;
        
        for (auto& [k, v] : node->children)
        {
            negate(v);
        }
    }

    // Сбор всех мономов
    void collect(const Node* node, std::vector<int>& cur, 
                 std::vector<std::pair<std::vector<int>, T>>& res) const
    {
        if (!node) return;
        
        if (node->coeff != T{})
        {
            res.push_back({cur, node->coeff});
        }
        
        for (auto& [k, v] : node->children)
        {
            cur.push_back(k);
            collect(v, cur, res);
            cur.pop_back();
        }
    }

    // Вычисление значения
    T evaluateNode(const Node* node, const std::vector<T>& point, 
                   size_t depth, T accum) const
    {
        T result = T{};
        
        if (node->coeff != T{})
        {
            result += node->coeff * accum;
        }
        
        if (depth < point.size())
        {
            for (auto& [k, v] : node->children)
            {
                T val = accum * std::pow(point[depth], k);
                result += evaluateNode(v, point, depth + 1, val);
            }
        }
        
        return result;
    }

    // Определение однородности
    bool isHomogeneousNode(const Node* node, int depth, int currentSum, 
                           int& refSum) const
    {
        if (!node) return true;
        
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
        
        for (auto& [k, v] : node->children)
        {
            if (!isHomogeneousNode(v, depth + 1, currentSum + k, refSum))
            {
                return false;
            }
        }
        
        return true;
    }

    // Извлечение однородной компоненты
    void extractHomogeneous(const Node* node, Node* out, int currentSum, 
                            int targetDegree) const
    {
        if (!node) return;
        
        if (node->coeff != T{} && currentSum == targetDegree)
        {
            out->coeff = node->coeff;
        }
        
        for (auto& [k, v] : node->children)
        {
            if (currentSum + k <= targetDegree)  // Оптимизация: не идем дальше, если сумма превысит
            {
                if (out->children.count(k) == 0)
                {
                    out->children[k] = new Node();
                }
                extractHomogeneous(v, out->children[k], currentSum + k, targetDegree);
            }
        }
    }

    // Сравнение узлов
    bool equalNodes(const Node* a, const Node* b) const
    {
        if (!a && !b) return true;
        if (!a || !b) return false;
        
        if (a->coeff != b->coeff) return false;
        
        if (a->children.size() != b->children.size()) return false;
        
        for (auto& [k, v] : a->children)
        {
            auto it = b->children.find(k);
            if (it == b->children.end()) return false;
            if (!equalNodes(v, it->second)) return false;
        }
        
        return true;
    }

    // Удаление нулевых коэффициентов (оптимизация)
    void removeZeroCoefficients(Node* node)
    {
        if (!node) return;
        
        // Сначала обрабатываем детей
        std::vector<int> toErase;
        for (auto& [k, v] : node->children)
        {
            removeZeroCoefficients(v);
            
            // Если ребенок пуст (нет коэффициента и нет детей), удаляем его
            if (v->coeff == T{} && v->children.empty())
            {
                delete v;
                toErase.push_back(k);
            }
        }
        
        // Удаляем пустых детей
        for (int k : toErase)
        {
            node->children.erase(k);
        }
    }

public:
    // Конструкторы
    explicit Polynomial(const std::vector<std::string>& variables) 
        : root(new Node()), vars(variables) 
    {
        if (variables.empty()) 
        {
            throw std::invalid_argument("At least one variable required");
        }
    }
    
    Polynomial(const Polynomial& other) 
        : root(clone(other.root)), vars(other.vars) 
    {}
    
    Polynomial(Polynomial&& other) noexcept
        : root(other.root), vars(std::move(other.vars))
    {
        other.root = nullptr;
    }
    
    ~Polynomial() 
    {
        if (root) clear(root);
    }
    
    // Операторы присваивания
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
    
    // Добавление монома
    void addTerm(const std::vector<int>& exps, const T& coeff)
    {
        if (exps.size() != vars.size())
        {
            throw std::invalid_argument("Exponent vector size doesn't match number of variables");
        }
        
        // Проверка на неотрицательные степени
        for (int e : exps)
        {
            if (e < 0)
            {
                throw std::invalid_argument("Negative exponents not allowed");
            }
        }
        
        insert(root, exps, 0, coeff);
    }
    
    // Арифметические операции
    Polynomial operator+(const Polynomial& other) const
    {
        if (vars.size() != other.vars.size())
        {
            throw std::invalid_argument("Cannot add polynomials with different number of variables");
        }
        
        Polynomial result(*this);
        result.addNodes(result.root, other.root);
        result.removeZeroCoefficients(result.root);
        return result;
    }
    
    Polynomial operator-() const
    {
        Polynomial result(*this);
        result.negate(result.root);
        return result;
    }
    
    Polynomial operator-(const Polynomial& other) const
    {
        if (vars.size() != other.vars.size())
        {
            throw std::invalid_argument("Cannot subtract polynomials with different number of variables");
        }
        
        Polynomial result(*this);
        result.subNodes(result.root, other.root);
        result.removeZeroCoefficients(result.root);
        return result;
    }
    
    Polynomial operator*(const Polynomial& other) const
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

        
        for (const auto& [e1, c1] : a)
        {
            for (const auto& [e2, c2] : b)
            {
                std::vector<int> e(vars.size());
                for (size_t i = 0; i < vars.size(); ++i)
                {
                    e[i] = e1[i] + e2[i];
                }
                result.addTerm(e, c1 * c2);
            }
        }
        
        result.removeZeroCoefficients(result.root);
        return result;
    }
    
    // Умножение на скаляр
    Polynomial operator*(const T& scalar) const
    {
        Polynomial result(*this);
        result.scale(result.root, scalar);
        result.removeZeroCoefficients(result.root);
        return result;
    }
    
    friend Polynomial operator*(const T& scalar, const Polynomial& p)
    {
        return p * scalar;
    }
    
    // Составные операции
    Polynomial& operator+=(const Polynomial& other)
    {
        *this = *this + other;
        return *this;
    }
    
    Polynomial& operator-=(const Polynomial& other)
    {
        *this = *this - other;
        return *this;
    }
    
    Polynomial& operator*=(const Polynomial& other)
    {
        *this = *this * other;
        return *this;
    }
    
    Polynomial& operator*=(const T& scalar)
    {
        *this = *this * scalar;
        return *this;
    }
    
    // Получение носителя
    std::vector<std::pair<std::vector<int>, T>> supp() const
    {
        std::vector<std::pair<std::vector<int>, T>> res;
        std::vector<int> cur;
        collect(root, cur, res);
        return res;
    }
    
    // Сравнение
    bool operator==(const Polynomial& other) const
    {
        if (vars.size() != other.vars.size()) return false;
        return equalNodes(root, other.root);
    }
    
    bool operator!=(const Polynomial& other) const
    {
        return !(*this == other);
    }
    
    // Вычисление значения
    T evaluate(const std::vector<T>& point) const
    {
        if (point.size() != vars.size())
        {
            throw std::invalid_argument("Point dimension doesn't match number of variables");
        }
        
        return evaluateNode(root, point, 0, T{1});
    }
    
    // Определение степени однородности
    int homogeneousDegree() const
    {
        int refSum = -1;
        if (isHomogeneousNode(root, 0, 0, refSum))
        {
            return refSum;
        }
        return -1;
    }
    
    bool isHomogeneous() const
    {
        return homogeneousDegree() != -1;
    }
    
    // Получение однородной компоненты
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
    
    // Вспомогательные методы
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
    
    // Вывод в поток
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p)
    {
        auto terms = p.supp();
        if (terms.empty())
        {
            os << "0";
            return os;
        }
        
        bool first = true;
        for (const auto& [exps, coeff] : terms)
        {
            if (!first)
            {
                os << (coeff > T{} ? " + " : " - ");
            }
            else
            {
                if (coeff < T{}) os << "-";
                first = false;
            }
            
            T absCoeff = coeff < T{} ? -coeff : coeff;
            if (absCoeff != T{1} || std::all_of(exps.begin(), exps.end(), [](int e) { return e == 0; }))
            {
                os << absCoeff;
            }
            
            bool firstVar = true;
            for (size_t i = 0; i < exps.size(); ++i)
            {
                if (exps[i] > 0)
                {
                    if (!firstVar) os << "*";
                    os << p.vars[i];
                    if (exps[i] > 1) os << "^" << exps[i];
                    firstVar = false;
                }
                else if (exps[i] < 0)
                {
                    os << "*(error: negative exponent)";
                }
            }
        }
        
        return os;
    }
};

// Тестирование
int main()
{
    using T = double;
    
    try
    {
        std::vector<std::string> vars = {"xi_1", "yj_1"};
        
        Polynomial<T> f(vars);
        Polynomial<T> g(vars);
        
        f.addTerm({2, 1}, 3.0);
        f.addTerm({0, 1}, 2.0);
        f.addTerm({1, 0}, 1.0);
        
        g.addTerm({1, 0}, 4.0);
        g.addTerm({0, 1}, -2.0);
        g.addTerm({0, 0}, 5.0);
        
        std::cout << "f(xi_1,yj_1) = " << f << std::endl;
        std::cout << "g(xi_1,yj_1) = " << g << std::endl;
        
        auto sum = f + g;
        auto diff = f - g;
        auto prod = f * g;
        
        std::cout << "\nf + g = " << sum << std::endl;
        std::cout << "f - g = " << diff << std::endl;
        std::cout << "f * g = " << prod << std::endl;
        
        std::cout << "\nSupp(f):" << std::endl;
        auto supp = f.supp();
        for (const auto& [e, c] : supp)
        {
            std::cout << "  " << c << " * ";
            for (size_t i = 0; i < e.size(); ++i)
            {
                if (e[i] > 0)
                {
                    std::cout << vars[i];
                    if (e[i] > 1) std::cout << "^" << e[i];
                    if (i < e.size() - 1) std::cout << "*";
                }
            }
            std::cout << std::endl;
        }
        
        std::vector<T> point = {2.0, 3.0};
        std::cout << "\nf(2,3) = " << f.evaluate(point) << std::endl;
        
        int deg = f.homogeneousDegree();
        std::cout << "Homogeneous degree of f: " << deg << std::endl;
        std::cout << "Is f homogeneous? " << (f.isHomogeneous() ? "Yes" : "No") << std::endl;
        
        auto h3 = f.homogeneousComponent(3);
        std::cout << "Homogeneous component of degree 3: " << h3 << std::endl;
        
        auto h2 = f.homogeneousComponent(2);
        std::cout << "Homogeneous component of degree 2: " << h2 << std::endl;
        
        std::cout << "\nf == g? " << (f == g ? "Yes" : "No") << std::endl;
        
        // Проверка на нулевой многочлен
        Polynomial<T> zero(vars);
        std::cout << "Zero polynomial: " << zero << std::endl;
        std::cout << "Is zero polynomial? " << zero.isZero() << std::endl;
        
        // Проверка операций присваивания
        Polynomial<T> copy = f;
        std::cout << "\nCopy of f: " << copy << std::endl;
        
        // Проверка умножения на скаляр
        auto scaled = f * 2.0;
        std::cout << "2*f = " << scaled << std::endl;
        
        // Проверка составных операций
        Polynomial<T> h(vars);
        h = f;
        h += g;
        std::cout << "f += g: " << h << std::endl;
        
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}