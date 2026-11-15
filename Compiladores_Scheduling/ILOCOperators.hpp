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
};