#include "001-App/Server.h"
#include "001-App/App.h"
#include <iostream>
#include <csignal>

Server::Server(int argc, char **argv)
    : WServer(argv[0]),
        argc_(argc),
        argv_(argv)
{

}

int Server::run()
{
    setServerConfiguration(argc_, argv_, WTHTTP_CONFIGURATION);

    addEntryPoint(Wt::EntryPointType::Application, [](const Wt::WEnvironment& env) {
        return std::make_unique<App>(env);
    });
    try {
        if (start()) {
            int sig = WServer::waitForShutdown();

            std::cerr << "Shutdown (signal = " << sig << ")" << std::endl;
            stop();

            if (sig == SIGHUP)
                WServer::restart(argc_, argv_, environ);
        }
    } catch (WServer::Exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    } catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}