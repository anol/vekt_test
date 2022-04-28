//
// Created by aeols on 24.09.2021.
//

#include "My_area.h"

My_area::My_area() {
}

My_area::~My_area() {
}

bool My_area::on_draw(const Cairo::RefPtr <Cairo::Context> &cr) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    // Draw a black rectangle
    cr->set_source_rgb(0.0, 0.0, 0.0);
    draw_rectangle(cr, width, height);
    // and some white text
    char buffer[16];
    switch (the_index) {
        case 0:
            sprintf(buffer, "%.2f bar", the_value);
            break;
        case 1:
            sprintf(buffer, "%.2f bar", the_value);
            break;
        case 2:
            sprintf(buffer, "%.2f kg", the_value);
            break;
        case 3:
            sprintf(buffer, "%.2f m/s", the_value);
            break;
        default:
            sprintf(buffer, "%.2f", the_value);
            break;
    }
    cr->set_source_rgb(1.0, 1.0, 1.0);
    draw_text(buffer, cr, width, height);
    return true;
}

void My_area::draw_rectangle(const Cairo::RefPtr <Cairo::Context> &cr,
                             int width, int height) {
    cr->rectangle(0, 0, width, height);
    cr->fill();
}

void My_area::draw_text(const char *text, const Cairo::RefPtr <Cairo::Context> &cr,
                        int rectangle_width, int rectangle_height) {
    // http://developer.gnome.org/pangomm/unstable/classPango_1_1FontDescription.html
    Pango::FontDescription font;

    font.set_family("Monospace");
    font.set_weight(Pango::WEIGHT_BOLD);
    font.set_size(40 * PANGO_SCALE);

    // http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
    auto layout = create_pango_layout(text);

    layout->set_font_description(font);

    int text_width;
    int text_height;

    //get the text dimensions (it updates the variables -- by reference)
    layout->get_pixel_size(text_width, text_height);

    // Position the text in the middle
    cr->move_to((rectangle_width - text_width) / 2, (rectangle_height - text_height) / 2);

    layout->show_in_cairo_context(cr);
}
