#ifndef NUCMD_HPP
#define NUCMD_HPP

#include <ncurses.h>
#include <panel.h>
#include <memory>
#include <vector>
#include <iostream>
#include <string>

#define UI_EXIT_KEY KEY_F(10)

#define TOP_PANE_HEIGHT 1
#define BOTTOM_PANE_HEIGHT 1
#define LEFT_PANE_WIDTH 20

namespace nucmd {
    class Ui;
    class AbstractPane;
    class AbstractItem;
    class AbstractItemList;
    class Item;
    class ItemList;
    class Pane;
    enum class Dim {HEIGHT, WIDTH, Y, X};
    enum class Panes {TOP, BOTTOM, LEFT, RIGHT};
    enum class Layout {HORIZONTAL, VERTICAL, HORIZONTAL_GRID, VERTICAL_GRID};
    enum class Menus {FILE, EDIT, VIEW, HELP, COUNT};
}

class nucmd::AbstractItem{
public:
    virtual void set_text(std::string) = 0;
    virtual std::string get_text() = 0;
protected:
    std::string text;
    bool selected;
};

class nucmd::Item : public nucmd::AbstractItem {
    Item() = default;
    void set_text(std::string) override;
    std::string get_text() override;
};

class nucmd::AbstractItemList {
public:
    AbstractItemList();
    virtual void push_back(Item) = 0;
    virtual void insert(int, Item) = 0;
    virtual void set_selected(int) = 0;
protected:
    std::vector<Item> items;
    int selected_index;
};

class nucmd::ItemList : public nucmd::AbstractItemList {
public:
    ItemList() = default;
    void push_back(Item) override;
    void insert(int, Item) override;
    void set_selected(int) override;
};

class nucmd::AbstractPane {
public:
    AbstractPane();
    virtual void draw_border() = 0;
    virtual void resize() = 0;
    virtual void set_dim(nucmd::Dim, int) = 0;
    virtual int get_dim(nucmd::Dim) = 0;
    virtual void set_color_pairs(int, int) = 0;
    virtual void set_border_enabled(bool) = 0;
    virtual void refresh() = 0;
    virtual void set_layout(nucmd::Layout) = 0;
    virtual void set_layout_spacing(int) = 0;
    virtual void set_cursor(int, int) = 0;
    virtual void reset_cursor() = 0;
    virtual void increment_cursor() = 0;
    virtual void panel_top() = 0;
    virtual void panel_bottom() = 0;
    virtual void insert_item(Item) = 0;
    virtual void push_back_item(Item) = 0;
protected:
    WINDOW* win;
    PANEL* panel;
    std::vector<int> dims;
    bool border_enabled;
    int foreground_color;
    int background_color;
    nucmd::Layout layout;
    int layout_spacing;
    std::vector<int> cursor; // the current cursor position
    std::vector<int> cursor_config; // the default cursor position
    ItemList items;
};

class nucmd::Pane : public nucmd::AbstractPane {
public:
    Pane() = default;
    void draw_border() override;
    void resize() override;
    void refresh() override;
    void set_dim(nucmd::Dim, int) override;
    int get_dim(nucmd::Dim) override;
    void set_color_pairs(int, int) override;
    void set_border_enabled(bool) override;
    void set_layout(nucmd::Layout) override;
    void set_layout_spacing(int) override;
    void set_cursor(int, int) override;
    void reset_cursor() override;
    void increment_cursor() override;
    void panel_top() override;
    void panel_bottom() override;
    void insert_item(Item) override;
    void push_back_item(Item) override;
};

class nucmd::Ui {
public:
    Ui();
    ~Ui();
    int run();
private:
    bool get_key();
    void action_key();
    void initialize_panes();
    void initialize_ncurses();
    void initialize_colors();
    void initialize_menubar();
    void initialize_menus();
    void show_menu(nucmd::Menus);
    void recalc_panes_dimensions();
    void resize_panes();
    void draw_panes();
    void put_menu();

    int key_pressed;
    std::vector<Pane> panes;
    std::vector<Pane> menus;
    std::unique_ptr<Pane> menu_active;
};

#endif
