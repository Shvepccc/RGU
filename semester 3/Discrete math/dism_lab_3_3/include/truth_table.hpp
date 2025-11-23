#include <cstddef>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <cctype>
#include <iomanip>

class TruthTable {
private:
    std::vector<std::string> variables;
    std::vector<std::vector<bool>> table;

public:
    TruthTable(const std::vector<std::string>& vars, const std::vector<std::vector<bool>>& truthTable) 
        : variables(vars), table(truthTable) {
    }

    TruthTable() : variables({}), table({}) {}

    std::vector<std::string> getVariables() const {
        return variables;
    }

    const std::vector<std::vector<bool>>& getTable() const {
        return table;
    }

    void printTruthTable() const {
        if (table.empty()) {
            std::cout << "No variables in expression" << std::endl;
            return;
        }

        std::vector<std::string> sortedVars = variables;
        std::sort(sortedVars.begin(), sortedVars.end());

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

        for (size_t i = 0; i < table.size(); i++) {
            std::cout << "|";
            for (size_t j = 0; j < table[i].size() - 1; j++) {
                std::cout << " " << std::setw(columnWidths[j]) 
                         << (table[i][j] ? "1" : "0") << " |";
            }
            std::cout << " " << std::setw(resultWidth) 
                     << (table[i].back() ? "1" : "0") << " |" << std::endl;
        }
        std::cout << std::endl;
    }

    bool isFectiveVariable(int varIndex) const
    {
        if (table.empty()) return true;
        
        int numVarsInTable = table[0].size() - 1;
        if (varIndex >= numVarsInTable) return true;

        for (int i = 0; i < static_cast<int>(table.size()); i++)
        {
            for (int j = i + 1; j < static_cast<int>(table.size()); j++)
            {
                if (table[i][varIndex] == table[j][varIndex]) continue;
                bool differsOnlyInVar = true;
                for (int k = 0; k < numVarsInTable; k++) {
                    if (k != varIndex && table[i][k] != table[j][k]) {
                        differsOnlyInVar = false;
                        break;
                    }
                }

                if (differsOnlyInVar 
                    && table[i][numVarsInTable] != table[j][numVarsInTable]) {
                    return false;
                }
            }
        }
        return true;
    }

    bool deleteFectiveVariables() {

        std::vector<size_t> essentialIndices;
        for (size_t i = 0; i < variables.size(); i++) {
            if (!isFectiveVariable(i)) {
                essentialIndices.push_back(i);
            }
        }

        std::vector<std::string> newVars;
        for (size_t idx : essentialIndices) {
            newVars.push_back(variables[idx]);
        }

        std::vector<std::vector<bool>> newTable;
        std::unordered_set<std::string> seenRows;
        
        for (const auto& row : table) {
            std::vector<bool> newRow;
            for (size_t idx : essentialIndices) {
                newRow.push_back(row[idx]);
            }
            newRow.push_back(row.back());
            
            std::string key;
            for (bool val : newRow) {
                key += val ? '1' : '0';
            }
            
            if (seenRows.find(key) == seenRows.end()) {
                seenRows.insert(key);
                newTable.push_back(newRow);
            }
        }
        bool res = newVars.size() < variables.size();
        this->table = newTable;
        this->variables = newVars;

        return res;
    }

    std::string getSDNF() const {
        auto variables = getVariables();
        
        if (variables.empty()) {
            if (!table.empty() && table[0].back()) {
                return "1";
            } else {
                return "0";
            }
        }
        
        if (table.empty()) {
            return "0";
        }
        
        std::vector<std::string> pieces;
        int last_el = table[0].size() - 1;
        for (const auto& row : table) {
            if (!row[last_el]) continue;
            
            std::string piece = "(";
            for (size_t i = 0; i < variables.size(); i++) {
                piece += (row[i] ? "" : "-") + variables[i];
                if (i < variables.size() - 1) {
                    piece += " & ";
                }
            }
            piece += ")";
            pieces.push_back(piece);
        }
    
        if (pieces.empty()) return "";
        std::string result;
        for (size_t i = 0; i < pieces.size(); i++) {
            result += pieces[i];
            if (i < pieces.size() - 1) {
                result += " + ";
            }
        }
        return result;
    }

    std::string getSKNF() const {
        auto variables = getVariables();
        
        if (variables.empty()) {
            if (!table.empty() && table[0].back()) {
                return "1";
            } else {
                return "0";
            }
        }
        
        if (table.empty()) {
            return "0";
        }
        
        std::vector<std::string> pieces;
        
        for (const auto& row : table) {
            if (row.back()) continue;
            
            std::vector<std::string> literals;
            for (size_t i = 0; i < variables.size(); i++) {
                literals.push_back(row[i] ? "-" + variables[i] : variables[i]);
            }
            
            std::string piece = "(";
            for (size_t j = 0; j < literals.size(); j++) {
                piece += literals[j];
                if (j < literals.size() - 1) {
                    piece += " + ";
                }
            }
            piece += ")";
            pieces.push_back(piece);
        }
        
        if (pieces.empty()) return "";
        
        std::string result;
        for (size_t i = 0; i < pieces.size(); i++) {
            result += pieces[i];
            if (i < pieces.size() - 1) {
                result += " & ";
            }
        }
        
        return result;
    }

    TruthTable getDualFunction() const {
        auto variables = getVariables();
        
        if (table.empty() && variables.empty()) {
            return TruthTable();
        }
        
        std::vector<std::vector<bool>> dualTable;

        for (int i = 0; i < static_cast<int>(table.size()); i++)
        {
            std::vector<bool> dualRow = table[i];
            std::transform(dualRow.begin(), dualRow.end(), dualRow.begin(), 
                [](bool b) { return !b; });
            dualTable.push_back(dualRow);
        }
        
        return TruthTable(variables, dualTable);
    }

    std::string getANF() const {
        auto vars = getVariables();
        
        if (table.empty()) return "0";
        const int n = vars.size();
        const int N = 1 << n;
        
        std::vector<int> f(N);
        for (int i = 0; i < N; i++) {
            f[i] = table[i].back() ? 1 : 0;
        }
        
        for (int k = 1; k < N; k <<= 1) {
            for (int i = 0; i < N; i += (k << 1)) {
                for (int j = 0; j < k; j++) {
                    f[i + j + k] ^= f[i + j];
                }
            }
        }
        
        std::vector<std::string> terms;
        for (int i = 0; i < N; i++) {
            if (f[i] == 0) continue;
            
            if (i == 0) {
                terms.push_back("1");
                continue;
            }
            
            std::string term = "(";
            for (int j = 0; j < n; j++) {
                if (i & (1 << (n - 1 - j))) {
                    if (term != "(") term += " & ";
                    term += vars[j];
                }
            }
            term += ")";
            terms.push_back(term);
        }
        
        if (terms.empty()) return "0";
        std::string result;
        for (int i = 0; i < static_cast<int>(terms.size()); i++) {
            result += terms[i];
            if (i < static_cast<int>(terms.size()) - 1) {
                result += " @ ";
            }
        }
        
        return result;
    }
};