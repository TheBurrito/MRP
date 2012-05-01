#ifndef sonarmodel_h_
#define sonarmodel_h_

#include "sensor/sensor-sonar.h"
#include "mapping/map.h"

class SimpleSonarModel {
public:
  /**
   * Creates an iterative sonar model. Uses nested for loops to "fill" in an
   * arc with data.
   *
   * @param sonar The sonar to use for range readings.
   * @param dTh The angle step to take when sweeping across the beam.
   * @param dd  The distance step when tracing lines out from the sensor.
   * @param oddObs  The odds to use when detecting an obstacle.
   * @param oddClr  The odds to use when clearing space.
   * @param beamWidth The width of the sonar reponse in radians.
   * @param res The accuracy of the sonar in meters.
   */
  SimpleSonarModel(SensorSonar *sonar, const double& dTh, const double& dd,
      const double& oddObs, const double& oddClr, const double& beamWidth,
      const double& res);

  /**
   * Uses the model to generate point data for updating the math. Note that the
   * map handles pruning overlapping grid updates.
   *
   * @param map The map instance to update.
   * @param robot The pose of the robot corresponding to the current sonar
   * readings available from the sonar object.
   */
  int updateMap(Map *map, const Pose& robot);

private:
  /**
   * The piece-wise function for determining obstacle/clearing response based
   * on the distance of the sonar reading.
   *
   * @param x The current distance from the sensor.
   * @param d The distance reading from the sensor.
   * @return
   */
  double profile(const double& x, const double& d);

  /**
   * Handles the response based on the angle of a pixel from the center of the
   * beam.
   * @param th The angle from the center of the beam, in radians.
   * @param prof The profile value returned from the piece-wise profile.
   * @return
   */
  virtual double falloff(const double& th, const double& prof);

  SensorSonar *_sonar;
  double _dTh, _dd, _obs, _clr, _w, _w_2, _res_2;
};

#endif /* sonarmodel_h_ */