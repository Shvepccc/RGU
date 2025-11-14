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
#include <iomanip>

struct TreeNode {
    std::string value;
    TreeNode* left;
    TreeNode* right;
    
    TreeNode(const std::string& val) : value(val), left(nullptr), right(nullptr) {}
    TreeNode(const std::string& val, TreeNode* l, TreeNode* r) : value(val), left(l), right(r) {}
};

class BooleanExpressions {
private:
    
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
    
public:
    BooleanExpressions() {}

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
    
    // TODO: change it to search in map
    bool isOpenBracket(char c) const {
        return c == '(' || c == '{' || c == '[';
    }
    
    // TODO: change it to search in map
    bool isCloseBracket(char c) const {
        return c == ')' || c == '}' || c == ']';
    }
    
    TreeNode* parse(const std::string& expression) {
        std::stack<TreeNode*> operandStack;
        std::stack<char> operatorStack;
        
        size_t i = 0;
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
                           getPrecedence(operatorStack.top()) >= getPrecedence(op)) {
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
    
    bool evaluate(TreeNode* root, const std::unordered_map<std::string, bool>& variables) const {
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
            bool operand = evaluate(root->right, variables);
            auto it = operators.find(root->value[0]);
            if (it != operators.end()) {
                return it->second.function(operand, false);
            }
            throw std::runtime_error("Unknown operator: " + root->value);
        }
        
        // binary operator
        bool leftVal = evaluate(root->left, variables);
        bool rightVal = evaluate(root->right, variables);
        
        auto it = operators.find(root->value[0]);
        if (it != operators.end()) {
            return it->second.function(leftVal, rightVal);
        }
        throw std::runtime_error("Unknown operator: " + root->value);
    }
    
    void clearTree(TreeNode* root) {
        if (root == nullptr) return;
        
        clearTree(root->left);
        clearTree(root->right);
        
        delete root;
    }
    
    //TODO: change it?
    void printTree(TreeNode* root, int depth = 0) const {
        if (root == nullptr) return;
        
        printTree(root->right, depth + 1);
        std::cout << std::string(depth * 4, ' ') << root->value << std::endl;
        printTree(root->left, depth + 1);
    }
    
private:

    std::string extractVariable(const std::string& expression, size_t& index) {
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

    void extractVariablesFromTree(TreeNode* node, std::vector<std::string>& variables) const {
        if (node == nullptr) return;
        
        if (node->left == nullptr && node->right == nullptr) {
            if (!node->value.empty() && std::isalpha(node->value[0])) {
                variables.push_back(node->value);
            }
            return;
        }
        
        extractVariablesFromTree(node->left, variables);
        extractVariablesFromTree(node->right, variables);
    }

public:
    std::vector<std::vector<bool>> generateTruthTable(TreeNode* expressionTree) const {
        std::vector<std::vector<bool>> truthTable;
        
        if (expressionTree == nullptr)
        {
            //TODO: trow exception
        }

        std::vector<std::string> sortedVars = extractVariables(expressionTree);
        std::sort(sortedVars.begin(), sortedVars.end());

        int rowCount = pow(2, sortedVars.size());
        
        std::vector<bool> header;
        truthTable.push_back(std::vector<bool>());

        for (int i = 0; i < rowCount; i++) {
            std::unordered_map<std::string, bool> currentValues;
            std::vector<bool> row;

            for (size_t j = 0; j < sortedVars.size(); j++) {
                bool value = (i >> (sortedVars.size() - 1 - j)) & 1;
                currentValues[sortedVars[j]] = value;
                row.push_back(value);
            }

            bool result = evaluate(expressionTree, currentValues);
            row.push_back(result);
            
            truthTable.push_back(row);
        }

        return truthTable;
    }

    void printTruthTable(
        std::vector<std::vector<bool>> table, 
        TreeNode* expressionTree = nullptr) const {
        
        if (table.empty()) {
            std::cout << "No variables in expression" << std::endl;
            return;
        }

        std::vector<std::string> sortedVars;
        if (expressionTree != nullptr) {
            sortedVars = extractVariables(expressionTree);
            std::sort(sortedVars.begin(), sortedVars.end());
        } else {
            for (size_t i = 0; i < table[0].size() - 1; i++) {
                sortedVars.push_back("Var" + std::to_string(i + 1));
            }
        }

        std::vector<size_t> columnWidths;
        for (const auto& var : sortedVars) {
            columnWidths.push_back(std::max(var.length(), size_t(3)));
        }
        size_t resultWidth = 6;

        std::cout << "|";
        for (size_t i = 0; i < sortedVars.size(); i++) {
            std::cout << " " << std::setw(columnWidths[i]) << sortedVars[i] << " |";
        }
        std::cout << " " << std::setw(resultWidth) << "Result" << " |" << std::endl;

        std::cout << "|";
        for (size_t i = 0; i < sortedVars.size(); i++) {
            std::cout << std::string(columnWidths[i] + 2, '-') << "|";
        }
        std::cout << std::string(resultWidth + 2, '-') << "|" << std::endl;

        for (size_t i = 1; i < table.size(); i++) {
            std::cout << "|";
            for (size_t j = 0; j < table[i].size() - 1; j++) {
                std::cout << " " << std::setw(columnWidths[j]) 
                         << (table[i][j] ? "1" : "0") << " |";
            }
            std::cout << " " << std::setw(resultWidth) 
                     << (table[i].back() ? "1" : "0") << " |" << std::endl;
        }
    }

    std::vector<std::string> extractVariables(TreeNode* expressionTree) const {
        std::vector<std::string> variables;
        extractVariablesFromTree(expressionTree, variables);

        std::unordered_set<std::string> uniqueVars(variables.begin(), variables.end());
        variables.assign(uniqueVars.begin(), uniqueVars.end());
        return variables;
    }
};




int main() {
    try {

        std::unordered_map<char, char> bracketPairs = {
            {'(', ')'}, {'{', '}'}, {'[', ']'},
            {')', '('}, {'}', '{'}, {']', '['}
        };

        BooleanExpressions bool_expr;
        bool_expr.setBrackets(bracketPairs);
        bool_expr.setOperator('+', 1, [](bool a, bool b) { return a || b; }); // OR
        bool_expr.setOperator('&', 2, [](bool a, bool b) { return a && b; }); // AND
        bool_expr.setOperator('@', 1, [](bool a, bool b) { return a != b; }); // XOR
        bool_expr.setOperator('~', 1, [](bool a, bool b) { return a == b; }); // EQUIV
        bool_expr.setOperator('>', 1, [](bool a, bool b) { return !a || b; }); // IMPLIES
        bool_expr.setOperator('|', 3, [](bool a, bool b) { return !(a && b); }); // NAND
        bool_expr.setOperator('!', 3, [](bool a, bool b) { return !(a || b); }); // NOR
        bool_expr.setOperator('-', 4, [](bool a, bool) { return !a; }, true); // NOT
        
        std::vector<std::string> expressions = {
            "(x_1 & y_2)",
            "(-x_1 | y_2)",
            "((x_1 + y_2) & z_3)",
            "(x_1 @ (y_2 > z_3))",
            "((x_1 + y_2) & [-z_3 + x_123])",
            "", 
            "{       -([{x_123} & y_2])}"
        };
        
        for (const auto& expr : expressions) {
            std::cout << "Parsing: " << expr << std::endl;
            
            TreeNode* tree = bool_expr.parse(expr);
            
            std::cout << "Expression tree:" << std::endl;
            bool_expr.printTree(tree);
            std::cout << std::endl;
            
            std::unordered_map<std::string, bool> vars = {
                {"x_1", true},
                {"x_123", true},
                {"y_2", false},
                {"z_3", true}
            };
            
            bool result = bool_expr.evaluate(tree, vars);
            std::cout << "Result: " << (result ? "true" : "false") << std::endl;
            std::cout << "-------------------" << std::endl;
            
            std::cout << "Truth Table:" << std::endl;
            auto table = bool_expr.generateTruthTable(tree);
            bool_expr.printTruthTable(table, tree);
            std::cout << std::endl;
            
            bool_expr.clearTree(tree);
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}