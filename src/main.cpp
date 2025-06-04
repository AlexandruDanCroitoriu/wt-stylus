#include "./001-App/App.h"
#include <Wt/WServer.h>


int main(int argc, char **argv)
{

    try
    {

        Wt::WServer server(argv[0]);
        
        server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
        server.addEntryPoint(
            Wt::EntryPointType::Application,
            [=](const Wt::WEnvironment &env)
            {
                return std::make_unique<App>(env);
            },
            "/");
        // Session::configureAuth();

        server.run();
    }
    catch (Wt::WServer::Exception &e)
    {
        std::cerr << e.what() << "\n";
        return 1;
    }
    catch (std::exception &e)
    {
        std::cerr << "exception: " << e.what() << "\n";
        return 1;
    }
}
