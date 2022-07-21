#include "connections.h"
#include "distance.h"
#include "system.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>

/* -------------------------------------------------------------------------
 * cluster connections initialization.
 *
 * it allocates the memory for the partial rdfs to accumulate data and define
 * the system configuration.
 *
 * ------------------------------------------------------------------------- */
void ClusterConnections::init() {
    partial_rdfs = (int **)calloc(cfg.NCATEGORIES, sizeof(int *));
    for (int i = 0; i < cfg.NCATEGORIES; ++i) {
        partial_rdfs[i] = (int *)calloc(cfg.BINS, sizeof(int));
    }
}

/* -------------------------------------------------------------------------
 * cluster connections of a single frame.
 *
 * for each central atom it calculates the number of interconnecting neighbors
 * with its second nearest neighbors and accumulates the distance in the
 * corresponding histogram depending on this number.
 *
 * ------------------------------------------------------------------------- */
void ClusterConnections::step(frame_type frame) {
    float ri[3], rj[3], rk[3];

    for (int i = 0; i < frame.natoms; ++i) {

        // I stand only in the central atoms
        if (frame.types[i] != cfg.CENTRAL_ATOM) continue;

        for (int d = 0; d < 3; ++d)
            ri[d] = frame.positions[d][i];

        std::vector<int> connectors = {};
        for (int j = 0; j < frame.natoms; ++j) {

            // I only considered interact type atoms
            if (frame.types[j] != cfg.INTERACT_ATOM) continue;

            for (int d = 0; d < 3; ++d)
                rj[d] = frame.positions[d][j];

            // fill a vector with the idxs of interacting neighbors in the
            // first coordination shell
            float rij = distance(cfg, ri, rj);
            if (rij <= cfg.INNER_RCUT) connectors.push_back(j);
        }

        for (int j = 0; j < frame.natoms; ++j) {

            // I only considered interact type atoms
            if (frame.types[j] != cfg.INTERACT_ATOM) continue;

            for (int d = 0; d < 3; ++d)
                rj[d] = frame.positions[d][j];

            // check how many atoms of the interacting type interact with the
            // first neighbors of the central atoms
            int nconnectors = 0;
            for (int &connector : connectors) {
                for (int d = 0; d < 3; ++d)
                    rk[d] = frame.positions[d][connector];

                float rjk = distance(cfg, rj, rk);
                if (rjk <= cfg.INNER_RCUT) ++nconnectors;
            }

            // add to the corresponding partial rdf the info of atom i with j,
            // if the distance is in [rmin, rmax).
            float rij = distance(cfg, ri, rj);
            if (rij >= cfg.OUTER_RCUT || rij < cfg.INNER_RCUT) continue;

            int category = std::min(nconnectors, cfg.NCATEGORIES - 1);
            int ig = static_cast<int>((rij - cfg.INNER_RCUT) / cfg.dr);
            ++partial_rdfs[category][ig];
        }
        connectors.clear();
    }
}

/* -------------------------------------------------------------------------
 * cluster connections final step.
 *
 * it normalize the data of partial rdfs.
 *
 * ------------------------------------------------------------------------- */
void ClusterConnections::end(frame_type frame, int nframes) {
    float nframesf = static_cast<float>(nframes);

    int central_natoms = 0;
    int interact_natoms = 0;
    for (int i = 0; i < frame.natoms; ++i) {
        if (frame.types[i] == cfg.CENTRAL_ATOM) {
            ++central_natoms;
        } else if (frame.types[i] == cfg.INTERACT_ATOM) {
            ++interact_natoms;
        }
    }
    float volume = cfg.BOX[0] * cfg.BOX[1] * cfg.BOX[2];
    float rho = static_cast<float>(central_natoms * interact_natoms) / volume;

    float factor = static_cast<float>(4.0 * M_PI * rho / 3.0);

    fpartial_rdfs = (float **)calloc(cfg.NCATEGORIES, sizeof(float *));
    for (int i = 0; i < cfg.NCATEGORIES; ++i) {
        fpartial_rdfs[i] = (float *)calloc(cfg.BINS, sizeof(float));
    }

    int offset = static_cast<int>(cfg.INNER_RCUT / cfg.dr);
    float dr3 = cfg.dr * cfg.dr * cfg.dr;
    for (int i = 0; i < cfg.BINS; ++i) {
        int outer = (offset + i + 1) * (offset + i + 1) * (offset + i + 1);
        int inner = (offset + i) * (offset + i) * (offset + i);
        float shell = static_cast<float>(outer - inner) * dr3;

        float ideal = factor * shell;

        for (int j = 0; j < cfg.NCATEGORIES; ++j) {
            float gofr = static_cast<float>(partial_rdfs[j][i]);
            fpartial_rdfs[j][i] = gofr / (ideal * nframesf);
        }
    }
}

/* -------------------------------------------------------------------------
 * cluster connections analysis main loop.
 * ------------------------------------------------------------------------- */
void ClusterConnections::run(std::vector<frame_type> frames, int print_every) {
    init();

    int nframes = 0;
    for (frame_type &frame : frames) {
        step(frame);
        if (++nframes % print_every == 0)
            std::cout << nframes << " frames calculated. " << std::endl;
    }

    frame_type last_frame = frames[nframes - 1];
    end(last_frame, nframes);
}

/* -------------------------------------------------------------------------
 * write the output of the partial rdfs to a csv file.
 * ------------------------------------------------------------------------- */
void ClusterConnections::to_csv(std::string fname, std::string header) {
    std::ofstream csv_file;

    csv_file.open(fname, std::ios::out);

    csv_file << header << std::endl;

    for (int i = 0; i < cfg.BINS; ++i) {

        csv_file << cfg.INNER_RCUT + static_cast<float>(i) * cfg.dr << std::flush;

        for (int j = 0; j < cfg.NCATEGORIES; ++j) {
            csv_file << "," << fpartial_rdfs[j][i] << std::flush;
        }
        csv_file << std::endl;
    }
    csv_file.close();
}
