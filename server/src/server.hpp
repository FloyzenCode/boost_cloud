#include <iostream>
#include <boost/asio.hpp>

namespace server
{
    class Server
    {
    public:
        void handle_client(boost::asio::ip::tcp::socket &socket);
    };
};