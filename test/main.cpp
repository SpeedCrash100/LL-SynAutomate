#include "automate.hpp"
#include <sstream>

int main(int argc, char** argv)
{
    Automate automat;
    for (int i = 1; i < argc; i++) {
		std::string str = argv[i];
		std::stringstream ss(str);
        automat.Analyze(ss);
    }
    return 0;
}
