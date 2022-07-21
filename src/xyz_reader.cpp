#include "xyz_reader.h"
#include "system.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/* -------------------------------------------------------------------------
 * reads a xyz trajectory file.
 *
 * it saves in a vector each one of the frames as frame_type.
 *
 * ------------------------------------------------------------------------- */
std::vector<frame_type> xyz_reader(std::string fname) {
    std::string line, comment;
    std::ifstream xyztraj(fname, std::ios::in);

    std::vector<frame_type> frames;

    while (getline(xyztraj, line)) {
        frame_type frame;
        int natoms = stoi(line);
        frame.natoms = natoms;

        getline(xyztraj, comment);

        std::string *types = (std::string *)calloc(natoms, sizeof(std::string));
        float **positions = (float **)calloc(3, sizeof(float *));
        for (int i = 0; i < 3; ++i) {
            positions[i] = (float *)calloc(natoms, sizeof(float));
        }

        for (int i = 0; i < natoms; ++i) {
            getline(xyztraj, line);

            std::istringstream iss(line);
            std::vector<std::string> txyz = {};
            for (std::string s; iss >> s;)
                txyz.push_back(s);

            types[i] = txyz[0];
            for (int j = 0; j < 3; ++j) {
                std::istringstream os(txyz[j + 1]);
                os >> positions[j][i];
            }

            txyz.clear();
        }

        frame.types = types;
        frame.positions = positions;

        frames.push_back(frame);
    }
    xyztraj.close();

    return frames;
}
