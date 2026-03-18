#include <iostream>
#include <vector>
#include <iomanip>

double calculate_collision_probability(int k, int N)
{
    if (k > N / 2)
    {
        return 1.0;
    }
    
    double prob_no_collision = 1.0;
    for (int i = 0; i < k; ++i)
    {
        prob_no_collision *= (static_cast<double>(N - k - i) / (N - i));
    }
    
    return 1.0 - prob_no_collision;
}

int find_min_k_for_collision(int N, double threshold)
{
    int left = 1;
    int right = N;
    int result = N;
    
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        double prob = calculate_collision_probability(mid, N);
        
        if (prob >= threshold)
        {
            result = mid;
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }
    
    return result;
}

int main()
{
    const int N = 1 << 16;
    const double threshold = 0.5;
    
    int min_k = find_min_k_for_collision(N, threshold);
    
    std::cout << "N = " << N << std::endl;
    std::cout << "Threshold probability = " << threshold << std::endl;
    std::cout << "Minimum k such that the sets do not intersect >= " << threshold << ": " << min_k << std::endl;
    std::cout << "Probability for k = " << min_k << ": " 
              << std::fixed << std::setprecision(6) 
              << calculate_collision_probability(min_k, N) << std::endl;
    
    std::vector<double> probabilities;
    std::vector<int> k_values;
    
    for (int k = 1; k <= 1000; k += 50)
    {
        double prob = calculate_collision_probability(k, N);
        probabilities.push_back(prob);
        k_values.push_back(k);
    }
    
    std::cout << "\nProbability growth with k:" << std::endl;
    for (size_t i = 0; i < k_values.size(); ++i)
    {
        std::cout << "k = " << std::setw(4) << k_values[i] 
                  << ", P = " << std::fixed << std::setprecision(4) 
                  << probabilities[i] << std::endl;
    }
    
    return 0;
}