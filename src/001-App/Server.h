#pragma once
#include <Wt/WServer.h>

class Server : public Wt::WServer
{
    public:
        Server(int argc, char **argv);
        int run();
    private:
    int argc_;
    char **argv_;
};