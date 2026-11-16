#pragma once

#include <iostream>
#include <algorithm>
#include <random>
#include "ILOCOperators.hpp"
#include "GrafoPredSec.hpp"

struct Cycle {
    int numIter;
    std::vector<Node> UnitsExec;
};

class PriorityOrderForward {
public:
    bool operator() (const Node& a, const Node& b) const {
        if (a.priority != b.priority) {
            return a.priority > b.priority;
        }
        return a.tieKey < b.tieKey;
    }
};

class PriorityOrderBackward {
public:
    bool operator() (const Node& a, const Node& b) const {
        if (a.priority != b.priority) {
            return a.priority > b.priority;
        }
        return a.tieKey < b.tieKey;
    }
};

class Scheduling {
private:
    std::string RUTA;
    std::vector<Cycle> Simulation;
    int maxRegister;
    std::unordered_map<std::string, std::string> actual;
public:
    Grafo grafito;
    uint UNITS;
    bool rename;

    Scheduling(const uint& unidades, const std::string& path, const bool &renam = false) : UNITS(unidades), RUTA(path), rename(renam),maxRegister(1) {};

    void ParseILOC() {
        std::ifstream leedor(RUTA);

        if (!leedor.is_open()) {
            std::cout << " No se pudo leer el archivo revisar :( " << std::endl;
            return;
        }

        std::string linea;
        int start = 1;
        std::vector<Instruction> blockIns;
        while (std::getline(leedor, linea)) {
            if (!linea.empty() && linea.back() == '\r') {
                linea.pop_back();
            }

            std::istringstream entry(linea);

            std::string instru, operandos, flecha, destino;
            if (entry >> instru >> operandos >> flecha >> destino) {
                std::cout << "Se leyo: " << instru << " | " << operandos << " | " << flecha << " | " << destino << std::endl;

                Operaciones optr = Operations[instru];

                Instruction instruction;
                instruction.fullIns = linea;
                instruction.operation = instru;
                instruction.flecha = flecha;
                instruction.GetVector(operandos, instruction.operands);
                instruction.GetVector(destino, instruction.destinations);
                instruction.order = start++;

                blockIns.push_back(instruction);
                instruction.MaximizeRegister(instruction.operands, instruction.destinations, this->maxRegister);
            }
        }
        maxRegister++;
        leedor.close();


        for (auto block : blockIns) {
            if (rename) {
                Instruction::RenameRegister(actual, block, maxRegister);
            }

            grafito.FillGraph(block);
        }
        
        //grafito.Imprimir(); POR SI IMPRIMIR LO DEL GRAFO
    }


    void ForwardSchedule(const uint& UNITS) {
        Simulation.clear();

        std::vector<Node> schedule = grafito.grafo;
        std::sort(schedule.begin(), schedule.end(), PriorityOrderForward());

        int START = 1;
        int pendientes = schedule.size();
        while (pendientes > 0) {
            int FREE_UNITS = UNITS;
            std::vector<Node> Unit;

            for (const auto& nod : schedule) {
                if (FREE_UNITS == 0){
                    break;
                }

                Node& real = grafito.grafo[nod.instru.order - 1];
                if (real.S != -1) {
                    continue;
                }
                if (!grafito.AllPredsEnded(real, START)) {
                    continue;
                }

                real.S = START;
                FREE_UNITS--;
                pendientes--;
                Unit.push_back(real);
            }

            if (Unit.empty()) {
                Unit.push_back(Node{ {"nop"},{},{} });
            }
            Simulation.push_back({ START++, std::move(Unit) });
        }
    }

    void BackwardSchedule(const uint& UNITS) {
        Simulation.clear();

        std::vector<Node> schedule = grafito.grafo;
        std::sort(schedule.begin(), schedule.end(), PriorityOrderBackward());

        int START = 1;
        int pendientes = schedule.size();
        while (pendientes > 0) {
            int FREE_UNITS = UNITS;
            std::vector<Node> Unit;

            for (auto it = schedule.begin(); it != schedule.end(); it++) {
                if (FREE_UNITS == 0) {
                    break;
                }

                Node& real = grafito.grafo[(*it).instru.order - 1];
                if (real.S != -1) {
                    continue;
                }
                if (!grafito.AllSuccsEnded(real, START)) {
                    continue;
                }

                real.S = START;
                FREE_UNITS--;
                pendientes--;
                Unit.push_back(real);
            }

            if (Unit.empty()) {
                Unit.push_back(Node{ {"nop"},{},{} });
            }
            Simulation.push_back({ START++, std::move(Unit) });
        }

        int E = 0;
        for (Node& nod : grafito.grafo) {
            E = std::max(E, nod.S);
        }
        E += 1;

        for (Node& nod : grafito.grafo) {
            nod.S = E - nod.S;
        }

        std::reverse(Simulation.begin(), Simulation.end());
        for (Cycle& c : Simulation) {
            c.numIter = E - c.numIter;
        }
    }

    void PrintSchedule() {
        for (Cycle& c : Simulation) {
            std::cout << c.numIter << ": ";
            for (int k = 0; k < c.UnitsExec.size(); k++) {
                if (k > 0) std::cout << " ; ";
                std::cout << c.UnitsExec[k].instru.fullIns;
            }
            std::cout << std::endl;
        }

        std::cout << "longitud: " << GetLength() << " cycles" << std::endl;
    }

    int GetLength() {
        int length = 0;
        for (Node& nod : grafito.grafo) {
            int fin = nod.S + ParseOperaciones(Operations[nod.instru.operation]);
            length = std::max(length, fin);
        }
        return length - 1;
    }

    void RBF(const uint& UNITS) {
        std::mt19937 rng(std::random_device{}());
        std::vector<Node>& g = grafito.grafo;
        int n = g.size();

        std::vector<int> keys(n);
        for (int i = 0; i < n; i++) {
            keys[i] = i;
        }

        int bestLength = -1;
        std::vector<Cycle> bestSim;
        std::vector<int> bestS(n);

        for (int run = 0; run < 10; run++) {
            bool forward = run < 5;

            std::shuffle(keys.begin(), keys.end(), rng);
            for (int i = 0; i < n; i++) {
                g[i].tieKey = keys[i];
                g[i].S = -1;
            }

            if (forward) {
                grafito.FillPrioritiesForward();
                ForwardSchedule(UNITS);
            }
            else {
                grafito.FillPrioritiesBackward();
                BackwardSchedule(UNITS);
            }

            int len = GetLength();
            std::cerr << (forward ? "forward " : "backward ") << (run % 5) + 1 << ": " << len << std::endl;

            if (bestLength == -1 || len < bestLength) {
                bestLength = len;
                bestSim = Simulation;
                for (int i = 0; i < n; i++) {
                    bestS[i] = g[i].S;
                }
            }
        }

        Simulation = bestSim;
        for (int i = 0; i < n; i++) {
            g[i].S = bestS[i];
        }
    }

};