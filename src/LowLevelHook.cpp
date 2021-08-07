#include "Listener.h"
#include <thread>


int main() {
    std::thread kbHook(Listener::setupKeyboardHook);
    std::thread mHook(Listener::setupMouseHook);
}
