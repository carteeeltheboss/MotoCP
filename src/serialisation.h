#ifndef SERIALISATION_H
#define SERIALISATION_H

#include "game_items.h"
#include <cstring>
#include <vector>

std::vector<char> serialize(const objkt1& obj);
objkt1 deserialize(const std::vector<char>& buffer);

#endif