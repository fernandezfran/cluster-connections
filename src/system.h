#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>

struct sysconfig_type {
    std::string FTRAJ, CENTRAL_ATOM, INTERACT_ATOM;
    bool PBC;
    int NCATEGORIES, BINS;
    float BOX[3], INNER_RCUT, OUTER_RCUT;
    float dr;
};

struct frame_type {
    int natoms;
    std::string *types;
    float **positions;
};

#endif
