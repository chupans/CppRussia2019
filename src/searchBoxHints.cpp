#include <rxcpp/rx.hpp>
#include "Header.h"
#include <string>
#include <iostream>
#include <condition_variable>
#include <future>

namespace rx = rxcpp;

class GuiWindow
{
public:

    GuiWindow(std::string &&userInput) : userInput_(userInput)
    {
        //Imitating user input
        auto observable = rx::observable<>::interval(std::chrono::milliseconds(std::rand() % 100 + 260));
        symbolsStream_ = observable.take(userInput_.size()).map([this](long i)
        {
            if (i <= userInput_.size())
            {
                return userInput_[i - 1];
            }
            else
            {
                throw std::logic_error("Shouldn't be happening");
            }
        });
    }
    rxcpp::observable<char> symbolsStream_;
private:
    std::string userInput_ = "";
};

class NetClient
{
public:
    static std::future<std::string> getHintFor(const std::string &text)
    {
        return std::async(std::launch::async, [text]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 500 + 100));
            return "Hint for: " + text;
        });
    }

};

class Logger
{
public:
    static void log(const std::string &s)
    {
        std::cout << "Log entry: " << s << std::endl;
    }
};

template <class T>
rxcpp::observable<T> fromFuture(std::future<T> &&fut)
{
    return rx::observable<>::create<T>([res = fut.share()](rx::subscriber<T> dest)
    {
        auto val = res.get();
        dest.on_next(val);

        dest.on_completed();
    });
}

void searchBoxHints()
{
    using namespace std::chrono_literals;
    using std::string;

    GuiWindow textEdit(string("Reactive programming"));
    auto symbols = textEdit.symbolsStream_;

    auto strings = symbols.scan(string{}, [](const string &seed, const char symbol)
    {
        return seed + symbol;
    }).debounce(300ms);

    auto allHints = strings.map([](const string &input)
    {
        auto observable = fromFuture<string>(NetClient::getHintFor(input));

        return observable.observe_on(rxcpp::observe_on_new_thread());
    });

    auto hints = allHints.switch_on_next().publish();

    hints.subscribe([](const string &str)
    {
        std::cout << str << std::endl;
    });

    hints.subscribe(Logger::log);

    hints.connect();
}