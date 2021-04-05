
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


// use of the boost::asio::strand class to synchronise callback handlers in a multithreaded program.
// calling io_service::run() from only one thread ensures that callback handlers cannot run concurrently.
// an alternative approach is to have a pool of threads calling io_service::run().
// However, as this allows handlers to execute concurrently, we need a method of synchronisation when handlers might be accessing a shared, thread-unsafe resource.
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

class printer {
public:
    printer(boost::asio::io_context& io)
        : strand_(io),
        timer1_(io, boost::asio::chrono::seconds(1)),
        timer2_(io, boost::asio::chrono::seconds(1)),
        count_(0) {
        timer1_.async_wait(boost::asio::bind_executor(strand_,
            boost::bind(&printer::print1, this)));

        timer2_.async_wait(boost::asio::bind_executor(strand_,
            boost::bind(&printer::print2, this)));
    }

    ~printer() {
        std::cout << "Final count is " << count_ << std::endl;
    }

    void print1() {
        if (count_ < 10) {
            std::cout << "Timer 1: " << count_ << std::endl;
            ++count_;

            timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));

            timer1_.async_wait(boost::asio::bind_executor(strand_,
                    boost::bind(&printer::print1, this)));
        }
    }

    void print2() {
        if (count_ < 10) {
            std::cout << "Timer 2: " << count_ << std::endl;
            ++count_;

            timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(1));

            timer2_.async_wait(boost::asio::bind_executor(strand_,
                    boost::bind(&printer::print2, this)));
        }
    }

private:
    boost::asio::io_context::strand strand_;
    boost::asio::steady_timer timer1_;
    boost::asio::steady_timer timer2_;
    int count_;
};

int main() {
    boost::asio::io_context io;
    printer p(io);
    boost::thread t(boost::bind(&boost::asio::io_context::run, &io));
    io.run();
    t.join();

    return 0;
}