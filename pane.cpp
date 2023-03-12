#include "nucmd.hpp"

using namespace nucmd;

Pane::Pane()
    :no_items(1),
    border_enabled(true),
    layout(VERTICAL_LAYOUT),
    layout_alignment(LEFT_ALIGN),
    item_expansion(FIXED_ITEM_EXPAND),
    item_separator(" "),
    dims(DIMS_COUNT),
    left_margin(1),
    right_margin(1),
    top_margin(1),
    bottom_margin(1),
    items_width(0),
    color_pair(1),
    activable(false)
{
    window = newwin(0, 0, 0, 0);
    wattron(window, COLOR_PAIR(color_pair));
    wbkgdset(window, COLOR_PAIR(color_pair));
    panel = new_panel(window);
}

void Pane::setDim(int dim, int value){
    dims.at(dim) = value;
}

int Pane::getDim(int dim){
    return dims.at(dim);
}

void Pane::enableBorder(){
    border_enabled = true;
}

void Pane::disableBorder(){
    border_enabled = false;
}

void Pane::resize(){
    wresize(window, dims.at(HEIGHT_DIM), dims.at(WIDTH_DIM));
    mvwin(window, dims.at(Y_DIM), dims.at(X_DIM));
    if (border_enabled){
        werase(window);
        box(window, 0, 0);
    }
}

void Pane::setLayout(int layout){
    this->layout = layout;
}

void Pane::setLayoutAlignment(int alignment){
    layout_alignment = alignment;
}

void Pane::setItemExpansion(int expansion){
    item_expansion = expansion;
}

void Pane::setItemWidth(int width){
    items_width = width;
}

void Pane::addItem(std::string name){
    items.push_back(Item());
    items.back().name(name);
}

void Pane::setItemSeparator(std::string separator){
    item_separator = separator;
}

std::string Pane::getItemSeparator(){
    return item_separator;
}

void Pane::calculateItemsDimensions(){
    switch (item_expansion){
        case FIXED_ITEM_EXPAND:
            setItemsDimensions();
            break;
        case FILL_ITEM_EXPAND:
            items_width = getmaxx(window) - left_margin - right_margin;
            items_width -= (no_items - 1) * item_separator.length();
            items_width /= no_items;
            setItemsDimensions();
            break;
        case FIT_ITEM_EXPAND:
            setItemsDimensionsFit();
            break;
        default:
            break;
    }
}

void Pane::setItemsDimensions(){
    int x0 = left_margin;
    for (auto &item : items){
        item.setDim(WIDTH_DIM, items_width);
        item.setDim(HEIGHT_DIM, 1);
        item.setDim(X_DIM, x0);
        item.setDim(Y_DIM, top_margin);
        x0 += items_width + item_separator.length();
    }
}

void Pane::setItemsDimensionsFit(){
    int x0 = left_margin;
    for (auto &item : items){
        item.setDim(WIDTH_DIM, item.name().length());
        item.setDim(HEIGHT_DIM, 1);
        item.setDim(X_DIM, x0);
        item.setDim(Y_DIM, top_margin);
        x0 += item.name().length() + item_separator.length();
    }
}

void Pane::setNoItems(int value){
    no_items = value;
}

void Pane::printItems(){
    for (auto item : items){
        printItem(item);
    }
}

void Pane::printItem(Item item){
    int i = 0;
    for (i = 0; i < items_width; i++){
        if (i < item.name().length()){
            mvwaddch(window, item.getDim(Y_DIM), item.getDim(X_DIM) + i, item.name().at(i));
        } else {
            wmove(window, item.getDim(Y_DIM), item.getDim(X_DIM) + i);
        }
    }
    if (item.name() != items.back().name())
        mvwprintw(window, item.getDim(Y_DIM), item.getDim(X_DIM) + i, item_separator.c_str());
}

void Pane::simplePrint(std::string text){
    werase(window);
    if (border_enabled)
        box(window, 0, 0);
    mvwprintw(window, top_margin, left_margin, text.c_str());
    update_panels();
    doupdate();
}

void Pane::setActivable(bool value){
    activable = value;
}

bool Pane::isActivable(){
    return activable;
}
