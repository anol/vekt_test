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
    My_area the_area;
    Gtk::Box the_box;
    Gtk::Button the_button;
    Glib::Dispatcher m_Dispatcher;
    My_worker m_Worker;
    std::thread *m_WorkerThread;

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
};

#endif //PINATOR_TEST_MY_WINDOW_H
