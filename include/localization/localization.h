#ifndef localization_h_
#define localization_h_

#include "core/types.h"

class Localization {
public:
  Pose getError(const Pose& robot, const Pose& dPose)=0;
};

#endif /* localization_h_ */
