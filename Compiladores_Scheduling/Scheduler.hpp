#pragma once

#include <iostream>
#include "ILOCOperators.hpp"
#include "GrafoPredSec.hpp"

/*struct UnitStruct {
    Operaciones optr;
    int cost;
    bool finished = false;
    Instruction ins;

    bool operator<(const UnitStruct& right) const {
        return this->cost < right.cost;
    }
};*/

class Scheduling {
private:
    std::string RUTA;

public:
    //std::priority_queue<UnitStruct> pq;
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

                //pq.push({ optr, ParseOperaciones(optr), false, instruction });
            }
        }

        leedor.close();
    }

};