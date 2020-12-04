//
// Created by shahrooz on 11/18/20.
//

#include "Queue.h"

shared_ptr<Job> Queue::get_job_by_id(uint32_t job_id) {
    for(auto it = jobs_p.begin(); it != jobs_p.end(); it++){
        if((*it)->get_id() == job_id){
            return *it;
        }
    }
    return shared_ptr<Job>();
}

shared_ptr<Job> Queue::remove_job_by_id(uint32_t job_id) {
    for(auto it = jobs_p.begin(); it != jobs_p.end(); it++){
        if((*it)->get_id() == job_id){
            shared_ptr<Job> ret = *it;
            jobs_p.erase(it);
            return ret;
        }
    }
    return shared_ptr<Job>();
}

int Queue::add_job(shared_ptr<Job> job) {
    jobs_p.push_back(job);
    return 0;
}

Queue::Queue(double mean_service_time) : mean_service_time{mean_service_time}{
}

std::chrono::duration<double> Queue::get_service_time(){
    static std::default_random_engine e(std::chrono::system_clock::now().time_since_epoch().count());
    std::exponential_distribution<double> exp_dist(1. / mean_service_time);

    return std::chrono::duration<double>(exp_dist(e));
}

//microseconds Queue::get_service_time(){
//    static std::default_random_engine e(std::chrono::system_clock::now().time_since_epoch().count());
//    std::exponential_distribution<double> exp_dist(1. / service_rate);
//
//    return std::chrono::microseconds(static_cast<int64_t>(exp_dist(e) * 1.0e6));
//}

bool Queue::is_empty(){
    return jobs_p.empty();
}

const vector<shared_ptr<Job>>& Queue::get_jobs_p() const{
    return jobs_p;
}

double Queue::get_service_rate() const{
    return mean_service_time;
}

ostream& operator<<(ostream& out, const Queue& queue){
    for(auto it = queue.get_jobs_p().begin(); it != queue.get_jobs_p().end(); it++){
        cout << *(*it);
    }
}
