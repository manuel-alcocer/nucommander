#include "nucmd.hpp"

using namespace nucmd;

Nucmd::Nucmd()
    :active_pane(-1)
{
    initCurses();
    initColors();
    initPanes();
    initMenuPane();
    
    putPanesContent();
    drawPanes();
    updatePanes();
}

void Nucmd::run(){
    while ((key_pressed = getch()) != QUIT_KEY){
        handleInput();
    }
    endCurses();
}

void Nucmd::initCurses(){
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    raw();
    start_color();
    refresh();
}

void Nucmd::initColors(){
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
}

void Nucmd::endCurses(){
    endwin();
}

void Nucmd::initPanes(){
    createPanes();

    recalculatePanes();
    resizePanes();
}

void Nucmd::createPanes(){
    for (int i = 0; i < PANES_COUNT; i++){
        panes.push_back(Pane());
    }

    panes.at(TOP_PANE).setDim(HEIGHT_DIM, TOP_PANE_HEIGHT);
    panes.at(TOP_PANE).setDim(Y_DIM, 0);
    panes.at(TOP_PANE).setDim(X_DIM, 0);

    panes.at(LEFT_PANE).setDim(Y_DIM, TOP_PANE_HEIGHT);
    panes.at(LEFT_PANE).setDim(X_DIM, 0);
    panes.at(LEFT_PANE).setActivable(true);

    panes.at(RIGHT_PANE).setDim(Y_DIM, TOP_PANE_HEIGHT);
    panes.at(RIGHT_PANE).setActivable(true);

    panes.at(BOTTOM_PANE).setDim(HEIGHT_DIM, BOTTOM_PANE_HEIGHT);
    panes.at(BOTTOM_PANE).setDim(X_DIM, 0);

    panes.at(MENU_PANE).setLayout(HORIZONTAL_LAYOUT);
    panes.at(MENU_PANE).setItemExpansion(FILL_ITEM_EXPAND);
}

void Nucmd::recalculatePanes(){
    panes.at(TOP_PANE).setDim(WIDTH_DIM, COLS);

    panes.at(LEFT_PANE).setDim(
        HEIGHT_DIM,
        LINES -
            panes.at(TOP_PANE).getDim(HEIGHT_DIM) -
            panes.at(BOTTOM_PANE).getDim(HEIGHT_DIM)
        );
    panes.at(LEFT_PANE).setDim(WIDTH_DIM, LEFT_PANE_WIDTH);

    panes.at(RIGHT_PANE).setDim(
        HEIGHT_DIM,
        panes.at(LEFT_PANE).getDim(HEIGHT_DIM)
        );
    panes.at(RIGHT_PANE).setDim(WIDTH_DIM, COLS - panes.at(LEFT_PANE).getDim(WIDTH_DIM));
    panes.at(RIGHT_PANE).setDim(X_DIM, LEFT_PANE_WIDTH);

    panes.at(BOTTOM_PANE).setDim(Y_DIM, LINES - panes.at(BOTTOM_PANE).getDim(HEIGHT_DIM));
    panes.at(BOTTOM_PANE).setDim(WIDTH_DIM, COLS);
}

void Nucmd::resizePanes(){
    for (auto& pane : panes)
        pane.resize();
}

void Nucmd::initMenuPane(){
    createMenuItems();
    calculateMenuItemsDimensions();
}

void Nucmd::createMenuItems(){
    panes.at(MENU_PANE).addItem("File");
    panes.at(MENU_PANE).addItem("Edit");
    panes.at(MENU_PANE).addItem("Settings");
    panes.at(MENU_PANE).addItem("Help");

    panes.at(MENU_PANE).setNoItems(4);
    panes.at(MENU_PANE).setItemWidth(13);
    panes.at(MENU_PANE).setItemExpansion(FIT_ITEM_EXPAND);
}

void Nucmd::calculateMenuItemsDimensions(){
    panes.at(MENU_PANE).calculateItemsDimensions();
}

void Nucmd::putPanesContent(){
    for (auto& pane : panes){
        pane.printItems();
    }
}

void Nucmd::drawPanes(){
    update_panels();
}

void Nucmd::updatePanes(){
    doupdate();
}

int main(int argc, char *argv[]){
    Nucmd nucmd;
    
    nucmd.run();
    
    return 0;
}

void Nucmd::toggleActivePane(){
    if (active_pane == -1)
        active_pane = 0;
    else {
        active_pane++;
        active_pane %= (panes.size()+1);
    }
    if (active_pane >= panes.size())
        active_pane = -1;
    else if (panes.at(active_pane).isActivable() == false)
        toggleActivePane();
}

void Nucmd::setActivePane(int pane){
    for (auto& p : panes){
        p.setActive(false);
    }
    if (pane != -1)
        panes.at(pane).setActive(true);
}
void Nucmd::handleInput(){
    std::string text;
    std::string key_name = keyname(key_pressed);
    switch (key_pressed){
        case KEY_RESIZE:
            text = "Resizing";
            recalculatePanes();
            resizePanes();
            calculateMenuItemsDimensions();
            putPanesContent();
            drawPanes();
            updatePanes();
            break;
        case KEY_TAB:
            toggleActivePane();
            setActivePane(active_pane);
            text = "Changing active pane to: " + std::to_string(active_pane);
            break;
        case KEY_ESC:
            nodelay(stdscr, TRUE);
            if (getch() == ERR){
                text = "Escaping";
            } else {
                text = "No Escaping";
            }
            nodelay(stdscr, FALSE);
            break;
        default:
            break;
    }
    panes.at(STATUS_PANE).simplePrint(key_name + " --> " + text);
};
