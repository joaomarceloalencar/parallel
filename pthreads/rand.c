/* simple reentrant substitution for rand */

#include "rand.h"

int myrand_r(int * seed_p)
{
  *seed_p = (9301 * (*seed_p) + 49297 ) % 233280;

  return(*seed_p);
}
