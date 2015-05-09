// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Client.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	//check();
	Client client("RoiBOT", "135284521");
	client.connect("http://urtela.redlynx.com:3002");
	client.run();
	return 0;
}

