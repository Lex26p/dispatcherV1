#include <cstdlib>
#include <iostream>
#include <string_view>

namespace dispatcher
{
    constexpr std::string_view ApplicationName = "Dispatcher";
    constexpr std::string_view ServerName = "dispatcher_server";
    constexpr std::string_view Version = "0.1.0-dev";
    constexpr std::string_view Mode = "Development";
}

int main()
{
    std::cout << dispatcher::ApplicationName << " server" << '\n';
    std::cout << "Executable: " << dispatcher::ServerName << '\n';
    std::cout << "Version: " << dispatcher::Version << '\n';
    std::cout << "Mode: " << dispatcher::Mode << '\n';
    std::cout << "Status: starting" << '\n';

    return EXIT_SUCCESS;
}