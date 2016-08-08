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

#include <iostream>
#include <utility>

#include "context.h"
#include "exceptions.h"
#include "grammar.h"
#include "instance.h"
#include "server.h"
#include "str_utils.h"

//////////////////////////////////////////////////////// Server ////////////////////////////////////////////////////////

// Initialize the singleton instance.
Server* Server::_instance = NULL;

Server* Server::get_instance() {
    return _instance;
}

Server* Server::get_instance(const std::string& ip, int port, const std::string& password) {
    delete _instance;
    _instance = new Server(ip, port, password);
    return _instance;
}

Server::Server(const std::string& ip, int port, const std::string& password) :
    _password(password),
    _endpoint(boost::asio::ip::address::from_string(ip), port),
    _acceptor(_io_service, _endpoint),
    _socket(_io_service) {
}

Server::~Server() {
    _instance = NULL;
}

std::string Server::get_password() {
    return _password;
}

void Server::do_accept() {
    _acceptor.async_accept(_socket,
            [this](const boost::system::error_code& error_code) {
                if (!error_code)
                    handle_accept();
            }
    );
}

void Server::handle_accept() {
    std::make_shared<Session>(std::move(_socket))->do_read();
    do_accept();
}

void Server::run() {
    do_accept();
    _io_service.run();
}

/////////////////////////////////////////////////////// Session ////////////////////////////////////////////////////////

Session::Session(boost::asio::ip::tcp::socket socket) : _is_connected(true), _socket(std::move(socket)) {
    _is_authenticated = (Server::get_instance()->get_password() == "");
}

std::string Session::receive(const std::string& message_name, const std::vector<std::string>& arguments) {
    if (message_name == "close") {
        _is_connected = false;
        return "null";
    }
    if (message_name == "authenticateWithPassword:") {
        if (arguments.size() != 1)
            throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
        if (!Grammar::is_string_value(arguments[0]))
            throw EXC_INVALID_ARGUMENT;
        if (StringInstance(arguments[0]).value() == Server::get_instance()->get_password()) {
            _is_authenticated = true;
            return "null";
        }
        throw EXC_WRONG_PASSWORD;
    }
    throw EXC_INVALID_MESSAGE;
}

void Session::do_read() {
    auto self(shared_from_this());
    boost::asio::async_read_until(_socket, _streambuf, '\n',
            [this, self](const boost::system::error_code& error_code, std::size_t bytes_transferred) {
                if (!error_code) {
                    std::istream istream(&_streambuf);
                    std::string input;
                    std::getline(istream, input);
                    std::string actor;
                    try {
                        Parser parser(input);
                        actor = parser.actor();
                    }
                    catch (...) {
                    }
                    if (_is_authenticated || Grammar::is_connection(actor)) {
                        std::string output = Context::get_instance()->execute(input, self);
                        if (_is_connected)
                            do_write(output);
                    }
                    else
                        do_write(EXC_NOT_AUTHENTICATED);
                }
            }
    );
}

void Session::do_write(const std::string& response) {
    auto self(shared_from_this());
    boost::asio::async_write(_socket, boost::asio::buffer(response + "\n"),
            [this, self](const boost::system::error_code& error_code, std::size_t bytes_transferred) {
                if (!error_code)
                    do_read();
            }
    );
}
