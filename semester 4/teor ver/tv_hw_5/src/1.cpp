#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>

int main()
{
    const double lambda = 1.94;
    const int m = 4;
    const int max_children = 15;
    const int num_families = 1000000;

    std::vector<double> prob_k(max_children + 1);
    double sum_probs = 0.0;
    for (int k = 0; k <= max_children; ++k)
    {
        prob_k[k] = std::exp(-lambda) * std::pow(lambda, k);
        double fact = 1.0;
        for (int i = 1; i <= k; ++i)
        {
            fact *= i;
        }
        prob_k[k] /= fact;
        sum_probs += prob_k[k];
    }

    for (int k = 0; k <= max_children; ++k)
    {
        prob_k[k] /= sum_probs;
    }

    double unconditional_prob = 0.0;
    for (int k = m; k <= max_children; ++k)
    {
        unconditional_prob += prob_k[k];
    }

    double A = 0.0; // event: at least m children in the family
    double B = 0.0; // event: no girls in family 
    for (int k = 0; k <= max_children; ++k)
    {
        double prob_no_girls = std::pow(0.5, k);
        if (k >= m)
        {
            A += prob_k[k] * prob_no_girls;
        }
        B += prob_k[k] * prob_no_girls;
    }
    if (B > 0)
    {
        A /= B;
    }



    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    int families_with_no_girls = 0;
    int families_with_no_girls_and_m_plus = 0;

    for (int i = 0; i < num_families; ++i)
    {
        double r = dis(gen);
        int num_children = 0;
        double g = 0.0;
        for (int k = 0; k <= max_children; ++k)
        {
            g += prob_k[k];
            if (r < g)
            {
                num_children = k;
                break;
            }
        }

        bool all_boys = true;
        for (int j = 0; j < num_children; ++j)
        {
            if (dis(gen) < 0.5)
            {
                all_boys = false;
                break;
            }
        }

        if (all_boys && num_children > 0)
        {
            families_with_no_girls++;
            if (num_children >= m)
            {
                families_with_no_girls_and_m_plus++;
            }
        }
        else if (num_children == 0 && all_boys)
        {
            families_with_no_girls++;
            if (0 >= m)
            {
                families_with_no_girls_and_m_plus++;
            }
        }
    }

    double conditional_prob_empirical = 0.0;
    if (families_with_no_girls > 0)
    {
        conditional_prob_empirical = static_cast<double>(families_with_no_girls_and_m_plus) / families_with_no_girls;
    }

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Unconditional probability P(children >= " << m << "): " 
        << unconditional_prob << std::endl;
    std::cout << "Conditional probability (theoretical) P(children >= " << m << " | no girls): " 
        << A << std::endl;
    std::cout << "Conditional probability (empirical) P(children >= " << m << " | no girls): " 
        << conditional_prob_empirical << std::endl;
    std::cout << "Difference (theoretical unconditional vs conditional): " 
        << std::abs(unconditional_prob - A) << std::endl;
    std::cout << "Difference (empirical unconditional vs conditional): " 
        << std::abs(unconditional_prob - conditional_prob_empirical) << std::endl;

    return 0;
}