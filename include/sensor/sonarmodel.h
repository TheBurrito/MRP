#ifndef sonarmodel_h_
#define sonarmodel_h_

#include "sensor/sensor-sonar.h"
#include "sensor/sensormodel.h"

class SonarProfile: public SensorOddsProfile {
public:
  /**
   * Stores the various parameters for performing odds calculations from a sonar
   * sensor.
   *
   * @param maxD The max reading of the sonar sensor.
   * @param oddObs The odds used when calculating odds for a perceived obstacle.
   * @param oddClr The odds used when calculating odds for clear space.
   * @param beamWidth The width of the beam, in radians.
   * @param res The resolution of the sensor, in meters.
   */
  SonarProfile(const double& maxD, const double& oddObs, const double& oddClr,
      const double& beamWidth, const double& res);

  virtual double getOdds(const PosPol& pt, const double& d);

private:
  double _obs, _clr;
  double _w, _w_2;
  double _res, _res_2;
  double _d;
};

class SonarIterativeRegion: public SensorRegion {
public:
  SonarIterativeRegion(const double& dTh, const double& dd,
      const double& beamWidth, const double& res);

  virtual void buildRegion(SensorRanger *sensor, const Pose& robot, size_t idx,
        const Pose& geom);

private:
  double _dTh, _dd;
  double _w, _w_2;
  double _res, _res_2;
};

#endif /* sonarmodel_h_ */
