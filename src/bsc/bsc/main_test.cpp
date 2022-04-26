
#include <context/context/Context.h>
#include <log/log/Logger.h>
#include <log/log/LoggerContext.h>
#include <tuple>
#include "date.h"
struct A {
    int a=5;
    int* b = &a;
};

void printA(A& a) {
    std::cout << fmt::format("A is {} and b is {}", a.a, *(a.b)) << std::endl;
}


int main(int argc, char* argv[]) {

auto format = "%Y";
std::chrono::time_point<std::chrono::system_clock, std::chrono::years > timePoint;
std::stringstream ss{"2022"};
ss >> date::parse(format, timePoint);
if (ss.good()) {
    std::cout << "yay";
} else {
    std::cout << "nay";
}
}
