#include <iostream>
#include <boost/asio.hpp>

int main() {
    // All programs that use asio need to have at least one io_context object
    // This class provides access to I/O functionality.
    boost::asio::io_context io;

    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
    t.wait();

    std::cout << "Hello, world!" << std::endl;

    return 0;
}