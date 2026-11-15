#include <iostream>
#include "Scheduler.hpp"

int main(int argc,char *argv[]) {
    if (argc < 3) {
        throw std::runtime_error("Se dio una cantidad menor a 2 argumentos");
    }

    Scheduling schedule(std::stoi(argv[1]),argv[2]);
    schedule.ParseILOC();
    schedule.ForwardSchedule(schedule.UNITS);
    schedule.PrintSchedule();
}