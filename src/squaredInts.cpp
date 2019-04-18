#include "Header.h"

#include <iostream>

#ifdef COMPILE_RANGES
include <ranges>

void squaredIntsRanges()
{
    std::vector<int> ints{ 0,1,2,3,4,5 };
    auto even = [](int i) { return 0 == i % 2; };
    auto square = [](int i) { return i * i; };

    for (int i : ints | std::view::filter(even) | std::view::transform(square))
    {
        std::cout << i << ' ';
    }
}
#else
void squaredIntsRanges()
{}
#endif

void squaredIntsObservable()
{
    std::vector<int> ints{ 0, 1, 2, 3, 4, 5 };

    auto even = [](int i) { return 0 == i % 2; };
    auto square = [](int i) { return i * i; };

    auto observableInts = rxcpp::observable<>::iterate(ints);

    observableInts.filter(even)
                  .map(square)
                  .subscribe([](const int &i)
                  {
                      std::cout << i << ' ';
                  });
}
