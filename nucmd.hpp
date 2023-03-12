#ifndef NUCMD_HPP
#define NUCMD_HPP

#include <string>
#include <iostream>
#include <vector>
#include <ncurses.h>
#include <panel.h>

#define NUCMD_VERSION "0.1"
#define QUIT_KEY KEY_F(10)

#define LEFT_PANE_WIDTH 20
#define TOP_PANE_HEIGHT 3
#define BOTTOM_PANE_HEIGHT 3

#define LEFT_PANE (int)Panes::LEFT
#define RIGHT_PANE (int)Panes::RIGHT
#define TOP_PANE (int)Panes::TOP
#define BOTTOM_PANE (int)Panes::BOTTOM
#define PANES_COUNT (int)Panes::COUNT

#define HEIGHT_DIM (int)Dims::HEIGHT
#define WIDTH_DIM (int)Dims::WIDTH
#define Y_DIM (int)Dims::Y
#define X_DIM (int)Dims::X
#define DIMS_COUNT (int)Dims::COUNT

#define MENU_PANE TOP_PANE
#define STATUS_PANE BOTTOM_PANE
#define OPTIONS_PANE LEFT_PANE
#define CONTENT_PANE RIGHT_PANE

#define HORIZONTAL_LAYOUT (int)Layouts::HORIZONTAL
#define VERTICAL_LAYOUT (int)Layouts::VERTICAL
#define HORIZONTAL_GRID_LAYOUT (int)Layouts::HORIZONTAL_GRID
#define VERTICAL_GRID_LAYOUT (int)Layouts::VERTICAL_GRID

#define LEFT_ALIGN (int)LayoutAlignments::LEFT
#define RIGHT_ALIGN (int)LayoutAlignments::RIGHT
#define CENTER_ALIGN (int)LayoutAlignments::CENTER
#define JUSTIFIED_ALIGN (int)LayoutAlignments::JUSTIFIED

#define FIXED_ITEM_EXPAND (int)ItemExpansion::FIXED
#define FILL_ITEM_EXPAND (int)ItemExpansion::FILL
#define FIT_ITEM_EXPAND (int)ItemExpansion::FIT

#define KEY_TAB '\t'
#define KEY_ESC 27

namespace nucmd {
    class Item;
    class Pane;
    class Nucmd;
    enum class Panes { LEFT, RIGHT, TOP, BOTTOM, COUNT };
    enum class Dims { HEIGHT, WIDTH, Y, X, COUNT };
    enum class Layouts { HORIZONTAL, VERTICAL, HORIZONTAL_GRID, VERTICAL_GRID };
    enum class LayoutAlignments { LEFT, RIGHT, CENTER, JUSTIFIED };
    enum class ItemExpansion { FIXED, FILL, FIT };
}

class nucmd::Item {
public:
    Item();

    Item(const Item&) = default;
    Item(Item&&) = default;
    Item& operator=(const Item&) = default;
    Item& operator=(Item&&) = default;
    ~Item() = default;

    std::string name();
    void name(std::string);
    std::string text();
    void text(std::string);
    std::string label();
    void label(std::string);
    void setDim(int, int);
    int getDim(int);
private:
    std::string _name;
    std::string _text;
    std::string _label;
    std::vector<int> dims;
};

class nucmd::Pane {
public:
    Pane();

    ~Pane() = default;
    Pane(const Pane&) = default;
    Pane(Pane&&) = default;
    Pane& operator=(const Pane&) = default;
    Pane& operator=(Pane&&) = default;

    void setDim(int dim, int value);
    int getDim(int dim);
    void enableBorder();
    void disableBorder();
    void setLayout(int);
    void setLayoutAlignment(int);
    void setItemExpansion(int);
    void setItemWidth(int);
    void resize();
    void addItem(std::string name);
    void setItemSeparator(std::string separator);
    std::string getItemSeparator();
    void calculateItemsDimensions();
    void setItemsDimensions();
    void setItemsDimensionsFit();
    void setNoItems(int);
    void printItems();
    void printItem(Item item);
    void simplePrint(std::string text);
    void setActivable(bool);
    bool isActivable();
    void setActive(bool);
private:
    WINDOW* window;
    PANEL* panel;
    bool border_enabled;

    std::vector<int> dims;
    std::vector<Item> items;
    std::vector<int> horizontal_items;
    std::vector<int> vertical_items;
    int color_pair;
    int layout;
    int layout_alignment;
    int item_expansion;
    int left_margin;
    int right_margin;
    int top_margin;
    int bottom_margin;
    int no_items;
    int items_width;
    bool activable;
    bool active;
    std::string item_separator;
};

class nucmd::Nucmd {
public:
    Nucmd();
    ~Nucmd() = default;
    Nucmd(const Nucmd&) = default;
    Nucmd(Nucmd&&) = default;
    Nucmd& operator=(const Nucmd&) = default;
    Nucmd& operator=(Nucmd&&) = default;

    void run();

private:
    void initCurses();
    void initColors();
    void endCurses();
    void initPanes();
    void createPanes();
    void recalculatePanes();
    void resizePanes();
    void initMenuPane();
    void createMenuItems();
    void calculateMenuItemsDimensions();
    void putPanesContent();
    void drawPanes();
    void updatePanes();
    void toggleActivePane();
    void setActivePane(int);

    void handleInput();

    std::vector<Pane> panes;
    int key_pressed;
    int active_pane;
};
#endif // NUCMD_HPP
