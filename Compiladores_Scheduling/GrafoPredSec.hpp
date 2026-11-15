#pragma once

#include <iostream>
#include "ILOCOperators.hpp"

enum AristaType {Verdadera,Anti,Salida,MemoriaSL,MemoriaLS,MemoriaSS};


struct RegisterInfo {
	int escritores = -1;
	std::vector<int> lectores;
};

struct Arista {
	AristaType tipo;
	int dest;
	int retardo;
};

struct Node {
	Instruction instru;
	std::vector<Arista> preds;
	std::vector<Arista> succs;
	int priority = -1;
	int S = -1;
};

class Grafo {
private:
	std::unordered_map<std::string, RegisterInfo> libreta;
	std::vector<int> MemoryList;
public:
	std::vector<Node> grafo;
	int GetRetardo(const Node& i, const Node& j, AristaType tip);
	bool GetConflict(const Node& i, const Node& j);
	void Add(Node &A,Node &B,AristaType tip);
	void FillGraph(const Instruction &instru);
	void Imprimir();
	void FillPriorities();
	bool AllPredsEnded(const Node& nod, int ciclo);
};