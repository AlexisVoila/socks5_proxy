#include "include/server.h"
#include "logger/logger.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

auto parse_command_line_arguments(int argc, char* argv[]) {
    std::string listen_port;
    std::string log_file_path;

    po::options_description desc("All options");
    desc.add_options()
        ("port,p", po::value<std::string>(&listen_port)->default_value("8080")->required(), "socks5 server listen port number")
        ("log_level,v", po::value<std::string>()->default_value("info"),
         "verbosity level of log messages [debug|trace|info|warning|error|fatal]")
        ("log_file,l", po::value<std::string>(&log_file_path), "log file path")
        ("help,h", "show help message");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    if (vm.count("help")) {
        std::cout << desc << "\n";
        exit(EXIT_SUCCESS);
    }

    try {
        po::notify(vm);
    } catch (const po::required_option& e) {
        std::cout << "Error: " << e.what() << "\n";
        exit(EXIT_FAILURE);
    }

    using logger = logging::logger;
    logger::level level;
    if (vm.count("log_level")) {
        const auto& log_level = vm["log_level"].as<std::string>();
        if (log_level == "debug")
            level = logger::level::debug;
        else if (log_level == "trace")
            level = logger::level::trace;
        else if (log_level == "info")
            level = logger::level::info;
        else if (log_level == "warning")
            level = logger::level::warning;
        else if (log_level == "error")
            level = logger::level::error;
        else if (log_level == "fatal")
            level = logger::level::fatal;
        else {
            std::cout << "Error: log_level value must be one of [debug|trace|info|warning|error|fatal]\n";
            exit(EXIT_FAILURE);
        }
    }

    return std::tuple<std::string, std::string, logger::level>{listen_port, log_file_path, level};
}

int main(int argc, char* argv[]) {
    auto [listen_port, log_file_path, level] = parse_command_line_arguments(argc, argv);

    using logger = logging::logger;
    logger::output log_output = log_file_path.empty()
        ? logger::output::console
        : logger::output::file_and_console;

    logger::initialize(log_file_path, log_output, level);

    try {
        std::cout << "listen_port: " << listen_port << "\n";
        server srv(listen_port);
        srv.run();
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}
