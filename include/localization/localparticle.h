#ifndef localparticle_h_
#define localparticle_h_

#include "localization/localization.h"
#include "sensor/sensormodel.h"

class ParticleLocalization: public Localization {
public:

  /**
   *
   * @param sensor The sensor model used to perform the sensor update step.
   * @param map The map used for the sensor update.
   * @param numParticles The number of particles to use.
   * @param discardThresh The probability threshhold that particles are
   * discarded once they drop below.
   * @param clearSpaceOdds The maximum odds value allowed for the localization
   * to spawn a random pose at a given location.
   */
  ParticleLocalization(SensorModel *sensor, Map *map, size_t numParticles,
      const double& discardThresh, const double& clearSpaceOdds,
      const double& randomChance, const double& posJitter,
      const double& yawJitter);

  virtual void motionUpdate(const Pose& dPose);
  virtual void sensorUpdate();

  virtual PoseV getPose();

  PoseVList* getParticles() {
    return &particles;
  }

private:

  /**
   * Handles spawning a new randomly posed particle in the map.
   */
  void spawnRandomParticle();

  /**
   * Spawns a new particle based on the most probable particles currently
   * available.
   *
   * @return The sum of the particle odds in this generation so it can be reused
   * to generate subsequent children without having to iterate over the whole
   * population again to sum.
   */
  double spawnChildParticle();

  /**
   *
   * @param sum The sum of the odds of the particles.
   */
  void spawnChildParticle(double sum);

  double getOddsSum();

  PoseVList particles;
  size_t num;

  SensorModel *sensor;
  Map *map;

  /**
   * The threshold for discarding unlikely particles, and for ensuring a
   * particle starts in clear space.
   */
  double thresh, clearOdds, randC;

  double pj, yj;
};

#endif /* localparticle_h_ */
