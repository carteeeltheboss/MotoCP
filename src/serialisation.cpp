#include "serialisation.h"

std::vector<char> serialize(const objkt1& obj) {
    std::vector<char> buffer(sizeof(objkt1));
    memcpy(buffer.data(), &obj, sizeof(objkt1));
    return buffer;
}

objkt1 deserialize(const std::vector<char>& buffer) {
    if (buffer.size() < sizeof(objkt1)) {
        throw std::runtime_error("Insufficient buffer size for deserialization.");
    }
    objkt1 obj;
    memcpy(&obj, buffer.data(), sizeof(objkt1));
    return obj;
}





/*
void serialize(const objkt1& obj, char* buffer, size_t buffer_size) {
    if (buffer_size < sizeof(objkt1)) {
        printf(
        	"espace insuffisant dans le buffer, "
     		"veuillez reserver plus d'espace pour serializer l'objet d'entrez."
        	);
    }
    // memcpy can only work when endianess is invariable
    memcpy(buffer, &obj, sizeof(objkt1));
    return;
}

objkt1 deserialize(const char* buffer) {
    objkt1 obj;
    memcpy(&obj, buffer, sizeof(objkt1));
    return obj;
}
*/

/*
void serialize(const objkt1& obj, char*& buffer, size_t buffer_size) {
    // Ensure the buffer is large enough
    if (buffer_size < sizeof(objkt1)) {
        throw std::runtime_error("Insufficient buffer size for serialization.");
    }
    memcpy(buffer, &obj, sizeof(objkt1));
}

objkt1 deserialize(const char* buffer, size_t buffer_size) {
    if (buffer_size < sizeof(objkt1)) {
        throw std::runtime_error("Insufficient buffer size for deserialization.");
    }
    objkt1 obj;
    memcpy(&obj, buffer, sizeof(objkt1));
    return obj;
}
*/

/* still needs aes encryption
* serves at a small ssl to secure and encrypt communications
* between clients and server
*/