#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

int main(int argc, char* argv[])
{
    std::string filename;
    std::vector<char> elements;
    std::vector<std::pair<char, char>> pairs;
    std::string line;
    bool reflexive = true;
    bool irreflexive = true;
    bool symmetric = true;
    bool antisymmetric = true;
    bool asymmetric = true;
    bool transitive = true;
    bool antitransitive = true;
    bool connected = true;

    if(argc < 2)
    {
        return 0;
    }
    filename = argv[1];
    std::ifstream file(filename);
    
    if (std::getline(file, line))
    {
        for (char c : line)
        {
            if (c != ' ')
            {
                elements.push_back(c);
            }
        }
    }
    
    while (std::getline(file, line))
    {
        if (line.size() >= 3)
        {
            pairs.push_back(std::make_pair(line[0], line[2]));
        }
    }

    file.close();
    
    std::set<char> elementSet(elements.begin(), elements.end());
    std::set<std::pair<char, char>> relation(pairs.begin(), pairs.end());

    if (elementSet.size() == 0 || relation.size() == 0)
    {
        printf("Invalid input file =()\n");
        return 0;
    }
    
    for (char a : elements)
    {
        if (relation.find(std::make_pair(a, a)) == relation.end())
        {
            reflexive = false;
        }
        else
        {
            irreflexive = false;
        }
    }
    
    for (char a : elements)
    {
        for (char b : elements)
        {
            bool ab = relation.find(std::make_pair(a, b)) != relation.end();
            bool ba = relation.find(std::make_pair(b, a)) != relation.end();
            
            if (ab && !ba)
            {
                symmetric = false;
            }
            
            if (ab && ba && a != b)
            {
                antisymmetric = false;
            }

            if (ab && ba)
            {
                asymmetric = false;
            }
        }
    }
    
    bool f = false;

    for (const auto& ab : relation)
    {
        char a = ab.first;
        char b = ab.second;
        
        for (char c : elements)
        {
            bool bc = relation.find(std::make_pair(c, b)) != relation.end();
            bool ac = relation.find(std::make_pair(a, c)) != relation.end();
            
            if (bc && !ac) {
                transitive = false;
            }
            
            if (bc && ac && a != c) {
                antitransitive = false;
            }
            
            if ((!transitive && !antitransitive) || transitive) {
                f = true;
                break;
            }
        }
    
        if (f) break;
    }

    for (char a : elements)
    {
        for (char b : elements)
        {
            if (a != b)
            {
                bool ab = relation.find(std::make_pair(a, b)) != relation.end();
                bool ba = relation.find(std::make_pair(b, a)) != relation.end();
                
                if (!ab && !ba)
                {
                    connected = false;
                }
            }
        }
    }
    
    std::cout 
    << "Reflexive: " << (reflexive ? "+" : "-")
    << "\nIrreflexive: " << (irreflexive ? "+" : "-")
    << "\nSymmetric: " << (symmetric ? "+" : "-")
    << "\nAntisymmetric: " << (antisymmetric ? "+" : "-")
    << "\nAsymmetric: " << (asymmetric ? "+" : "-")
    << "\nTransitive: " << (transitive ? "+" : "-")
    << "\nAtitransivity: " << (antitransitive ? "+" : "-")
    << "\nConnected: " << (connected ? "+" : "-")
    << std::endl << std::endl;

    if (reflexive && symmetric && transitive)
    {
        std::map<char, std::set<char>> classes;
        std::vector<std::set<char>> equivalenceClasses;
        
        for (char a : elements)
        {
            if (classes.find(a) == classes.end())
            {
                std::set<char> newClass;
                for (char b : elements)
                {
                    if (relation.find(std::make_pair(a, b)) != relation.end() && 
                        relation.find(std::make_pair(b, a)) != relation.end())
                    {
                        newClass.insert(b);
                        classes[b] = newClass;
                    }
                }
                equivalenceClasses.push_back(newClass);
            }
        }
        
        std::cout << "Equivalence classes:" << std::endl;
        for (size_t i = 0; i < equivalenceClasses.size(); i++)
        {
            std::cout << "Class " << i + 1 << ": ";
            for (char c : equivalenceClasses[i])
            {
                std::cout << c << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "Partition index: " << equivalenceClasses.size() << std::endl;
    }
    
    if (reflexive && antisymmetric && transitive)
    {
        std::vector<char> minimal;
        std::vector<char> maximal;
        
        for (char a : elements)
        {
            bool isMinimal = true;
            bool isMaximal = true;
            
            for (char b : elements)
            {
                if (a != b)
                {
                    if (relation.find(std::make_pair(b, a)) != relation.end())
                    {
                        isMinimal = false;
                    }
                    if (relation.find(std::make_pair(a, b)) != relation.end())
                    {
                        isMaximal = false;
                    }
                }
            }
            
            if (isMinimal)
            {
                minimal.push_back(a);
            }
            if (isMaximal)
            {
                maximal.push_back(a);
            }
        }
        
        std::cout << "Minimal elements: ";
        for (char c : minimal)
        {
            std::cout << c << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Maximal elements: ";
        for (char c : maximal)
        {
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}