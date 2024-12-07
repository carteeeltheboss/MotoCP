#ifndef SERIALISATION_H
#define SERIALISATION_H

#include <cstring> 
#include <iostream>

class kObjct{
	private:
		int id;
	public:
		kObjct(int k=0): id(k) {}
		void affiche();
		void kSet(int o);
};

void serialize(const kObjct& obj, char* buffer, size_t buffer_size);
kObjct deserialize(const char* buffer);

#endif