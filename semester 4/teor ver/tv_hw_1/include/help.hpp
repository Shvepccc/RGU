#include <ctime>
#include <cstdlib>

#define NUM_EXPERIMENTS 10000

inline int getRandomNumber(int a, int b)
{
    static bool initialized = false;
    if (!initialized)
    {
        srand(time(nullptr));
        initialized = true;
    }
    return a + rand() % (b - a + 1);
}