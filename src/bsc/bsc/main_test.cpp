
#include <context/context/Context.h>
#include <log/log/Logger.h>
#include <log/log/LoggerContext.h>


class Finisher {
public:
    Finisher() {
        std::cout << "Finisher c" << std::endl;
    }

    ~Finisher() {
        std::cout << "Finisher d" << std::endl;
    }
};


class Base {
public:
    Base(const Finisher& f) {
        std::cout << "base c" << std::endl;
    }

    ~Base() {
        std::cout << "base d" << std::endl;
    }
};

class Derived : public Base {
public:
    Derived() : Base({}) {
        std::cout << "Derived c" << std::endl;
    }

    ~Derived() {
        std::cout << "Derived d" << std::endl;
    }
};



int main(int argc, char* argv[]) {
    Derived d;
//    auto nodeContext = bsc::Context::makeContext();
//    bsc::Context::setActiveContext(nodeContext);
//    bsc::logger.info("Test log");
//    nodeContext->get<bsc::LoggerContext>()->setInstanceFetcher([&] { return "lalal"; });
//    bsc::logger.info("Test log");
}
