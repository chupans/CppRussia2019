#include "Header.h"

#include <string>
#include <iostream>


void helloWorld()
{
    std::vector<std::string> words = { "Hello", ", ", "world", "!" };

    auto strings = rxcpp::observable<>::iterate(words);

    strings.subscribe([](const std::string &str)
    {
        std::cout << str;
    });
}