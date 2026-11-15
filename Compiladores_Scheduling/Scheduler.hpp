#pragma once

#include <iostream>
#include <algorithm>
#include "ILOCOperators.hpp"
#include "GrafoPredSec.hpp"

struct Cycle {
    int numIter;
    std::vector<Node> UnitsExec;
};

class PriorityOrder {
public:
    bool operator() (const Node& a, const Node& b) const {
        if (a.priority != b.priority) {
            return a.priority > b.priority;
        }
        return a.instru.order < b.instru.order;
    }
};

class Scheduling {
private:
    std::string RUTA;
    std::vector<Cycle> Simulation;
public:
    Grafo grafito;
    uint UNITS;

    Scheduling(const uint& unidades, const std::string& path) : UNITS(unidades), RUTA(path) {};

    void ParseILOC() {
        std::ifstream leedor(RUTA);

        if (!leedor.is_open()) {
            std::cout << " No se pudo leer el archivo revisar :( " << std::endl;
            return;
        }

        std::string linea;
        int start = 1;
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

                grafito.FillGraph(instruction);
            }
        }

        leedor.close();
        //grafito.Imprimir(); POR SI IMPRIMIR LO DEL GRAFO
        grafito.FillPriorities();
    }


    void ForwardSchedule(const uint& UNITS) {
        Simulation.clear();

        std::vector<Node> schedule = grafito.grafo;
        std::sort(schedule.begin(), schedule.end(), PriorityOrder());

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

    void PrintSchedule() {
        for (Cycle& c : Simulation) {
            std::cout << c.numIter << ": ";
            for (int k = 0; k < c.UnitsExec.size(); k++) {
                if (k > 0) std::cout << " ; ";
                std::cout << c.UnitsExec[k].instru.fullIns;
            }
            std::cout << std::endl;
        }

        int length = 0;
        for (Node& nod : grafito.grafo) {
            int fin = nod.S + ParseOperaciones(Operations[nod.instru.operation]);
            length = std::max(length, fin);
        }
        std::cout << "length: " << length - 1 << " cycles" << std::endl;
    }

};