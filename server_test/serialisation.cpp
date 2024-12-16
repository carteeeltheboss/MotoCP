#include "serialisation.h"

void serialize(const objkt1& obj, char* buffer, size_t buffer_size) {
    if (buffer_size < sizeof(objkt1)) {
        printf(
        	"espace insuffisant dans le buffer, "
     		"veuillez reserver plus d'espace pour serializer l'objet d'entrez."
        	);
    }
    memcpy(buffer, &obj, sizeof(objkt1));
    return;
}

objkt1 deserialize(const char* buffer) {
    objkt1 obj;
    memcpy(&obj, buffer, sizeof(objkt1));
    return obj;
}

// still needs aes encryption
// serves at a small ssl to secure and encrypt communications
// between clients and server
