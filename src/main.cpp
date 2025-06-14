// #include "./001-App/App.h"
// #include <Wt/WServer.h>
#include "001-App/Server.h"

int main(int argc, char **argv)
{
    std::cout << "Program arguments:" << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
    }
    std::cout << "Starting Wt server..." << std::endl;

    Server server(argc, argv);
    return server.run();

    // return Wt::WRun(argc, argv, [](const Wt::WEnvironment& env) {
    //     return std::make_unique<App>(env);
    // });

    // try
    // {

    //     Wt::WServer server(argv[0]);
        
    //     server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
    //     server.addEntryPoint(
    //         Wt::EntryPointType::Application,
    //         [=](const Wt::WEnvironment &env)
    //         {
    //             return std::make_unique<App>(env);
    //         },
    //         "/");
    //     // Session::configureAuth();

    //     server.run();
    // }
    // catch (Wt::WServer::Exception &e)
    // {
    //     std::cerr << e.what() << "\n";
    //     return 1;
    // }
    // catch (std::exception &e)
    // {
    //     std::cerr << "exception: " << e.what() << "\n";
    //     return 1;
    // }
}
