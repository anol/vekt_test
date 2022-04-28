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

void My_worker::initalize(My_window *window) {
    optional_caller = window;
}

bool My_worker::has_data(uint32_t index) const {
    return (index < Number_of_values) && the_flag[index];
}

double My_worker::get_data(uint32_t index) {
    std::lock_guard <std::mutex> lock(m_Mutex);
    return (index < Number_of_values) ? the_data[index] : 0.0;
}

void My_worker::update_data(uint32_t index) {
    std::lock_guard <std::mutex> lock(m_Mutex);
    the_buffer[the_pos] = 0;
    try {
        switch (index) {
            case 0: // Pressure A
                the_data[0] = std::stod(the_buffer) / 100.0;
                break;
            case 1: // Pressure B
                the_data[1] = std::stod(the_buffer) / 100.0;
                break;
            case 2: // Weight C
                the_data[2] = std::stod(the_buffer);
                break;
            case 3: // Speed D
                the_data[3] = std::stod(the_buffer);
                break;
            default:
                break;
        }
    }
    catch (...) {
    }
    the_pos = 0;
    if (optional_caller) {
        optional_caller->notify();
    }
}

void My_worker::stop_work() {
    std::lock_guard <std::mutex> lock(m_Mutex);
    m_shall_stop = true;
}

bool My_worker::has_stopped() const {
    std::lock_guard <std::mutex> lock(m_Mutex);
    return m_has_stopped;
}

void My_worker::do_work(const char *port_name) {
    {
        std::lock_guard <std::mutex> lock(m_Mutex);
        m_has_stopped = false;
    }
    std::cout << "\r\n Hi there " << port_name << std::endl;
    auto fd = open(port_name, O_RDWR | O_NOCTTY);
    if (fd == -1) {
        perror(port_name);
        return;
    }
    struct termios options{};
    tcgetattr(fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    tcsetattr(fd, TCSANOW, &options);
    the_pos = 0;
    uint32_t index = 0;
    while (!m_shall_stop) {
        char sym;
        while (read(fd, &sym, 1) > 0) {
            if (the_pos < Buffer_size) {
                switch (sym) {
                    case '\r':
                    case '\n':
                    case '=':
                    case 'P':
                    case 'W':
                    case 'T':
                        break;
                    case 'a':
                        index = 0;
                        the_flag[0] = true;
                        break;
                    case 'b':
                        index = 1;
                        the_flag[1] = true;
                        break;
                    case 'c':
                        index = 2;
                        the_flag[2] = true;
                        break;
                    case 'd':
                        index = 3;
                        the_flag[3] = true;
                        break;
                    case ';':
                        update_data(index);
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
    {
        std::lock_guard <std::mutex> lock(m_Mutex);
        m_shall_stop = false;
        m_has_stopped = true;
    }
    if (optional_caller) optional_caller->notify();
}
