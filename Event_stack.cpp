//
// Created by shahrooz on 11/15/20.
//

#include "Event_stack.h"
#include <thread>
#include <assert.h>

Event_stack::Event_stack(double arrival_rate, double q1_service_rate, double q2_service_rate) :
                            q1(q1_service_rate), q2(q2_service_rate), finished_jobs(0), arrival_rate{arrival_rate},
                            number_of_arrived_jobs{0}, time{time_point_cast<milliseconds>(steady_clock::now())},
                            head{nullptr}, tail{nullptr}{
}

void Event_stack::start(){
    job_generator(10);
    while(head != nullptr){
        event_handler(head);
        remove_event(head);
    }
    return;
}

int Event_stack::add_event(shared_ptr<Event> event_p){
    if(!head){
        head = make_shared<Event>();
        tail = head;
        head->time = event_p->time;
        head->job = event_p->job;
        head->func = event_p->func;
        head->prev_event = head;
        head->next_event = head;
        return 0;
    }
    shared_ptr<Event> iter = head;
    if(iter->time > event_p->time){
        shared_ptr<Event> new_event = make_shared<Event>();
        new_event->time = event_p->time;
        new_event->job = event_p->job;
        new_event->func = event_p->func;
        new_event->prev_event = iter->prev_event;
        new_event->next_event = iter;
        iter->prev_event = new_event;
        new_event->prev_event->next_event = new_event;
        head = new_event;
        return 0;
    }
    iter = head->next_event;
    while(iter != head){
        if(iter->time > event_p->time){
            shared_ptr<Event> new_event = make_shared<Event>();
            new_event->time = event_p->time;
            new_event->job = event_p->job;
            new_event->func = event_p->func;
            new_event->prev_event = iter->prev_event;
            new_event->next_event = iter;
            iter->prev_event = new_event;
            new_event->prev_event->next_event = new_event;
            return 0;
        }
        iter = iter->next_event;
    }
    shared_ptr<Event> new_event = make_shared<Event>();
    new_event->time = event_p->time;
    new_event->job = event_p->job;
    new_event->func = event_p->func;
    new_event->prev_event = tail;
    new_event->next_event = iter;
    iter->prev_event = new_event;
    tail->next_event = new_event;
    tail = new_event;
    return 0;
}

int Event_stack::remove_event(shared_ptr<Event> event_p){
    if(head == tail && head == event_p){
        head->next_event = nullptr;
        head->prev_event = nullptr;
        head = nullptr;
        tail = nullptr;
        return 0;
    }
    else if(head == event_p){
        head->next_event->prev_event = tail;
        tail->next_event = head->next_event;
        head = head->next_event;
        return 0;
    }
    if(tail == event_p){
        tail->prev_event->next_event = head;
        head->prev_event = tail->prev_event;
        tail = tail->prev_event;
        return 0;
    }
    shared_ptr<Event> iter = head;
    while(iter != event_p){
        iter = head->next_event;
    }
    iter->prev_event->next_event = iter->next_event;
    iter->next_event->prev_event = iter->prev_event;
    return 0;
}

int Event_stack::event_handler(shared_ptr<Event> event_p){
    if(event_p->func == Event_function::job_arrive_to_first_queue){
        event_p->job->move_to_queue(q1);
        if(q1.is_empty()){
            q1.add_job(event_p->job);
            shared_ptr<Event> e = make_shared<Event>();
            e->time = event_p->job->get_queues().front().arrival_tp;
            e->job = event_p->job;
            e->func = Event_function::job_start_getting_service_first_queue;
            add_event(e);
        }
        else{
            q1.add_job(event_p->job);
        }
    }
    else if(event_p->func == Event_function::job_start_getting_service_first_queue){
        // Todo: it also means that a job has depart from queue1, so the next job should take the server here.
        event_p->job->service_start(event_p->time);
        shared_ptr<Event> e = make_shared<Event>();
        e->time = event_p->job->get_queues().back().service_start_tp + event_p->job->get_queues().back().service_time;
        e->job = event_p->job;
        e->func = Event_function::job_arrive_to_second_queue;
        add_event(e);
    }
    else if(event_p->func == Event_function::job_arrive_to_second_queue){
        event_p->job->move_to_queue(q2);
        if(q2.is_empty()){
            q2.add_job(event_p->job);
            shared_ptr<Event> e = make_shared<Event>();
            e->time = event_p->job->get_queues().back().arrival_tp;
            e->job = event_p->job;
            e->func = Event_function::job_start_getting_service_second_queue;
            add_event(e);
        }
        else{
            q2.add_job(event_p->job);
        }
        q1.remove_job_by_id(event_p->job->get_id());

        // Service another job in the queue
        if(!q1.is_empty()){
            shared_ptr<Event> e2 = make_shared<Event>();
            e2->time = event_p->job->get_queues().rbegin()[1].departure_tp;
            e2->job = q1.get_jobs_p().front();
            e2->func = Event_function::job_start_getting_service_first_queue;
            add_event(e2);
        }
    }
    else if(event_p->func == Event_function::job_start_getting_service_second_queue){
        event_p->job->service_start(event_p->time);
        shared_ptr<Event> e = make_shared<Event>();
        e->time = event_p->job->get_queues().back().service_start_tp + event_p->job->get_queues().back().service_time;
        e->job = event_p->job;
        e->func = Event_function::job_departs_second_queue;
        add_event(e);
    }
    else if(event_p->func == Event_function::job_departs_second_queue){
        event_p->job->move_to_queue(finished_jobs);
        finished_jobs.add_job(event_p->job);
        event_p->job->finish_job();
        q2.remove_job_by_id(event_p->job->get_id());

        // Service another job in the queue
        if(!q2.is_empty()){
            shared_ptr<Event> e2 = make_shared<Event>();
            e2->time = event_p->job->get_queues().rbegin()[1].departure_tp;
            e2->job = q2.get_jobs_p().front();
            e2->func = Event_function::job_start_getting_service_second_queue;
            add_event(e2);
        }
    }
    else{
        assert(false);
    }
    return 0;
}

int Event_stack::job_generator(uint32_t number_of_jobs){
    time_point<steady_clock, microseconds> timer = time_point_cast<microseconds>(steady_clock::now());
    while(number_of_arrived_jobs < number_of_jobs){
        microseconds dur = get_exp_dist_time();
        timer += dur;

        // Create a job arrival event to queue 1
        shared_ptr<Event> e = make_shared<Event>();
        e->time = timer;
        e->job = make_shared<Job>(timer);
        e->func = Event_function::job_arrive_to_first_queue;
        add_event(e);

        number_of_arrived_jobs++;
    }

    return 0;
}

std::chrono::microseconds Event_stack::get_exp_dist_time() const{
    static std::default_random_engine e(std::chrono::system_clock::now().time_since_epoch().count());
    std::exponential_distribution<double> exp_dist(1. / arrival_rate);

    return std::chrono::microseconds(static_cast<int64_t>(exp_dist(e) * 1.0e6));
}

const Queue& Event_stack::get_finished_jobs() const{
    return finished_jobs;
}
