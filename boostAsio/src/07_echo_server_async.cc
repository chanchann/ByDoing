// Asynchronous echo server.

#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include "boost/asio.hpp"
#include "boost/core/ignore_unused.hpp"

using boost::asio::ip::tcp;

// -----------------------------------------------------------------------------

#define USE_BIND 1  // Use std::bind or lambda

enum { BUF_SIZE = 1024 };

// -----------------------------------------------------------------------------

class Session : public std::enable_shared_from_this<Session> {
public:
	Session(tcp::socket socket) 
		: socket_(std::move(socket)) {
	}

	void Start() {
		DoRead();
	}

private:
  	void DoRead() {
#if USE_BIND
    socket_.async_read_some(boost::asio::buffer(buffer_),
                            std::bind(&Session::OnRead, shared_from_this(),
                                      std::placeholders::_1,
                                      std::placeholders::_2));
#else
    auto self(shared_from_this());

    socket_.async_read_some(
        boost::asio::buffer(buffer_),
        [this, self](boost::system::error_code ec, std::size_t length) {
			if (!ec) {
				AsyncWrite(length);
			}
        });
#endif  // USE_BIND
  }

  	void DoWrite(std::size_t length) {
#if USE_BIND
		boost::asio::async_write(socket_,
								boost::asio::buffer(buffer_, length),
								std::bind(&Session::OnWrite, shared_from_this(),
										std::placeholders::_1,
										std::placeholders::_2));
#else
		auto self(shared_from_this());

		boost::asio::async_write(
			socket_,
			boost::asio::buffer(buffer_, length),
			[this, self](boost::system::error_code ec, std::size_t /*length*/) {
				if (!ec) {
					AsyncRead();
				}
			});
#endif  // USE_BIND
  	}

#if USE_BIND
  	void OnRead(boost::system::error_code ec, std::size_t length) {
		if (!ec) {
			DoWrite(length);
		} else {
			if (ec == boost::asio::error::eof) {
				std::cerr << "Socket read EOF: " << ec.message() << std::endl;
			} else if (ec == boost::asio::error::operation_aborted) {
				// The socket of this connection has been closed.
				// This happens, e.g., when the server was stopped by a signal (Ctrl-C).
				std::cerr << "Socket operation aborted: " << ec.message() << std::endl;
			} else {
				std::cerr << "Socket read error: " << ec.message() << std::endl;
			}
		}
	}

	void OnWrite(boost::system::error_code ec, std::size_t length) {
		boost::ignore_unused(length);
		if (!ec) {
			DoRead();
		}
	}
#endif  // USE_BIND
	// 只要 Session 对象在，socket 就在，连接就不断。Socket 对象是构造时传进来的，而且是通过 move 语义转移进来的。
	tcp::socket socket_;    // Client 通信，
	std::array<char, BUF_SIZE> buffer_;   // 接收 Client 数据的缓存
};

// -----------------------------------------------------------------------------

class Server {
public:
	Server(boost::asio::io_context& io_context, std::uint16_t port)
		: acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
		DoAccept();
	}

private:
	void DoAccept() {
		acceptor_.async_accept(
			[this](boost::system::error_code ec, tcp::socket socket) {
				if (!ec) {
					std::make_shared<Session>(std::move(socket))->Start();
				}
				DoAccept();
			});
	}

	tcp::acceptor acceptor_;
};

// -----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
		return 1;
	}

	std::uint16_t port = std::atoi(argv[1]);

	boost::asio::io_context io_context;

	Server server{ io_context, port };

	io_context.run();

	return 0;
}