#ifndef SERIALISATION_H
#define SERIALISATION_H

#include <vector>
#include <string>
#include "game_items.h" // Assuming objkt1 is defined here

// Serialize a vector of objkt1 objects into a byte buffer
std::vector<char> serialize(const std::vector<objkt1>& objects);

// Serialize a single objkt1 object into a byte buffer
std::vector<char> serialize(const objkt1& obj);

// Deserialize a byte buffer into a vector of objkt1 objects
std::vector<objkt1> deserialize_vector(const std::vector<char>& buffer);

// Deserialize a byte buffer into a single objkt1 object
objkt1 deserialize(const std::vector<char>& buffer);

#endif // SERIALISATION_H