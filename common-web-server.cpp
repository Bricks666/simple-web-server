#include "general.h"
#include "Server.h"

using namespace server;

int main()
{
	Server server = Server();
	server.start();
	return 0;
}