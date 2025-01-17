// Copyright 2019 Victor Barbu

#include "microloop.h"
#include "net/tcp_server.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

using namespace microloop;

void on_conn(net::TcpServer::PeerConnection &conn)
{
  std::cout << "Received connection\n";
}

void on_data(net::TcpServer::PeerConnection &conn, const microloop::Buffer &buf)
{
  if (buf.empty())
  {
    conn.close();
    return;
  }

  std::cout << "Received: " << static_cast<char *>(buf.data()) << "\n";

  conn.send("goodbye.\n");
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "usage: " << argv[0] << " <port>\n";
    return -1;
  }

  auto port = std::stoi(argv[1]);
  if (port < 1 || port > std::numeric_limits<std::uint16_t>::max())
  {
    throw std::range_error("invalid port");
  }

  auto event_loop = microloop::EventLoop::get_main();

  auto tcp_server = microloop::net::TcpServer{static_cast<uint16_t>(port)};
  tcp_server.set_connection_callback(on_conn);
  tcp_server.set_data_callback(on_data);

  using microloop::event_sources::TimerController;

  using namespace std::literals::chrono_literals;
  microloop::timers::set_interval(6s, event_loop, [](TimerController &timer) {
    std::cout << "Interval is done.\n";

    auto elapsed = timer.get_elapsed_time();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    std::cout << "Time elapsed: " << ms << "ms.\n";

    if (timer.get_expirations_count() == 2)
    {
      timer.set_value(std::chrono::seconds{1});
    }

    if (timer.get_expirations_count() == 10)
    {
      timer.cancel();
    }
  });

  while (true)
  {
    MICROLOOP_TICK();
  }
}
