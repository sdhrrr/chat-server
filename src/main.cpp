#include "server.h"

int main(int argc , char** argv)
{
	if(argc > 1){
		server myserver(argv[1],8000);
		myserver.run();
		return 0;
	}

}
