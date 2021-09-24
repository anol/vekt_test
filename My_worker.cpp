#include "My_worker.h"
#include "My_window.h"
#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <cstdio>

My_worker::My_worker() :
        m_Mutex(),
        m_shall_stop(false),
        m_has_stopped(false) {
}

double My_worker::get_data()  {
    std::lock_guard<std::mutex> lock(m_Mutex);
    return the_data;
}

void My_worker::update_data(My_window *caller) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    the_buffer[the_pos] = 0;
    try {
        the_data = std::stod(the_buffer);
    }
    catch (...) {
    }
    the_pos = 0;
    caller->notify();
}

void My_worker::stop_work() {
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_shall_stop = true;
}

bool My_worker::has_stopped() const {
    std::lock_guard<std::mutex> lock(m_Mutex);
    return m_has_stopped;
}

void My_worker::do_work(My_window *caller) {
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_has_stopped = false;
    }
    std::cout << "Hi there!" << std::endl;
    auto fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
    if (fd == -1) {
        perror("<> open_port: Unable to open /dev/ttyUSB0 <>");
        return;
    }
    struct termios options{};
    tcgetattr(fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    tcsetattr(fd, TCSANOW, &options);
    the_pos = 0;
    while (!m_shall_stop) {
        char sym;
        while (read(fd, &sym, 1) > 0) {
            {
                if (the_pos < Buffer_size) {
                    switch (sym) {
                        case '\r':
                        case '\n':
                        case 'x':
                        case '=':
                            break;
                        case ';':
                            update_data(caller);
                            break;
                        case '.':
                            the_buffer[the_pos++] = ',';
                            break;
                        default:
                            the_buffer[the_pos++] = sym;
                            break;
                    }
                } else {
                    the_pos = 0;
                }
            }
        }
    }
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_shall_stop = false;
        m_has_stopped = true;
    }
    caller->notify();
}
