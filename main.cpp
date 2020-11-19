#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include "Event_stack.h"
#include "Job.h"

using namespace std;
using namespace chrono;

std::chrono::microseconds get_exp_rv_value(double poisson_rate){
    static std::default_random_engine e(std::chrono::system_clock::now().time_since_epoch().count());
    std::exponential_distribution<double> exp_dist(1. / poisson_rate);

    return std::chrono::microseconds(static_cast<int64_t>(exp_dist(e) * 1.0e6));
}

std::chrono::microseconds get_exp_rv_value(std::chrono::microseconds mean_time){
    static std::default_random_engine e(std::chrono::system_clock::now().time_since_epoch().count());
    std::exponential_distribution<double> exp_dist(mean_time.count() / 1.0e6);

    return std::chrono::microseconds(static_cast<int64_t>(exp_dist(e) * 1.0e6));
}

// Todo: produce job in poisson arrivals
//Job get_job(double poisson_lambda){
//    // Wait exponential amount of time
//    this_thread::sleep_for(get_exp_rv_value(poisson_lambda));
//
//    // Create job
//    Job ret(steady_clock::now());
//
//    return ret;
//}



int main() {
    Event_stack event_stack(1, 10, 10);
    event_stack.start();
    for(auto it = event_stack.get_finished_jobs().get_jobs_p().begin();
            it != event_stack.get_finished_jobs().get_jobs_p().end(); it++){
        cout << *(*it);
    }
    std::cout << event_stack.get_finished_jobs().get_jobs_p().size() << std::endl;;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
