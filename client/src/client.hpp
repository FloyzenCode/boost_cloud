#include <iostream>
#include <boost/asio.hpp>

namespace cloud
{
    class Client
    {
    private:
        std::string get_filename(const std::string &file_path);
    public:
        void send_file(boost::asio::ip::tcp::socket &socket, const std::string &file_path);
    };
};