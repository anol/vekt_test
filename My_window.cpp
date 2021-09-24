//
// Created by aeols on 24.09.2021.
//

#include "My_window.h"
#include <iostream>

My_window::My_window() :
        the_box(Gtk::ORIENTATION_VERTICAL),
        the_button("Hello World"),
        m_Dispatcher(),
        m_Worker(),
        m_WorkerThread(nullptr) {
    set_border_width(10);
    the_button.signal_clicked().connect(sigc::mem_fun(*this,
                                                      &My_window::on_button_clicked));
    m_Dispatcher.connect(sigc::mem_fun(*this, &My_window::on_notification_from_worker_thread));
}

My_window::~My_window() {
}

void My_window::initialize() {
    the_area.set_size_request(250, 100);
    add(the_box);
    the_box.pack_start(the_area);
    the_box.pack_start(the_button);
    the_button.show();
    the_area.show();
    the_box.show();
    if (m_WorkerThread) {
        std::cout << "Can't start a worker thread while another one is running." << std::endl;
    } else {
        m_WorkerThread = new std::thread([this] { m_Worker.do_work(this); });
    }
}

void My_window::on_button_clicked() {
    the_area.set_value(the_area.get_value() + 1.1);
    auto win = get_window();
    if (win) {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                         get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
    std::cout << "Hello World" << std::endl;
}

void My_window::notify() {
    m_Dispatcher.emit();
}

void My_window::on_notification_from_worker_thread() {
    if (m_WorkerThread && m_Worker.has_stopped()) {
        if (m_WorkerThread->joinable())
            m_WorkerThread->join();
        delete m_WorkerThread;
        m_WorkerThread = nullptr;
    }
    update_widgets();
}

void My_window::update_widgets() {
    if (m_WorkerThread) {
        double value = m_Worker.get_data();
        the_area.set_value(value);
        auto win = get_window();
        if (win) {
            Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                             get_allocation().get_height());
            win->invalidate_rect(r, false);
        }
    }
}
