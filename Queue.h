//
// Created by shahrooz on 11/18/20.
//

#ifndef CSE517_EVAL_QUEUE_H
#define CSE517_EVAL_QUEUE_H

#include <vector>
#include "Job.h"
#include <memory>

using namespace std;
using namespace chrono;

class Queue{
public:
    explicit Queue(double service_rate);
    ~Queue() = default;

    shared_ptr<Job> get_job_by_id(uint32_t job_id);
    shared_ptr<Job> remove_job_by_id(uint32_t job_id);
    int add_job(shared_ptr<Job> job);

    bool is_empty();

    microseconds get_service_time();

    const vector<shared_ptr<Job>>& get_jobs_p() const;

private:
    vector<shared_ptr<Job> > jobs_p;
    double service_rate;
public:
    double get_service_rate() const;
};


#endif //CSE517_EVAL_QUEUE_H
