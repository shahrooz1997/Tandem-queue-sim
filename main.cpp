#include <iostream>
#include "Event_stack.h"

using namespace std;
using namespace chrono;

double get_mean_number_of_jobs_in_q(const Event_stack& event_stack, int queue_number){
    duration<double> simulation_duration =
            (*(event_stack.get_finished_jobs().get_jobs_p().end() - 10))->get_queues()[queue_number].departure_tp -
                (*(event_stack.get_finished_jobs().get_jobs_p().begin() + 10))->get_queues()[queue_number].arrival_tp;
    uint32_t number_of_jobs_in_queue = 0;
    uint32_t job_arr_index = 0;
    uint32_t job_dep_index = 0;
    auto it = event_stack.get_finished_jobs().get_jobs_p().begin();
    time_point<steady_clock, duration<double>> time;

    // warmup
    while(job_arr_index < 10 && job_dep_index < 10){
        if((*(it + job_arr_index))->get_queues()[queue_number].arrival_tp < (*(it + job_dep_index))->get_queues()[queue_number].departure_tp){
            number_of_jobs_in_queue++;
            time = (*(it + job_arr_index))->get_queues()[queue_number].arrival_tp;
            job_arr_index++;
        }
        else if((*(it + job_arr_index))->get_queues()[queue_number].arrival_tp > (*(it + job_dep_index))->get_queues()[queue_number].departure_tp){
            number_of_jobs_in_queue--;
            time = (*(it + job_dep_index))->get_queues()[queue_number].departure_tp;
            job_dep_index++;
        }
        else{
            time = (*(it + job_arr_index))->get_queues()[queue_number].arrival_tp;
            job_arr_index++;
            job_dep_index++;
            cout << "SSS" << endl;
        }
    }

    double workload_amount = 0;
    while(job_arr_index < event_stack.get_finished_jobs().get_jobs_p().size() - 10 &&
            job_dep_index < event_stack.get_finished_jobs().get_jobs_p().size() - 10){
        if((*(it + job_arr_index))->get_queues()[queue_number].arrival_tp < (*(it + job_dep_index))->get_queues()[queue_number].departure_tp){
            workload_amount += number_of_jobs_in_queue * ((*(it + job_arr_index))->get_queues()[queue_number].arrival_tp -
                                                          time).count();
            number_of_jobs_in_queue++;
            time = (*(it + job_arr_index))->get_queues()[queue_number].arrival_tp;
            job_arr_index++;
        }
        else if((*(it + job_arr_index))->get_queues()[queue_number].arrival_tp > (*(it + job_dep_index))->get_queues()[queue_number].departure_tp){
            workload_amount += number_of_jobs_in_queue * ((*(it + job_dep_index))->get_queues()[queue_number].departure_tp -
                                                          time).count();
            number_of_jobs_in_queue--;
            time = (*(it + job_dep_index))->get_queues()[queue_number].departure_tp;
            job_dep_index++;
        }
        else{
            time = (*(it + job_arr_index))->get_queues()[queue_number].arrival_tp;
            job_arr_index++;
            job_dep_index++;
            cout << "SSS" << endl;
        }
    }

    return workload_amount / simulation_duration.count();
}

duration<double> get_mean_sojourn_time_in_q(const Event_stack& event_stack, int queue_number){
    duration<double> total_sojourn_time;
    for(auto it = event_stack.get_finished_jobs().get_jobs_p().begin() + 10;
        it != event_stack.get_finished_jobs().get_jobs_p().end() - 10; it++){
        auto& job =  *(*it);
        total_sojourn_time += job.get_queues()[queue_number].departure_tp - job.get_queues()[queue_number].arrival_tp;
//        cout << job.get_sojourn_time().count() << endl;
//        cout << "AAa" << endl;
    }
    return duration<double>(total_sojourn_time.count() / event_stack.get_finished_jobs().get_jobs_p().size());
}

duration<double> get_mean_sojourn_time(const Event_stack& event_stack){
    duration<double> total_sojourn_time;
    for(auto it = event_stack.get_finished_jobs().get_jobs_p().begin() + 10;
        it != event_stack.get_finished_jobs().get_jobs_p().end() - 10; it++){
        auto& job =  *(*it);
        total_sojourn_time += job.get_sojourn_time();
//        cout << job.get_sojourn_time().count() << endl;
//        cout << "AAa" << endl;
    }
    return duration<double>(total_sojourn_time.count() / event_stack.get_finished_jobs().get_jobs_p().size());
}

void run_hundred_times(){
    for(int i = 0; i < 100; i++){
        Event_stack event_stack(7, .1, .05);
        event_stack.run();

        cout << get_mean_sojourn_time_in_q(event_stack, 0).count() << " " <<
                get_mean_sojourn_time_in_q(event_stack, 1).count() << " " <<
                get_mean_sojourn_time(event_stack).count() << " " <<
                get_mean_number_of_jobs_in_q(event_stack, 0) << " " <<
                get_mean_number_of_jobs_in_q(event_stack, 1)  << endl;
    }
}

int main() {
//    Event_stack event_stack(1, .8, .9);
//    event_stack.run();

//    cout << event_stack.get_finished_jobs();

//    cout << get_mean_number_of_jobs_in_q(event_stack, 0) << endl;
//
//    cout << "Sojourn time in Q1: " << get_mean_sojourn_time_in_q(event_stack, 0).count() << "s" << endl;
//    cout << "Sojourn time in Q2: " << get_mean_sojourn_time_in_q(event_stack, 1).count() << "s" << endl;
//    cout << get_mean_sojourn_time(event_stack).count() << "s" << endl;

    run_hundred_times();

    return 0;
}
