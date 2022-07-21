#include "distance.h"
#include "system.h"

#include <cmath>

/* -------------------------------------------------------------------------
 * distance between the position of two atoms r1 and r2.
 *
 * if pbc is true it is considered the minimum image convention, if it is
 * false, then it is calculated as if it were a non-periodic system.
 *
 * ------------------------------------------------------------------------- */
float distance(sysconfig_type cfg, float *r1, float *r2) {
    float rij = 0.0f;

    for (int i = 0; i < 3; ++i) {
        float d = r1[i] - r2[i];
        if (cfg.PBC) {
            while (d > 0.5f * cfg.BOX[i])
                d -= cfg.BOX[i];
            while (d < -0.5f * cfg.BOX[i])
                d += cfg.BOX[i];
        }
        rij += d * d;
    }

    rij = static_cast<float>(sqrt(rij));
    return rij;
}
