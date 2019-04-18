#include "Header.h"

static std::function<void(int)> print = [](const int &i)
{
    std::cout << i << std::endl;
};

void hotCold()
{
    auto randoms = rxcpp::observable<>::create<int>([](rxcpp::subscriber<int> dest)
    {
        dest.on_next(std::rand() % 100);
        dest.on_completed();
    });

    randoms.subscribe(print);
    randoms.subscribe(print);
}

void hotColdPublished()
{
    auto randoms = rxcpp::observable<>::create<int>([](rxcpp::subscriber<int> dest)
    {
        dest.on_next(std::rand() % 100);
        dest.on_completed();
    }).publish();

    randoms.subscribe(print);
    randoms.subscribe(print);

    randoms.connect();
}
