#include <iostream>
#include "Scheduler.hpp"

int main(int argc,char *argv[]) {
    if (argc < 3) {
        throw std::runtime_error("Se dio una cantidad menor a 2 argumentos");
    }

    Scheduling schedule(std::stoi(argv[1]), argv[argc - 1]);
    if (argc == 3) {
        schedule.ParseILOC();
        schedule.grafito.FillPrioritiesForward();
        schedule.ForwardSchedule(schedule.UNITS);
        schedule.PrintSchedule();
    }
    else {
        bool rename=false, backward=false, rbf=false;
        
        for (int i = 2; i < argc - 1; i++) {
            if (std::string(argv[i]) == "-r") {
                rename = true;
            }

            if (std::string(argv[i]) == "-b") {
                backward = true;
            }

            if (std::string(argv[i]) == "-rbf") {
                rbf = true;
            }
            
        }

        if (rename) {
            schedule.rename = true;
        }
        schedule.ParseILOC();

        if (backward) {
            schedule.grafito.FillPrioritiesBackward();
            schedule.BackwardSchedule(schedule.UNITS);
        }
        else if (rbf) {
            schedule.RBF(schedule.UNITS);
        }
        else {
            schedule.grafito.FillPrioritiesForward();
            schedule.ForwardSchedule(schedule.UNITS);
        }

        schedule.PrintSchedule();
    }

    
    
}