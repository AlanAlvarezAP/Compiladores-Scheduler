#include <iostream>
#include "Scheduler.hpp"


/*void SimulateIterations(Scheduling& schedule) {
    auto& pq = schedule.pq;

    const uint anchoCiclo = 8;
    const uint anchoUnidad = 30;

    std::cout << std::left;
    std::cout << std::setw(anchoCiclo) << "Ciclo" << " | ";
    for (int i = 1; i <= schedule.UNITS; i++) {
        std::cout << std::setw(anchoUnidad) << ("Unidad " + std::to_string(i)) << " | ";
    }
    std::cout << std::endl;

    std::cout << std::string(anchoCiclo + 3 + UNIDAD * (anchoUnidad + 3), '-') << std::endl;
    while (!pq.empty()) {
        std::cout << std::setw(anchoCiclo) << START++ << " | ";

        for (int i = 1; i <= schedule.UNITS; i++) {
            std::string texto;

            if (!pq.empty()) {
                auto pp = pq.top();
                pq.pop();
                texto = pp.ins.fullIns + " (" + std::to_string(pp.cost) + ")";
            }
            std::cout << std::setw(anchoUnidad) << texto << " | ";
        }
        std::cout << std::endl;
    }
    std::cout << std::right;
}*/

int main(int argc,char *argv[]) {
    if (argc < 3) {
        throw std::runtime_error("Se dio una cantidad menor a 2 argumentos");
    }

    Scheduling schedule(std::stoi(argv[1]),argv[2]);
    schedule.ParseILOC();
    
    //SimulateIterations(schedule);
}