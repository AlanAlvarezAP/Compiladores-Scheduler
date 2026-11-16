#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include <iomanip>

typedef unsigned int uint;

enum Operaciones { loadI, loadAI, storeAI, add, sub, mult, lshift, rshift, nop };
static std::unordered_map<std::string, Operaciones> Operations = { {"loadI", loadI},{"loadAI", loadAI},{"storeAI", storeAI},{"add", add},{"sub", sub},{"mult", mult},{"lshift", lshift},{"rshift", rshift},{"nop", nop} };

static int ParseOperaciones(Operaciones op) {
    switch (op) {
    case loadI:
    case add:
    case sub:
    case lshift:
    case rshift:
    case nop:
        return 1;
    case loadAI:
    case storeAI:
        return 3;
    case mult:
        return 2;
    default:
        return -1;
    }
}

struct Instruction {
    std::string fullIns;
    std::string operation;
    std::vector<std::string> operands;
    std::string flecha;
    std::vector<std::string> destinations;
    int order;

    void GetVector(std::string& str, std::vector<std::string>& resultado) {
        std::istringstream entrada(str);

        std::string part;
        while (std::getline(entrada, part, ',')) {
            resultado.push_back(part);
        }
    }

    void MaximizeRegister(const std::vector<std::string>& operan, const std::vector<std::string>& dest, int &MaxRegister) {
        for (const auto &op : operan) {
            if (op[0] == 'r') {
                int tmpMax = std::stoi(op.substr(1));
                MaxRegister = tmpMax > MaxRegister ? tmpMax : MaxRegister;
            }
        }

        for (const auto &de : dest) {
            if (de[0] == 'r') {
                int tmpMax = std::stoi(de.substr(1));
                MaxRegister = tmpMax > MaxRegister ? tmpMax : MaxRegister;
            }
        }
    }

    static void RenameRegister(std::unordered_map<std::string, std::string> &actual, Instruction& ins, int& MaxRegister) {
        bool esStore = ins.operation == "storeAI";

        for (auto &op : ins.operands) {
            if (op[0] == 'r' && op != "r0" && actual.find(op) != actual.end()) {
                op = actual[op];
            }
        }
        if (esStore && !ins.destinations.empty()) {
            std::string& base = ins.destinations[0];
            if (base[0] == 'r' && base != "r0" && actual.find(base) != actual.end()) {
                base = actual[base];
            }
        }

        if (!esStore && !ins.destinations.empty()) {
            std::string& de = ins.destinations[0];
            if (de[0] == 'r' && de != "r0") {
                std::string newStr = "r" + std::to_string(MaxRegister++);
                actual[de] = newStr;
                de = newStr;
            }
        }

        std::string ops, dests;
        for (int k = 0; k < ins.operands.size(); k++) {
            if (k > 0) {
                ops += ",";
            }
            ops += ins.operands[k];
        }
        for (int k = 0; k < ins.destinations.size(); k++) {
            if (k > 0) {
                dests += ",";
            }
            dests += ins.destinations[k];
        }
        ins.fullIns = ins.operation + " " + ops + " => " + dests;
    }
};