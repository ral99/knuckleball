/*
Copyright (c) 2016, Rodrigo Alves Lima
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the
       following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
       following disclaimer in the documentation and/or other materials provided with the distribution.

    3. Neither the name of Knuckleball nor the names of its contributors may be used to endorse or promote products
       derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include <string>
#include <vector>

#include <boost/asio.hpp>

class Server {
private:
    // Singleton instance:
    static Server *_instance;

    // Attributes:
    std::string _password;
    boost::asio::io_service _io_service;
    boost::asio::ip::tcp::endpoint _endpoint;
    boost::asio::ip::tcp::acceptor _acceptor;
    boost::asio::ip::tcp::socket _socket;

    // Constructor:
    Server(const std::string& ip, int port, const std::string& password);

    // Asynchronously accept a new connection.
    void do_accept();
    void handle_accept();

    // Delete copy constructor and assignment operator.
    Server(const Server& other) = delete;
    Server& operator=(const Server& other) = delete;
public:
    // Singleton getter:
    static Server* get_instance();
    static Server* get_instance(const std::string& ip, int port, const std::string& password);

    // Destructor:
    ~Server();

    // Getter methods:
    std::string get_password();

    // Loop:
    void run();
};

class Session : public std::enable_shared_from_this<Session> {
private:
    // Attributes:
    bool _is_connected;
    bool _is_authenticated;
    boost::asio::streambuf _streambuf;
    boost::asio::ip::tcp::socket _socket;

    // Delete copy constructor and assignment operator.
    Session(const Session& other) = delete;
    Session& operator=(const Session& other) = delete;
public:
    // Constructor:
    Session(boost::asio::ip::tcp::socket socket);

    // Receive message and return a string.
    std::string receive(const std::string& message_name, const std::vector<std::string>& arguments);

    // Asynchronously read input from socket and process it.
    void do_read();

    // Asynchronously write response to socket.
    void do_write(const std::string& response);
};

#endif
