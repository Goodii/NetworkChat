#include "Client.h"

int main() {
	
	auto app = new Client();

	app->startup();
	
	delete app;

	return 0;
}