#include "GrafoPredSec.hpp"
#include <utility>


static bool GetRegistro(const std::string& str) {
    return !str.empty() && str[0] == 'r';
}

static bool GetLoad(const std::string& str) {
    return !str.empty() && Operations[str] == loadAI;
}

static bool GetStore(const std::string& str) {
    return !str.empty() && Operations[str] == storeAI;
}

static std::vector<std::string> GetLee(const Instruction& i) {
    std::vector<std::string> lee;
    for (const auto& s : i.operands) {
        if (GetRegistro(s)) {
            lee.push_back(s);
        }
    }
    if (GetStore(i.operation) && !i.destinations.empty()){
        lee.push_back(i.destinations[0]);
    }
    return lee;
}


static std::string GetEscribe(const Instruction& i) {
    if (GetStore(i.operation) || i.destinations.empty()) {
        return "";
    }
    return GetRegistro(i.destinations[0]) ? i.destinations[0] : "";
}

static const std::vector<std::string>& GetDireccion(const Instruction& i) {
    return GetStore(i.operation) ? i.destinations : i.operands;
}

int Grafo::GetRetardo(const Node &i,const Node &j,AristaType tip) {
	switch (tip) {
		case Verdadera:
		case Salida:
		case MemoriaSL:
        case MemoriaSS:
			return ParseOperaciones(Operations[i.instru.operation]);
		case Anti:
		case MemoriaLS:
			return 1;
		default:
			return -1;
	}
}

bool Grafo::GetConflict(const Node& i, const Node& j) {
    Operaciones opt1 = Operations[i.instru.operation];
    Operaciones opt2 = Operations[j.instru.operation];
    bool equal = opt1 == opt2;

    if (equal && (opt1 == loadAI)) {
        return false;
    }

    std::pair<std::string, std::string> par1 = opt1 == storeAI ? std::make_pair(i.instru.destinations[0], i.instru.destinations[1]) : std::make_pair(i.instru.operands[0], i.instru.operands[1]);
    std::pair<std::string, std::string> par2 = opt2 == storeAI ? std::make_pair(j.instru.destinations[0], j.instru.destinations[1]) : std::make_pair(j.instru.operands[0], j.instru.operands[1]);

    if (par1.first == par2.first) {
        if (par1.second == par2.second) {
            return true;
        }
        return false;
    }
    return true;
}

void Grafo::Add(Node& A,Node &B, AristaType tip) {
    int a = A.instru.order - 1;
    int b = B.instru.order - 1;
    int retardo = GetRetardo(A,B,tip);

    for (auto& p : grafo[b].preds) {
        if (p.dest == a) {
            if (retardo > p.retardo) {
                p.retardo = retardo;
                for (auto& s : grafo[a].succs) {
                    if (s.dest == b) {
                        s.retardo = retardo;
                        break;
                    }
                }
            }
            return;
        }
    }

    grafo[a].succs.push_back({ tip, b, retardo });
    grafo[b].preds.push_back({ tip, a, retardo });
}

void Grafo::FillGraph(const Instruction &instru) {
    Node tmpNode{ instru,{},{}};
    grafo.push_back(tmpNode);

    int b = grafo.size() - 1;
    const Instruction& ins = grafo[b].instru;

    std::vector<std::string> lee = GetLee(ins);
    std::string escribe = GetEscribe(ins);
    bool esMem = GetLoad(ins.operation) || GetStore(ins.operation);

    for (auto regis : lee) {
        int& escri = libreta[regis].escritores;
        if (escri != -1) {
            Add(grafo[escri],grafo[b],Verdadera);
        }
    }

    if (!escribe.empty()) {
        RegisterInfo& info = libreta[escribe];
        if (info.escritores != -1){
            Add(grafo[info.escritores], grafo[b], Salida);
        }
        for (int L : info.lectores) {
            if (L != b) {
                Add(grafo[L], grafo[b], Anti);
            }
        }
    }

    if (esMem) {
        for (int m : MemoryList) {
            if (!GetConflict(grafo[m], grafo[b])) {
                continue;
            }

            bool mStore = GetStore(grafo[m].instru.operation);
            bool bStore = GetStore(ins.operation);
            AristaType t;

            if (mStore && bStore) {
                t = MemoriaSS;
            }
            else if (mStore) {
                t = MemoriaSL;
            }
            else {
                t = MemoriaLS;
            }
            Add(grafo[m], grafo[b], t);
        }
    }

    for (const auto& r : lee) {
        libreta[r].lectores.push_back(b);
    }
    if (!escribe.empty()) {
        libreta[escribe].escritores = b;
        libreta[escribe].lectores.clear();
    }
    if (esMem){
        MemoryList.push_back(b);
    }
}

static const char* NombreTipo(AristaType t) {
    switch (t) {
        case Verdadera: 
            return "Verdadera";
        case Anti:      
            return "Anti";
        case Salida:    
            return "Salida";
        case MemoriaSL: 
            return "MemoriaSL";
        case MemoriaLS: 
            return "MemoriaLS";
        case MemoriaSS: 
            return "MemoriaSS";
    }
    return "?";
}

void Grafo::Imprimir() {
    for (const Node& n : grafo) {
        std::cout << n.instru.order << ": " << n.instru.fullIns << std::endl << "   preds:";
        for (const Arista& a : n.preds)
            std::cout << " (" << a.dest + 1 << ", " << a.retardo << ", " << NombreTipo(a.tipo) << ")";
        std::cout << std::endl <<"   succs:";
        for (const Arista& a : n.succs)
            std::cout << " (" << a.dest + 1 << ", " << a.retardo << ", " << NombreTipo(a.tipo) << ")";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Grafo::FillPriorities() {
    for (auto it = grafo.rbegin(); it != grafo.rend(); it++) {
        Node& tmp = *it;

        int maxPrio = 0;
        for (auto succ : tmp.succs) {
            Node& visit = grafo[succ.dest];
            maxPrio = std::max(maxPrio, visit.priority);
        }

        tmp.priority = ParseOperaciones(Operations[tmp.instru.operation]) + maxPrio;
    }
}


bool Grafo::AllPredsEnded(const Node& nod, int ciclo) {
    for (auto& p : nod.preds) {
        const Node& pred = grafo[p.dest];
        if (pred.S == -1) {
            return false;
        }
        if (pred.S + p.retardo > ciclo) {
            return false;
        }
    }
    return true;
}