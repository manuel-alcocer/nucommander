#include "nucmd.hpp"

using namespace nucmd;

Item::Item()
    : _name("")
    , _text("")
    , _label("")
    , dims{0,0,0,0}
{}

std::string Item::name(){
    return _name;
}

void Item::name(std::string name){
    _name = name;
}

std::string Item::text(){
    return _text;
}

void Item::text(std::string text){
    _text = text;
}

std::string Item::label(){
    return _label;
}

void Item::label(std::string label){
    _label = label;
}

void Item::setDim(int dim, int value){
    dims.at(dim) = value;
}

int Item::getDim(int dim){
    return dims.at(dim);
}
