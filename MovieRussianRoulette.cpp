// 53616c7465645f5f4ad3fc5e8fcde55c182286f0a530a2e3d25585c79a9d2887d159d8fff2b6a43ac9bab9a76c5729e0

#include "MovieRussianRoulette.h"
#include "DbClient.hpp"

using namespace std;

int main()
{
	try {
		Cli cli = Cli();
		cli.run();
	} catch (const std::exception& e) {
		std::cerr << "Fatal error: " << e.what() << std::endl;
		std::cerr << "Press Enter to exit..." << std::endl;
		std::cin.get();
		return 1;
	}

	return 0;
}
