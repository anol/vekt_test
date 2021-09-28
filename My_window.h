//
// Created by aeols on 24.09.2021.
//

#ifndef PINATOR_TEST_MY_WINDOW_H
#define PINATOR_TEST_MY_WINDOW_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <glibmm/dispatcher.h>
#include "My_area.h"
#include "My_worker.h"

class My_window : public Gtk::Window {
    enum {
        Number_of_values = 4,
        Number_of_workers = 3
    };
    My_area the_area[Number_of_values];
    Gtk::Box the_box;
    Gtk::Button the_button;
    Glib::Dispatcher m_Dispatcher{};
    My_worker m_Worker[Number_of_workers]{};
    std::thread *m_WorkerThread[Number_of_workers]{};

public:
    My_window();

    virtual ~My_window();

    void initialize();

    void notify();

protected:
    void on_button_clicked();

private:
    void on_notification_from_worker_thread();

    void update_widgets();

    static void run_worker(uint32_t n);

    My_worker *get_worker(uint32_t index) { return (index < Number_of_workers) ? &m_Worker[index] : nullptr; };
};

#endif //PINATOR_TEST_MY_WINDOW_H
