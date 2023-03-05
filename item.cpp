#include "nucmd.hpp"

using namespace nucmd;

void Item::set_text(std::string text) {
    this->text = text;
}

std::string Item::get_text() {
    return this->text;
}

AbstractItemList::AbstractItemList() {
    selected_index = 0;
}

void ItemList::push_back(Item item) {
    items.push_back(item);
}

void ItemList::set_selected(int index) {
    selected_index = index;
}
