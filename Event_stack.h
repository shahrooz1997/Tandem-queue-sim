//
// Created by shahrooz on 11/15/20.
//

#ifndef CSE517_EVAL_EVENT_STACK_H
#define CSE517_EVAL_EVENT_STACK_H

#include <string>
#include <chrono>
#include "Job.h"
#include <memory>
#include "Queue.h"

using namespace std;
using namespace chrono;

enum Event_function{
    job_arrive_to_first_queue = 0,
    job_start_getting_service_first_queue,
    job_arrive_to_second_queue,
    job_start_getting_service_second_queue,
    job_departs_second_queue
};

struct Event{
    // Todo: Add attributes of an event here
    time_point<steady_clock, duration<double> > time;
    shared_ptr<Job> job;
    Event_function func;

    shared_ptr<Event> prev_event;
    shared_ptr<Event> next_event;
};

class Event_stack{
public:
    Event_stack(double arrival_rate, double q1_mean_service_time, double q2_mean_service_time);

    void run();

    const Queue& get_finished_jobs() const;

private:
    time_point<steady_clock, duration<double> > time;
    double arrival_rate;

    // Handling doubly linked list
    shared_ptr<Event> head;
    shared_ptr<Event> tail;

    // Stats
    uint32_t number_of_arrived_jobs;

    // Queues
    Queue q1;
    Queue q2;
    Queue finished_jobs;

    int add_event(shared_ptr<Event> event_p);
    int remove_event(shared_ptr<Event> event_p);

    int event_handler(shared_ptr<Event> event_p);
    int job_generator(uint32_t number_of_jobs);

    duration<double> get_exp_dist_time() const;
    duration<double> get_norm_dist_time() const;
};


#endif //CSE517_EVAL_EVENT_STACK_H
