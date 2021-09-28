//
// Created by aeols on 24.09.2021.
//

#ifndef PINATOR_TEST_MY_AREA_H
#define PINATOR_TEST_MY_AREA_H

#include <gtkmm/drawingarea.h>

class My_area : public Gtk::DrawingArea {
    double the_value{};
    uint32_t the_index{};

public:
    My_area();

    ~My_area() override;

    void initialize(uint32_t index) { the_index = index; };

    void set_value(double value) { the_value = value; }

    double get_value() const { return the_value; }

protected:
    //Override default signal handler:
    bool on_draw(const Cairo::RefPtr <Cairo::Context> &cr) override;

private:
    void draw_rectangle(const Cairo::RefPtr <Cairo::Context> &cr, int width, int height);

    void
    draw_text(const char *text, const Cairo::RefPtr <Cairo::Context> &cr, int rectangle_width, int rectangle_height);

};


#endif //PINATOR_TEST_MY_AREA_H
