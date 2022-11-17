#include "general.h"
#include "Server.cpp"

using namespace server;

int main()
{
	Server server = Server();
	server.Start();
	return 0;
}
