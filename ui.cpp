#include "nucmd.hpp"
using namespace nucmd;

Ui::Ui()
    :key_pressed{0}
{
    initialize_ncurses();
    initialize_colors();
    initialize_panes();
    resize_panes();
    draw_panes();
    initialize_menubar();
    put_menu();
}

Ui::~Ui() {
    endwin();
}

void Ui::initialize_ncurses() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
}

void Ui::initialize_colors() {
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_WHITE);
    init_pair(3, COLOR_BLUE, COLOR_GREEN);
    init_pair(4, COLOR_BLUE, COLOR_GREEN);
}

int Ui::run(){
    while (get_key()) {
        action_key();
    }
    return key_pressed;
}

bool Ui::get_key() {
    key_pressed = getch();
    if (key_pressed == UI_EXIT_KEY)
        return false;
    return true;
}

void Ui::resize_panes() {
    recalc_panes_dimensions();

    for (auto& pane : panes)
        pane.resize();
}

void Ui::draw_panes() {
    for (auto& pane : panes)
        pane.draw_border();
}

void Ui::show_menu(nucmd::Menus menu) {
    switch (menu) {
        case Menus::FILE:
            break;
        case Menus::EDIT:
            break;
        case Menus::VIEW:
            break;
        case Menus::HELP:
            break;
    }
}

void Ui::initialize_menubar() {
    // menu_items.push_back(std::make_unique<MenuItem>("[F1 File]", "File operations", "file", 1));
    // menu_items.push_back(std::make_unique<MenuItem>("[F2 Edit]", "Edit operations", "edit", 2));
    // menu_items.push_back(std::make_unique<MenuItem>("[F3 View]", "View operations", "view", 3));
    // menu_items.push_back(std::make_unique<MenuItem>("[F4 Search]", "Search operations", "search", 4));
    // menu_items.push_back(std::make_unique<MenuItem>("[F5/F6 Resize]", "Search operations", "search", 4));
    // menu_items.push_back(std::make_unique<MenuItem>("[F10 Exit]", "Search operations", "search", 4));
}

void Ui::put_menu() {
    panes.at((size_t)Panes::BOTTOM).refresh();
    panes.at((size_t)Panes::BOTTOM).reset_cursor();
}

void Ui::initialize_panes() {
    for (int i = 0; i < 4; i++)
        panes.push_back(Pane());

    panes.at((size_t)Panes::TOP).set_dim(Dim::HEIGHT, TOP_PANE_HEIGHT);
    panes.at((size_t)Panes::BOTTOM).set_dim(Dim::HEIGHT, BOTTOM_PANE_HEIGHT);
    panes.at((size_t)Panes::LEFT).set_dim(Dim::WIDTH, LEFT_PANE_WIDTH);

    panes.at((size_t)Panes::BOTTOM).set_border_enabled(false);
    panes.at((size_t)Panes::TOP).set_border_enabled(false);
    panes.at((size_t)Panes::BOTTOM).set_color_pairs(1,2);
    panes.at((size_t)Panes::TOP).set_color_pairs(1,1);

    panes.at((size_t)Panes::BOTTOM).set_layout(Layout::HORIZONTAL);
    panes.at((size_t)Panes::BOTTOM).set_layout_spacing(1);
    panes.at((size_t)Panes::BOTTOM).set_cursor(0,0);
    panes.at((size_t)Panes::BOTTOM).reset_cursor();
}

void Ui::initialize_menus(){
    for (int i = 0; i < (static_cast<int>(Menus::COUNT)); i++)
        menus.push_back(Pane());
}

void Ui::action_key() {
    int curwidth;
    switch (key_pressed) {
        case KEY_RESIZE:
            resize_panes();
            draw_panes();
            put_menu();
            break;
        case KEY_F(1):
            show_menu(Menus::FILE);
            break;
        case KEY_F(5):
            curwidth = panes.at((size_t)Panes::LEFT).get_dim(Dim::WIDTH);
            panes.at((size_t)Panes::LEFT).set_dim(Dim::WIDTH, curwidth - 1);
            resize_panes();
            draw_panes();
            put_menu();
            break;
        case KEY_F(6):
            curwidth = panes.at((size_t)Panes::LEFT).get_dim(Dim::WIDTH);
            panes.at((size_t)Panes::LEFT).set_dim(Dim::WIDTH, curwidth + 1);
            resize_panes();
            draw_panes();
            put_menu();
            break;
        default:
            break;
    }
}

void Ui::recalc_panes_dimensions() {
    panes.at((size_t)Panes::TOP).set_dim(Dim::WIDTH, COLS);

    panes.at((size_t)Panes::BOTTOM).set_dim(Dim::WIDTH, COLS);
    panes.at((size_t)Panes::BOTTOM).set_dim(
        Dim::Y, LINES - panes.at((size_t)Panes::BOTTOM).get_dim(Dim::HEIGHT));

    panes.at((size_t)Panes::LEFT).set_dim(
        Dim::HEIGHT, LINES 
        - panes.at((size_t)Panes::TOP).get_dim(Dim::HEIGHT) 
        - panes.at((size_t)Panes::BOTTOM).get_dim(Dim::HEIGHT));
    panes.at((size_t)Panes::LEFT).set_dim(
        Dim::Y, panes.at((size_t)Panes::TOP).get_dim(Dim::HEIGHT));

    panes.at((size_t)Panes::RIGHT).set_dim(
        Dim::HEIGHT, LINES 
        - panes.at((size_t)Panes::TOP).get_dim(Dim::HEIGHT) 
        - panes.at((size_t)Panes::BOTTOM).get_dim(Dim::HEIGHT));
    panes.at((size_t)Panes::RIGHT).set_dim(
        Dim::WIDTH, COLS - panes.at((size_t)Panes::LEFT).get_dim(Dim::WIDTH));
    panes.at((size_t)Panes::RIGHT).set_dim(
        Dim::Y, panes.at((size_t)Panes::TOP).get_dim(Dim::HEIGHT));
    panes.at((size_t)Panes::RIGHT).set_dim(
        Dim::X, panes.at((size_t)Panes::LEFT).get_dim(Dim::WIDTH));
}
