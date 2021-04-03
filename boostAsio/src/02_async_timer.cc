#include <iostream>
#include <boost/asio.hpp>

void print(const boost::system::error_code& /*e*/) {
    std::cout << "Hello, world!" << std::endl;
}

int main() {
    boost::asio::io_context io;

    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
    // Using asio's asynchronous functionality means having a callback function that will be called when an asynchronous operation completes
    // we define a function called print to be called when the asynchronous wait finishes.
    t.async_wait(&print);
    // So we can print test here，but not block to wait

    io.run();
    // callback handlers will only be called from threads that are currently calling io_service::run().
    // It is important to remember to give the io_service some work to do before calling io_service::run().

    return 0;
}