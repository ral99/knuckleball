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

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include "context.h"
#include "server.h"

#define VERSION "0.1.0"

int main(int argc, char* argv[]) {
    boost::program_options::options_description description("Options");
    description.add_options()
        ("help,h", "print usage message")
        ("bind", boost::program_options::value<std::string>()->default_value("0.0.0.0"), "server ip")
        ("port,p", boost::program_options::value<int>()->required(), "server port number")
        ("password", boost::program_options::value<std::string>()->default_value(""), "server password")
        ("logfile", boost::program_options::value<std::string>()->default_value(""), "log file name")
        ("floatprecision", boost::program_options::value<int>()->default_value(3),
         "decimal precision to be used to format floating-point values")
        ("floatcomparisontolerance", boost::program_options::value<float>()->default_value(0.0000001),
         "tolerance to be used to compare floating-point values");
    boost::program_options::variables_map vmap;
    try {
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, description), vmap);
        if (vmap.count("help")) {
            std::cout << description << std::endl;
            return EXIT_SUCCESS;
        }
        boost::program_options::notify(vmap);
    }
    catch(std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }
    int pid = int(getpid());
    std::cout << "Running Knuckleball " << VERSION << std::endl;
    std::cout << "PID: " << pid << std::endl;
    std::cout << std::endl;
    Context::get_instance(vmap["logfile"].as<std::string>(), vmap["floatprecision"].as<int>(),
                          vmap["floatcomparisontolerance"].as<float>());
    Server::get_instance(vmap["bind"].as<std::string>(), vmap["port"].as<int>(),
                         vmap["password"].as<std::string>())->run();
    return EXIT_FAILURE;
}
