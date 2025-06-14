#include "001-App/Server.h"

int main(int argc, char **argv)
{
    std::cout << "Program arguments:" << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
    }
    std::cout << "Starting Wt server..." << std::endl;

    Server server(argc, argv);

}
