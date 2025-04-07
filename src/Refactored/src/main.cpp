#include "GameLogic.h"

int main(int argc, char *argv[]) {
	if (argc < 5) {
		std::cout << "Invalid Parameters\n";
		return 0;
	}

	// NOTE: "m" = Manual mode
    //       "s" = Play against self
    //       "t" = Tournament mode

	int col = atoi(argv[1]);
	int row = atoi(argv[2]);
	int p = atoi(argv[3]);
	std::string mode = argv[4];

	int order = 0;
    if (mode == "m" || mode == "manual"|| mode == "s"|| mode == "self") {
        order = atoi(argv[5]);
    }

	GameLogic main(col,row,p, mode, order);//col,row,p,g,mode,debug
	main.Run();

	return 0;
}
