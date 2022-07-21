#include "input.h"
#include "system.h"

#include <iostream>

/* -------------------------------------------------------------------------
 * set configuration from terminal in the following order:
 *
 * trajectory
 * central type of atoms
 * interact type of atoms
 * box size (x y z)
 * pbc (true or false)
 * inner rcut
 * outer rcut
 * number of categories
 * number of bins in partial rdfs
 *
 * or you can save this values in a file (in.config, for example) and run
 *
 * $ ./cluster-connections < in.config
 *
 * ------------------------------------------------------------------------- */
sysconfig_type input() {
    sysconfig_type cfg;

    std::cout << "Trajectory file: " << std::flush;
    std::cin >> cfg.FTRAJ;
    std::cout << cfg.FTRAJ << std::endl;

    std::cout << "Central type of atoms: " << std::flush;
    std::cin >> cfg.CENTRAL_ATOM;
    std::cout << cfg.CENTRAL_ATOM << std::endl;

    std::cout << "Interact type of atoms: " << std::flush;
    std::cin >> cfg.INTERACT_ATOM;
    std::cout << cfg.INTERACT_ATOM << std::endl;

    std::cout << "box size (in x, y, z direction): " << std::flush;
    for (int i = 0; i < 3; ++i)
        std::cin >> cfg.BOX[i];
    for (int i = 0; i < 3; ++i)
        std::cout << cfg.BOX[i] << "  " << std::flush;
    std::cout << std::endl;

    std::string pbc_str;
    std::cout << "pbc: " << std::flush;
    std::cin >> pbc_str;
    cfg.PBC = (pbc_str == "true") ? true : false;
    std::cout << cfg.PBC << std::endl;

    std::cout << "Inner cutoff radius: " << std::flush;
    std::cin >> cfg.INNER_RCUT;
    std::cout << cfg.INNER_RCUT << std::endl;

    std::cout << "Outer cutoff radius: " << std::flush;
    std::cin >> cfg.OUTER_RCUT;
    std::cout << cfg.OUTER_RCUT << std::endl;
    
    std::cout << "Number of categories: " << std::flush;
    std::cin >> cfg.NCATEGORIES;
    std::cout << cfg.NCATEGORIES << std::endl;

    std::cout << "Number of bins in the partials rdf: " << std::flush;
    std::cin >> cfg.BINS;
    std::cout << cfg.BINS << std::endl;

    cfg.dr = (cfg.OUTER_RCUT - cfg.INNER_RCUT) / static_cast<float>(cfg.BINS);

    return cfg;
}
