#include "serialisation.h" // Ensure this includes include/serialisation.h
#include <cstring> // For std::memcpy
#include <vector>


std::vector<char> serialize(const objkt1& obj) {
    std::vector<char> buffer;
    buffer.reserve(sizeof(int) * 5);

    int x = obj.get_x();
    int y = obj.get_y();
    int r = obj.get_r();
    int g = obj.get_g();
    int b = obj.get_b();
    // Serialize position & color
    buffer.insert(buffer.end(), reinterpret_cast<char*>(&x), reinterpret_cast<char*>(&x) + sizeof(int));
    buffer.insert(buffer.end(), reinterpret_cast<char*>(&y), reinterpret_cast<char*>(&y) + sizeof(int));
    buffer.insert(buffer.end(), reinterpret_cast<char*>(&r), reinterpret_cast<char*>(&r) + sizeof(int));
    buffer.insert(buffer.end(), reinterpret_cast<char*>(&g), reinterpret_cast<char*>(&g) + sizeof(int));
    buffer.insert(buffer.end(), reinterpret_cast<char*>(&b), reinterpret_cast<char*>(&b) + sizeof(int));

    // Serialize name length + name
    const std::string& nm = obj.get_name();
    int nameLen = static_cast<int>(nm.size());
    buffer.insert(buffer.end(), reinterpret_cast<char*>(&nameLen), reinterpret_cast<char*>(&nameLen) + sizeof(int));
    buffer.insert(buffer.end(), nm.begin(), nm.end());
    return buffer;
}
std::vector<char> serialize(const std::vector<objkt1>& objects) {
    std::vector<char> buffer;
    int count = static_cast<int>(objects.size());
    buffer.insert(buffer.end(), reinterpret_cast<char*>(&count), reinterpret_cast<char*>(&count) + sizeof(int));
    for(const auto& obj : objects) {
        std::vector<char> objData = serialize(obj);
        buffer.insert(buffer.end(), objData.begin(), objData.end());
    }
    return buffer;
}

objkt1 deserialize(const std::vector<char>& buffer) {
    if (buffer.size() < sizeof(int) * 5) {
        throw std::runtime_error("Insufficient buffer");
    }
    objkt1 obj;
    size_t offset = 0;
    int x, y, r, g, b;
    // Deserialize position & color
    std::memcpy(&x, &buffer[offset], sizeof(int)); offset += sizeof(int);
    std::memcpy(&y, &buffer[offset], sizeof(int)); offset += sizeof(int);
    std::memcpy(&r, &buffer[offset], sizeof(int)); offset += sizeof(int);
    std::memcpy(&g, &buffer[offset], sizeof(int)); offset += sizeof(int);
    std::memcpy(&b, &buffer[offset], sizeof(int)); offset += sizeof(int);
    obj.set(1, x);
    obj.set(2, y);
    obj.set_color(r, g, b);

    // Deserialize name length + name
    if (offset + sizeof(int) > buffer.size()) {
        return obj; // no name data
    }
    int nameLen;
    std::memcpy(&nameLen, &buffer[offset], sizeof(int));
    offset += sizeof(int);

    if (offset + nameLen <= buffer.size()) {
        std::string nm(buffer.begin() + offset, buffer.begin() + offset + nameLen);
        obj.set_name(nm);
        offset += nameLen;
    }
    return obj;
}

std::vector<objkt1> deserialize_vector(const std::vector<char>& buffer) {
    std::vector<objkt1> objects;
    size_t offset = 0;
    if (buffer.size() < sizeof(int)) {
        return objects;
    }
    int count = 0;
    std::memcpy(&count, &buffer[offset], sizeof(int));
    offset += sizeof(int);
    for(int i = 0; i < count; ++i) {
        // 5 ints for x,y,r,g,b + 1 int for nameLen + name
        // We just pass the remaining data to single-object deserializer
        if (offset >= buffer.size()) break;
        // Slice from offset to end
        std::vector<char> objBuffer(buffer.begin() + offset, buffer.end());
        objkt1 obj = deserialize(objBuffer);
        objects.push_back(obj);

        // Recalculate position used by single-object deserialization
        // -> x,y,r,g,b (5 ints) + nameLen + name
        // We do quick parse of nameLen after the 5 ints
        size_t consumed = sizeof(int) * 5; 
        if (objBuffer.size() > consumed) {
            int nameLen;
            std::memcpy(&nameLen, &objBuffer[consumed], sizeof(int));
            consumed += sizeof(int) + nameLen;
        }
        offset += consumed;
    }
    return objects;
}

/* still needs aes encryption
* serves at a small ssl to secure and encrypt communications
* between clients and server
*/