#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include "system.h"

#include <vector>

class ClusterConnections {
  private:
    sysconfig_type cfg;
    int **partial_rdfs;
    float **fpartial_rdfs;

  private:
    void init();
    void step(frame_type frame);
    void end(frame_type frame, int nframes);

  public:
    ClusterConnections(sysconfig_type sysconfig) : cfg{sysconfig} {};
    ~ClusterConnections() {
        for (int i = 0; i < 6; ++i) {
            free(partial_rdfs[i]);
            free(fpartial_rdfs[i]);
        }
    };
    void run(std::vector<frame_type> frames, int print_every = 10);
    void to_csv(std::string fname, std::string header);
};

#endif
