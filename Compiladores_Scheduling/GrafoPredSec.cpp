#include "GrafoPredSec.hpp"

void Grafo::Add(Instruction ins){
	grafo.push_back({ins,{},{},-1,-1});
}

void Grafo::Imprimir() {
	for (auto nod : this->grafo) {
		std::cout << "Nodo -> " << nod.instru.order << " con instruccion " << nod.instru.fullIns << std::endl;
	}
	std::cout << std::endl;
}