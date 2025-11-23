#include <cstddef>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <stack>
#include <unordered_map>
#include <functional>
#include <cctype>
#include <stdexcept>

class BooleanExpression {
private: 
    
    struct TreeNode {
        std::string value;
        TreeNode* left;
        TreeNode* right;
        
        TreeNode(const std::string& val) : value(val), left(nullptr), right(nullptr) {}
        TreeNode(const std::string& val, TreeNode* l, TreeNode* r) : value(val), left(l), right(r) {}
    };

    struct OperatorInfo {
        char symbol;
        int precedence;
        std::function<bool(bool, bool)> function;
        bool isUnary;
        
        OperatorInfo(char sym, int prec, std::function<bool(bool, bool)> func, bool unary = false)
            : symbol(sym), precedence(prec), function(func), isUnary(unary) {}
        
        OperatorInfo() : symbol('\0'), precedence(-1), function(nullptr), isUnary(false) {}
    };
    
    std::unordered_map<char, OperatorInfo> operators;
    std::unordered_map<char, char> bracketPairs;

    std::vector<std::string> variables;
    TreeNode* expressionTree;
    
public:
    BooleanExpression() : expressionTree(nullptr) {
        initializeOperators();
        initializeBrackets();
    }

    BooleanExpression(const std::string& expression) : expressionTree(nullptr) {
        initializeOperators();
        initializeBrackets();
        expressionTree = parse(expression);
        extractVariables();
    }

    ~BooleanExpression() {
        clearTree(expressionTree);
    }

    void setBrackets(std::unordered_map<char, char> bracketPairs)
    {
        if (bracketPairs.empty())
        {
            throw std::runtime_error("Brackets collection is empty");
        }
        this->bracketPairs = bracketPairs;
    }
    
    void setOperator(char symbol, int precedence, std::function<bool(bool, bool)> function, bool isUnary = false) {
        operators[symbol] = OperatorInfo(symbol, precedence, function, isUnary);
    }
    
    int getPrecedence(char op) const {
        auto it = operators.find(op);
        if (it != operators.end()) {
            return it->second.precedence;
        }
        return -1;
    }
    
    bool isOperator(char c) const {
        return operators.find(c) != operators.end();
    }
    
    bool isUnaryOperator(char c) const {
        auto it = operators.find(c);
        return it != operators.end() && it->second.isUnary;
    }

    bool isOpenBracket(char c) const {
        return bracketPairs.find(c) != bracketPairs.end();
    }

    bool isCloseBracket(char c) const {
        for (const auto& pair : bracketPairs) {
            if (pair.second == c) {
                return true;
            }
        }
        return false;
    }
    
    TreeNode* parse(const std::string& expression) {
        std::stack<TreeNode*> operandStack;
        std::stack<char> operatorStack;
        
        int i = 0;
        bool expectOperand = true;

        if (operators.empty()) {
            throw std::runtime_error("No operators in parser");
        }
        if(bracketPairs.empty()) {
            throw std::runtime_error("No brakets in parser");
        }
        
        while (i < expression.length())
        {
            if (std::isspace(expression[i])) {
                i++;
                continue;
            }
            
            if (isOpenBracket(expression[i])) {
                if (!expectOperand) {
                    throw std::runtime_error("Unexpected opening bracket");
                }
                operatorStack.push(expression[i]);
                i++;
                continue;
            }
            
            if (isCloseBracket(expression[i])) {
                if (expectOperand) {
                    throw std::runtime_error("Unexpected closing bracket");
                }
                
                while (!operatorStack.empty() && !isOpenBracket(operatorStack.top())) {
                    processOperator(operandStack, operatorStack);
                }
                
                if (operatorStack.empty() || !isOpenBracket(operatorStack.top())) {
                    throw std::runtime_error("Mismatched brackets");
                }
                
                char openBracket = operatorStack.top();
                if (bracketPairs[openBracket] != expression[i]) {
                    throw std::runtime_error("Bracket's type mismatch");
                }
                
                operatorStack.pop();
                i++;
                continue;
            }
            
            if (isOperator(expression[i])) {
                char op = expression[i];
                
                if (expectOperand && isUnaryOperator(op)) {
                    operatorStack.push(op);
                    i++;
                    continue;
                }
                
                if (!expectOperand) {
                    while (!operatorStack.empty() && 
                           !isOpenBracket(operatorStack.top()) && 
                           getPrecedence(operatorStack.top()) < getPrecedence(op)) {
                        processOperator(operandStack, operatorStack);
                    }
                    operatorStack.push(op);
                    expectOperand = true;
                    i++;
                    continue;
                }
                else {
                    throw std::runtime_error("Unexpected operator");
                }
            }
            
            if (expectOperand) {
                std::string operand = extractVariable(expression, i);
                if (operand.empty()) {
                    throw std::runtime_error("Invalid variable name");
                }
                operandStack.push(new TreeNode(operand));
                expectOperand = false;
                continue;
            }
            
            throw std::runtime_error("Unexpected character: " + std::string(1, expression[i]));
        }
        
        while (!operatorStack.empty()) {
            if (isOpenBracket(operatorStack.top())) {
                throw std::runtime_error("Mismatched brackets");
            }
            processOperator(operandStack, operatorStack);
        }
        
        if (operandStack.size() != 1) {
            throw std::runtime_error("Invalid expression");
        }
        
        return operandStack.top();
    }

    bool evaluate(const std::unordered_map<std::string, bool>& variableValues) const {
        return evaluateInner(expressionTree, variableValues);
    }
    
    void clearTree(TreeNode* root) {
        if (root == nullptr) return;
        
        clearTree(root->left);
        clearTree(root->right);
        
        delete root;
    }

    std::vector<std::vector<bool>> generateTruthTable() const {
            
        std::vector<std::vector<bool>> truthTable;
        std::vector<std::string> sortedVars = getVariables();
        
        if (sortedVars.empty()) {
            return truthTable;
        }

        int varCount = sortedVars.size();
        int rowCount = 1 << varCount;

        for (int i = 0; i < rowCount; i++) {
            std::unordered_map<std::string, bool> currentValues;
            std::vector<bool> row;

            for (int j = 0; j < varCount; j++) {
                bool value = (i & (1 << (varCount - 1 - j))) != 0;
                currentValues[sortedVars[j]] = value;
                row.push_back(value);
            }

            try {
                bool result = evaluate(currentValues);
                row.push_back(result);
            } catch (const std::exception& e) {
                row.push_back(false);
            }

            truthTable.push_back(row);
        }

        return truthTable;
    }

    void printTree() const
    {
        printTreeInner(this->expressionTree);
        std::cout << std::endl;
    }

    void printTreeInner(TreeNode* root, int depth = 0) const {
        if (root == nullptr) return;
        
        printTreeInner(root->right, depth + 1);
        std::cout << std::string(depth * 4, ' ') << root->value << std::endl;
        printTreeInner(root->left, depth + 1);
    }
    
    std::vector<std::string> getVariables() const {
        std::vector<std::string> sortedVars = variables;
        std::sort(sortedVars.begin(), sortedVars.end());
        return sortedVars;
    }
    

// --------------------------------------------------------------------------------------------------------------
private:
    void initializeOperators() {
        setOperator('+', 1, [](bool a, bool b) { return a || b; }); // OR (дизъюнкция)
        setOperator('&', 2, [](bool a, bool b) { return a && b; }); // AND (конъюнкция)
        setOperator('@', 3, [](bool a, bool b) { return a != b; }); // XOR (сложение по модулю 2)
        setOperator('~', 3, [](bool a, bool b) { return a == b; }); // EQUIV (эквивалентность)
        setOperator('>', 4, [](bool a, bool b) { return !a || b; }); // IMPLIES (импликация)
        setOperator('|', 5, [](bool a, bool b) { return !(a && b); }); // NAND (штрих Шеффера)
        setOperator('!', 5, [](bool a, bool b) { return !(a || b); }); // NOR (стрелка Пирса)
        setOperator('-', 6, [](bool a, bool) { return !a; }, true); // NOT (отрицание) - унарный
    }

    void initializeBrackets() {
        bracketPairs = {
            {'(', ')'}, {'{', '}'}, {'[', ']'}
        };
    }

    std::string extractVariable(const std::string& expression, int& index) {
        size_t start = index;
        
        if (index >= expression.length() || !std::isalpha(expression[index])) {
            return "";
        }
        index++;

        if (expression[index] != '_') {
            return "";
        }
        index++;

        while (index < expression.length() && std::isdigit(expression[index])) {
            index++;
        }
        
        return expression.substr(start, index - start);
    }
    
    void processOperator(std::stack<TreeNode*>& operandStack, std::stack<char>& operatorStack) {
        if (operatorStack.empty() || operandStack.empty()) {
            throw std::runtime_error("Invalid expression structure");
        }
        
        char op = operatorStack.top();
        operatorStack.pop();
        
        if (isUnaryOperator(op)) {
            if (operandStack.empty()) {
                throw std::runtime_error("Not enough operands for unary operator");
            }
            TreeNode* operand = operandStack.top();
            operandStack.pop();
            TreeNode* newNode = new TreeNode(std::string(1, op), nullptr, operand);
            operandStack.push(newNode);
        }

        else {
            if (operandStack.size() < 2) {
                throw std::runtime_error("Not enough operands for binary operator");
            }
            TreeNode* right = operandStack.top();
            operandStack.pop();
            TreeNode* left = operandStack.top();
            operandStack.pop();
            TreeNode* newNode = new TreeNode(std::string(1, op), left, right);
            operandStack.push(newNode);
        }
    }

    void extractVariables() {
        variables.clear();
        extractVariablesFromTree(expressionTree);
        
        std::unordered_set<std::string> uniqueVars(variables.begin(), variables.end());
        variables.assign(uniqueVars.begin(), uniqueVars.end());
    }

    void extractVariablesFromTree(TreeNode* node) {
        if (node == nullptr) return;
        
        if (node->left == nullptr && node->right == nullptr) {
            if (!node->value.empty() && std::isalpha(node->value[0])) {
                variables.push_back(node->value);
            }
            return;
        }
        
        extractVariablesFromTree(node->left);
        extractVariablesFromTree(node->right);
    }

    bool evaluateInner(TreeNode* root, const std::unordered_map<std::string, bool>& variables) const {
        if (root == nullptr) return false;
        
        // variable
        if (root->left == nullptr && root->right == nullptr) {
            auto it = variables.find(root->value);
            if (it == variables.end()) {
                throw std::runtime_error("Undefined variable: " + root->value);
            }
            return it->second;
        }
        
        // unary operator
        if (root->left == nullptr && root->right != nullptr) {
            bool operand = evaluateInner(root->right, variables);
            auto it = operators.find(root->value[0]);
            if (it != operators.end()) {
                return it->second.function(operand, false);
            }
            throw std::runtime_error("Unknown operator: " + root->value);
        }
        
        // binary operator
        bool leftVal = evaluateInner(root->left, variables);
        bool rightVal = evaluateInner(root->right, variables);
        
        auto it = operators.find(root->value[0]);
        if (it != operators.end()) {
            return it->second.function(leftVal, rightVal);
        }
        throw std::runtime_error("Unknown operator: " + root->value);
    }
};
