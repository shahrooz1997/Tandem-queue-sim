//
// Created by shahrooz on 11/15/20.
//

#ifndef CSE517_EVAL_JOB_H
#define CSE517_EVAL_JOB_H

#include <string>
#include <chrono>
#include <random>
#include <iostream>

using namespace std;
using namespace chrono;

class Queue; // forward declaration

struct Queue_info{
    Queue* queue_p;
    time_point<steady_clock, duration<double> > arrival_tp;
    time_point<steady_clock, duration<double> > service_start_tp;
    duration<double> service_time;
    time_point<steady_clock, duration<double> > departure_tp;
};

// Todo: Add attributes of an event here
class Job{
public:
    explicit Job(const time_point<steady_clock, duration<double> >& arrival_time);

    int service_start(time_point<steady_clock, duration<double> > service_start_tp);
    int move_to_queue(Queue& new_queue);
    int finish_job();

    // Getters
    const uint32_t get_id() const;
    const time_point<steady_clock, duration<double> >& get_arrival_time() const;
    const duration<double>& get_sojourn_time() const;
    const string& get_description() const;
    const vector<Queue_info>& get_queues() const;

private:
    const uint32_t id;
    time_point<steady_clock, duration<double> > arrival_time;
    duration<double> sojourn_time;
    string description;

    vector<Queue_info> queues; // The last one is the queue that the job is in.

    // id generator
    static uint32_t number_of_instances;

    friend ostream& operator<<(ostream& out, const Job& job);
};

ostream& operator<<(ostream& out, const Job& job);

#endif //CSE517_EVAL_JOB_H
