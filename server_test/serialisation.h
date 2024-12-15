#ifndef SERIALISATION_H
#define SERIALISATION_H

#include <cstring> 
#include <iostream>
#include "game_items.h"

void serialize(const objkt1& obj, char* buffer, size_t buffer_size);
objkt1 deserialize(const char* buffer);

#endif