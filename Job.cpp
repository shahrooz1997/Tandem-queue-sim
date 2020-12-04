//
// Created by shahrooz on 11/15/20.
//

#include <iomanip>
#include "Job.h"
#include "Queue.h"


uint32_t Job::number_of_instances = 0;

const uint32_t Job::get_id() const{
    return id;
}

const time_point<steady_clock, duration<double>>& Job::get_arrival_time() const{
    return arrival_time;
}

const duration<double>& Job::get_sojourn_time() const{
    return sojourn_time;
}

const string& Job::get_description() const{
    return description;
}

const vector<Queue_info>& Job::get_queues() const{
    return queues;
}

Job::Job(const time_point<steady_clock, duration<double> >& arrival_time) :
        id{number_of_instances++}, arrival_time{arrival_time}, description{""}, sojourn_time{0}{
}

int Job::service_start(time_point<steady_clock, duration<double> > service_start_tp){
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

ostream& operator<<(ostream& out, const duration<double>& dur){
    out << dur.count();
    return out;
}

ostream& operator<<(ostream& out, const time_point<steady_clock, duration<double> >& tp){
    out << tp.time_since_epoch().count();
    return out;
}

ostream& operator<<(ostream& out, const Job& job){
    out << "Job with id " << job.get_id() << endl;
    for(auto it = job.queues.begin(); it != job.queues.end(); it++){
        if(it->queue_p->get_service_rate() == 0){
            break;
        }
        out << fixed << setprecision(6) << it->arrival_tp << " " << it->service_start_tp << " " << it->service_time << " " << it->departure_tp << endl;
    }
    out << "sojourn time: " << job.get_sojourn_time() << "s" << endl << endl;
    return out;
}

bool Job::operator!=(const Job& job){
    return this->get_id() == job.get_id();
}
