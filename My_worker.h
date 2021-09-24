//
// Created by aeols on 24.09.2021.
//

#ifndef PINATOR_TEST_MY_WORKER_H
#define PINATOR_TEST_MY_WORKER_H

#include <gtkmm.h>
#include <thread>
#include <mutex>

class My_window;

class My_worker {
    enum {
        Buffer_size = 16
    };
    char the_buffer[Buffer_size]{};
    int the_pos{};
    double the_data{};

public:
    My_worker();

    // Thread function.
    void do_work(My_window *caller);

    double get_data();

    void stop_work();

    bool has_stopped() const;

private:
    // Synchronizes access to member data.
    mutable std::mutex m_Mutex;

    // Data used by both GUI thread and worker thread.
    bool m_shall_stop;
    bool m_has_stopped;

    void update_data(My_window *caller);
};


#endif //PINATOR_TEST_MY_WORKER_H
