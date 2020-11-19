//
// Created by shahrooz on 11/15/20.
//

#include "Job.h"
#include "Queue.h"


uint32_t Job::number_of_instances = 0;

std::chrono::microseconds get_service_time(std::chrono::microseconds mean_service_time){
    static std::default_random_engine e(std::chrono::system_clock::now().time_since_epoch().count());
    std::exponential_distribution<double> exp_dist(mean_service_time.count() / 1.0e6);

    return std::chrono::microseconds(static_cast<int64_t>(exp_dist(e) * 1.0e6));
}

const uint32_t Job::get_id() const{
    return id;
}

const time_point<steady_clock, microseconds>& Job::get_arrival_time() const{
    return arrival_time;
}

const microseconds& Job::get_sojourn_time() const{
    return sojourn_time;
}

const string& Job::get_description() const{
    return description;
}

const vector<Queue_info>& Job::get_queues() const{
    return queues;
}

Job::Job(const time_point<steady_clock, microseconds>& arrival_time) :
        id{number_of_instances++}, arrival_time{arrival_time}, description{""}, sojourn_time{0}{
}

int Job::service_start(time_point<steady_clock, microseconds> service_start_tp){
    queues.back().service_start_tp = service_start_tp;
    return 0;
}

int Job::move_to_queue(Queue& new_queue){
    if(!queues.empty()){
        queues.back().departure_tp = queues.back().service_start_tp + queues.back().service_time;
        queues.push_back(Queue_info{&new_queue, queues.back().departure_tp, time_point<steady_clock, milliseconds>(milliseconds(0)),
                new_queue.get_service_time(), time_point<steady_clock, milliseconds>(milliseconds(0))});
    }
    else{
        queues.push_back(Queue_info{&new_queue, arrival_time, time_point<steady_clock, milliseconds>(milliseconds(0)),
                new_queue.get_service_time(), time_point<steady_clock, milliseconds>(milliseconds(0))});
    }
    return 0;
}

int Job::finish_job(){
    sojourn_time = queues.back().arrival_tp - arrival_time;
    return 0;
}

ostream& operator<<(ostream& out, const microseconds& dur){
    out << dur.count();
    return out;
}

ostream& operator<<(ostream& out, const time_point<steady_clock, microseconds>& tp){
    out << tp.time_since_epoch().count();
    return out;
}

ostream& operator<<(ostream& out, const Job& job){
    out << "Job with id " << job.get_id() << endl;
    for(auto it = job.queues.begin(); it != job.queues.end(); it++){
        if(it->queue_p->get_service_rate() == 0){
            break;
        }
        out << it->arrival_tp << " " << it->service_start_tp << " " << it->service_time << " " << it->departure_tp << endl;
    }
    out << "sojourn time: " << job.get_sojourn_time() << " microseconds" << endl << endl;
    return out;
}
