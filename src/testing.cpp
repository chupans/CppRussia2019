#include "Header.h"
#include <rxcpp/rx-test.hpp>
#include <chrono>

using namespace rxcpp;

void testing()
{
    using namespace std::chrono_literals;
    
    auto testScheduler = schedulers::make_test();
    auto worker = testScheduler.create_worker();
    auto test = identity_same_worker(worker);

    observable<>::interval(500ms, test)
        .delay(500ms, test)
        .subscribe([](int i)
    {
        std::cout << i << std::endl;
    });

    worker.advance_by((10s).count() * 1000);
}
