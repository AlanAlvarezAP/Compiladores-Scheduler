#pragma once

#include <iostream>
#include "ILOCOperators.hpp"

enum AristaType {Verdadera,Anti,Salida,Memoria};

struct Arista {
	AristaType tipo;
	int dest;
	int retardo;
};

struct Node {
	Instruction instru;
	std::vector<Arista> preds;
	std::vector<Arista> succs;
	int priority;
	int S;
};

class Grafo {
public:
	std::vector<Node> grafo;

	void Add(Instruction ins);
	void Imprimir();
};