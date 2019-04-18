#include "Header.h"

void performTest(std::function <int()> f)
{
    auto startTime = std::chrono::high_resolution_clock::now();
    auto res = f();
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Finished after: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() 
        << " ms. With result: " << res << std::endl;
}

void performanceCold()
{
    std::cout << "Testing cold observables" << std::endl;
    auto iterations = 100'000'000;
    volatile int j;

    for (int test = 0; test < 3; test++)
    {
        auto ints = rxcpp::observable<>::range(0, iterations);

        performTest([&ints]()
        {
            int j = 1;
            ints.subscribe([&j](const int &i)
            {
                j = 2 - j;
            });
            return j;
        });

        performTest([&iterations]()
        {
            int j = 1;
            for (auto i = 0; i <= iterations; i++)
            {
                j = 2 - j;
            }

            return j;
        });
    }
}

void performanceHot()
{
    std::cout << "Testing hot observables" << std::endl;
    auto iterations = 100'000'000;
    volatile int j;

    for (int test = 0; test < 3; test++)
    {
        auto ints = rxcpp::observable<>::range(0, iterations).publish();

        performTest([&ints]() {
            int j = 1;
            ints.subscribe_on(rxcpp::observe_on_new_thread()).subscribe([&j](const int &i)
            {
                j = 3 - j;
            });
            ints.subscribe_on(rxcpp::observe_on_new_thread()).subscribe([&j](const int &i)
            {
                j = 2 - j;
            });
            ints.connect();
            return j;
        });
    }
}
