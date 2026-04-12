#ifndef TERM_ORDER_H_
#define TERM_ORDER_H_

#include <vector>
#include <numeric>

enum class term_order_type
{
    LEX,
    GRLEX,
    GREVLEX,
    INVLEX,
    RINVLEX
};

class term_order
{
private:
    term_order_type type;
    
    static int totalDegree(const std::vector<int>& exp)
    {
        return std::accumulate(exp.begin(), exp.end(), 0);
    }
    
    static bool lexCompare(const std::vector<int>& a, const std::vector<int>& b)
    {
        for (size_t i = 0; i < a.size(); ++i)
        {
            if (a[i] != b[i])
            {
                return a[i] > b[i];
            }
        }
        return false;
    }
    
    static bool grlexCompare(const std::vector<int>& a, const std::vector<int>& b)
    {
        int degA = totalDegree(a);
        int degB = totalDegree(b);
        if (degA != degB)
        {
            return degA > degB;
        }
        return lexCompare(a, b);
    }
    
    static bool grevlexCompare(const std::vector<int>& a, const std::vector<int>& b)
    {
        int degA = totalDegree(a);
        int degB = totalDegree(b);
        if (degA != degB)
        {
            return degA > degB;
        }
        for (size_t i = a.size(); i > 0; --i)
        {
            if (a[i-1] != b[i-1])
            {
                return a[i-1] < b[i-1];
            }
        }
        return false;
    }
    
    static bool invlexCompare(const std::vector<int>& a, const std::vector<int>& b)
    {
        for (size_t i = a.size(); i > 0; --i)
        {
            if (a[i-1] != b[i-1])
            {
                return a[i-1] > b[i-1];
            }
        }
        return false;
    }
    
    static bool rinvlexCompare(const std::vector<int>& a, const std::vector<int>& b)
    {
        int degA = totalDegree(a);
        int degB = totalDegree(b);
        if (degA != degB)
        {
            return degA > degB;
        }
        return invlexCompare(a, b);
    }
    
public:
    term_order(term_order_type t = term_order_type::LEX)
        : type(t)
    {
    }
    
    bool compare(const std::vector<int>& a, const std::vector<int>& b, 
        term_order_type t = term_order_type{}) const
    {
        switch ((t == term_order_type{}) ? type : t)
        {
            case term_order_type::LEX:
                return lexCompare(a, b);
            case term_order_type::GRLEX:
                return grlexCompare(a, b);
            case term_order_type::GREVLEX:
                return grevlexCompare(a, b);
            case term_order_type::INVLEX:
                return invlexCompare(a, b);
            case term_order_type::RINVLEX:
                return rinvlexCompare(a, b);
            default:
                return lexCompare(a, b);
        }
    }
    
    term_order_type getType() const
    {
        return type;
    }
};


#endif