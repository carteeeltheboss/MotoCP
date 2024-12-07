#include "serialisation.h"

void kObjct::affiche(){
	std::cout << "Your id: " << this->id << std::endl;
	return;
}


void kObjct::kSet(int o) {this->id=o;}

void serialize(const kObjct& obj, char* buffer, size_t buffer_size) {
    if (buffer_size < sizeof(kObjct)) {
        printf(
        	"espace insuffisant dans le buffer, "
     		"veuillez reserver plus d'espace pour serializer l'objet d'entrez."
        	);
    }
    memcpy(buffer, &obj, sizeof(kObjct));
    return;
}

kObjct deserialize(const char* buffer) {
    kObjct obj;
    memcpy(&obj, buffer, sizeof(kObjct));
    return obj;
}

/*
int main(int argc, char const *argv[])
{
	printf("Program Starting ...");
	char buffer[512];
	kObjct q(13);
	q.affiche();
	serialize(q, buffer, 512);
	for(int i=0; i<512; i++){
		std::cout << buffer[i];
	}
		std::cout << std::endl;

	deserialize(buffer).affiche();
	return 0;
}
*/