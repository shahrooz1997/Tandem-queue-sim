#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include "Event_stack.h"
#include "Job.h"

using namespace std;
using namespace chrono;

int main() {
    Event_stack event_stack(1, .8, .9);
    event_stack.start();
    for(auto it = event_stack.get_finished_jobs().get_jobs_p().begin();
            it != event_stack.get_finished_jobs().get_jobs_p().end(); it++){
        cout << *(*it);
    }
    std::cout << event_stack.get_finished_jobs().get_jobs_p().size() << std::endl;;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
