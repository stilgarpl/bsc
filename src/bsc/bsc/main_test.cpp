
#include <context/context/Context.h>
#include <log/log/Logger.h>
#include <log/log/LoggerContext.h>
#include <tuple>

struct A {
    int a=5;
    int* b = &a;
};

void printA(A& a) {
    std::cout << fmt::format("A is {} and b is {}", a.a, *(a.b)) << std::endl;
}


int main(int argc, char* argv[]) {

A a;
printA(a);
a.a = 77;
printA(a);
//    auto nodeContext = bsc::Context::makeContext();
//    bsc::Context::setActiveContext(nodeContext);
//    bsc::logger.info("Test log");
//    nodeContext->get<bsc::LoggerContext>()->setInstanceFetcher([&] { return "lalal"; });
//    bsc::logger.info("Test log");
}
