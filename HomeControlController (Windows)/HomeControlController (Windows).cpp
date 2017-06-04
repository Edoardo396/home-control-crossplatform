// HomeControlController (Windows).cpp : Defines the entry point for the console application.
//

#include "../HomeControlController (Windows)/stdafx.h"
#include "../SharedLibrary/MyServerApplication.h"

int main(int argc, char** argv) {
	MyServerApplication app;
	return app.run(argc, argv);
}

