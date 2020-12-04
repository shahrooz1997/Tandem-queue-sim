//
// Created by shahrooz on 11/18/20.
//

#ifndef CSE517_EVAL_QUEUE_H
#define CSE517_EVAL_QUEUE_H

#include <vector>
#include "Job.h"
#include <memory>
#include <iostream>

using namespace std;
using namespace chrono;

class Queue{
public:
    explicit Queue(double mean_service_time);
    ~Queue() = default;

    shared_ptr<Job> get_job_by_id(uint32_t job_id);
    shared_ptr<Job> remove_job_by_id(uint32_t job_id);
    int add_job(shared_ptr<Job> job);

    bool is_empty();

    duration<double> get_service_time();

    const vector<shared_ptr<Job>>& get_jobs_p() const;

    double get_service_rate() const;

private:
    vector<shared_ptr<Job> > jobs_p;
    double mean_service_time;

    friend ostream& operator<<(ostream& out, const Queue& queue);
};

ostream& operator<<(ostream& out, const Queue& queue);

#endif //CSE517_EVAL_QUEUE_H
