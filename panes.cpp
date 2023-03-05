#include "nucmd.hpp"
using namespace nucmd;

AbstractPane::AbstractPane()
    : dims{0, 0, 0, 0},
      border_enabled{true},
      foreground_color{1},
      background_color{2},
      layout{Layout::VERTICAL},
      layout_spacing{0},
      cursor_config{1, 1},
      cursor{1, 1}
{
    win = newwin(0, 0, 0, 0);
    panel = new_panel(win);
    wattron(win, COLOR_PAIR(foreground_color));
}

void Pane::draw_border() {
    if (border_enabled) {
        box(win, 0, 0);
    }
}

void Pane::set_color_pairs(int foregroround_color, int background_color) {
    this->foreground_color = foregroround_color;
    this->background_color = background_color;
    wattron(win, COLOR_PAIR(foregroround_color));
    wbkgdset(win, COLOR_PAIR(background_color));
}

void Pane::set_border_enabled(bool border_enabled) {
    this->border_enabled = border_enabled;
}

void Pane::set_dim(nucmd::Dim dim, int value) {
    dims[(int)dim] = value;
}

int Pane::get_dim(nucmd::Dim dim) {
    return dims[(int)dim];
}

void Pane::refresh() {
    update_panels();
    doupdate();
}

void Pane::resize() {
    wresize(win, get_dim(Dim::HEIGHT), get_dim(Dim::WIDTH));
    mvwin(win, get_dim(Dim::Y), get_dim(Dim::X));
    werase(win);
}

void Pane::set_layout(nucmd::Layout layout) {
    this->layout = layout;
}

void Pane::set_layout_spacing(int spacing) {
    this->layout_spacing = spacing;
}

void Pane::set_cursor(int y, int x) {
    cursor_config[0] = y;
    cursor_config[1] = x;
}

void Pane::reset_cursor() {
    cursor[0] = cursor_config[0];
    cursor[1] = cursor_config[1];
}

void Pane::print(std::string str) {
    wmove(win, cursor[0], cursor[1]);
    for (auto& c : str) {
        waddch(win, c);
        increment_cursor();
    }
    increment_cursor();
}

void Pane::increment_cursor() {
    if (layout == Layout::HORIZONTAL) {
        cursor[1] += layout_spacing;
    }
}

void Pane::panel_top() {
    top_panel(panel);
}

void Pane::panel_bottom() {
    bottom_panel(panel);
}
