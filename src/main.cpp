#include "connections.h"
#include "input.h"
#include "system.h"
#include "xyz_reader.h"

#include <chrono>
#include <iostream>
#include <vector>

int main() {
    const auto start = std::chrono::steady_clock::now();

    // clustering algorithm's main loop
    std::cout << "Setting the input configuration..." << std::endl;
    sysconfig_type cfg = input();

    std::cout << "Reading the trajectory file..." << std::endl;
    std::vector<frame_type> frames = xyz_reader(cfg.FTRAJ);

    std::cout << "Performing the cluster connections..." << std::endl;
    ClusterConnections *connections = new ClusterConnections(cfg);
    connections->run(frames);

    std::cout << "Writing output 'cluster-connections.csv'..." << std::endl;
    connections->to_csv("cluster-connections.csv", "r,g0,g1,g2,g3,g4,gelse");

    frames.clear();
    delete connections;
    // end

    const auto stop = std::chrono::steady_clock::now();
    float elapsed = std::chrono::duration<float>(stop - start).count();
    std::cout << "elapsed time [s]: " << elapsed << std::endl;
    return 0;
}
