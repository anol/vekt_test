//
// Created by aeols on 24.09.2021.
//

#include "My_window.h"
#include <iostream>

My_window::My_window() :
        the_box(Gtk::ORIENTATION_VERTICAL),
        the_button("NMDF Dynamometervogn " __DATE__) {
    set_border_width(10);
    the_button.signal_clicked().connect(sigc::mem_fun(*this,
                                                      &My_window::on_button_clicked));
    m_Dispatcher.connect(sigc::mem_fun(*this, &My_window::on_notification_from_worker_thread));
}

My_window::~My_window() {
}

static My_window *optional_instance{};

void My_window::run_worker(uint32_t n) {
    const char port_name[Number_of_workers][16] = {
            "/dev/ttyUSB0",
            "/dev/ttyUSB1",
            "/dev/ttyUSB2"
    };
    if (optional_instance) {
        auto *worker = optional_instance->get_worker(n);
        if (worker && n < Number_of_workers) {
            worker->do_work(port_name[n]);
        }
    }
}

void My_window::initialize() {
    optional_instance = this;
    move(0, 0);
    add(the_box);
    for (uint32_t index = 0; index < Number_of_values; index++) {
        the_area[index].initialize(index);
        the_area[index].set_size_request(400, 100);
        the_box.pack_start(the_area[index]);
    }
    the_box.pack_start(the_button);
    the_button.show();
    for (auto &area: the_area) {
        area.show();
    }
    the_box.show();
    for (uint32_t gpc = 0; gpc < Number_of_workers; gpc++) {
        if (!m_WorkerThread[gpc]) {
            m_Worker[gpc].initalize(this);
            m_WorkerThread[gpc] = new std::thread(My_window::run_worker, gpc);
        }
    }
}

void My_window::on_button_clicked() {
    the_area[1].set_value(the_area[1].get_value() + 1.1);
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
    for (uint32_t gpc = 0; gpc < Number_of_workers; gpc++) {
        if (m_WorkerThread[gpc] && m_Worker[gpc].has_stopped()) {
            if (m_WorkerThread[gpc]->joinable())
                m_WorkerThread[gpc]->join();
            delete m_WorkerThread[gpc];
            m_WorkerThread[gpc] = nullptr;
        }
    }
    update_widgets();
}

void My_window::update_widgets() {
    for (uint32_t gpc = 0; gpc < Number_of_values; gpc++) {
        for (uint32_t worker = 0; worker < Number_of_workers; worker++) {
            if (m_Worker[worker].has_data(gpc)) {
                double value = m_Worker[worker].get_data(gpc);
                the_area[gpc].set_value(value);
            }
        }
    }
    auto win = get_window();
    if (win) {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                         get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
}
