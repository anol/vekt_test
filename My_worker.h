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
        Number_of_values = 4,
        Buffer_size = 16
    };
    char the_buffer[Buffer_size]{};
    int the_pos{};
    bool the_flag[Number_of_values]{};
    double the_data[Number_of_values]{};
    My_window *optional_caller{};

public:
    My_worker();

    void do_work(const char* port_name);

    bool has_data(uint32_t index) const;

    double get_data(uint32_t index);

    void stop_work();

    bool has_stopped() const;

    void initalize(My_window *);

private:
    // Synchronizes access to member data.
    mutable std::mutex m_Mutex;

    // Data used by both GUI thread and worker thread.
    bool m_shall_stop;
    bool m_has_stopped;

    void update_data(uint32_t index);
};


#endif //PINATOR_TEST_MY_WORKER_H
