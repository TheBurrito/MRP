#ifndef localization_h_
#define localization_h_

#include "core/types.h"

class Localization {
public:
  /**
   * Use the specified delta to perform the motion update step of localization.
   * @param current The current odometry pose of the robot
   */
  virtual void motionUpdate(const Pose& current)=0;

  /**
   * Perform the sensor update step of localization. Since there are no params
   * for this function, it is the responsibility of the subclass to know what
   * sensor model it will be using.
   */
  virtual void sensorUpdate()=0;

  /**
   * Gives the current best guess of the pose of the robot purely by
   * localization. The v member of the struct contains some measure of the
   * accuracy of the estimate and can be implementation dependant.
   * @return The pose estimate along with a measure of certainty.
   */
  virtual PoseV getPose()=0;
};

#endif /* localization_h_ */
