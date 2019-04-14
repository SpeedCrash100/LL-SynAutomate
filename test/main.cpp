#include "testautomate.hpp"
#include <sstream>

int main(int argc, char** argv)
{
    TestAutomate automat;
    automat.Init();
    for (int i = 1; i < argc; i++) {
        std::string str = argv[i];
        std::stringstream ss(str);
        automat.Analyze(ss);
    }
    return 0;
}
