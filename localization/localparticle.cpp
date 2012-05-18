/*
 * localparticle.cpp
 *
 *  Created on: May 17, 2012
 *      Author: derrick
 */

#include "localization/localparticle.h"
#include "core/common.h"

#include <cstdlib>

ParticleLocalization::ParticleLocalization(SensorModel *sensor, Map *map,
    size_t numParticles, const double& discardThresh,
    const double& clearSpaceOdds) {
  num = numParticles;
  this->sensor = sensor;
  this->map = map;
  thresh = discardThresh;
  clearOdds = clearSpaceOdds;

  for (size_t i = 0; i < num; ++i) {
    spawnRandomParticle();
  }
}

void ParticleLocalization::motionUpdate(const Pose& dPose) {
  for (PoseVList::iterator i = particles.begin(); i != particles.end(); ++i) {
    i->pose = i->pose + dPose;
    while (i->pose.yaw < -PI_2) i->pose.yaw += PI2;
    while (i->pose.yaw > PI_2) i->pose.yaw -= PI2;
  }
}

void ParticleLocalization::sensorUpdate() {
  double p;
  for (PoseVList::iterator i = particles.begin(); i != particles.end(); ++i) {
    p = PtoO(sensor->localizationProb(*map, i->pose));
    i->v *= p;
    //std::cout << p << " -> " << i->v << std::endl;
  }

  double sum = 0;
  //prune dead particles
  for (PoseVList::iterator i = particles.begin(); i != particles.end(); ++i) {
    if (i->v < thresh) {
      i = particles.erase(i) - 1;
    } else {
      sum += i->v;
    }
  }

  //Number of particles we have to now spawn
  if (particles.size() < num * 0.1) {
    //Spawn all new particles randomly due to lack of population
    std::cout << "Spawning all random particles." << std::endl;
    for (size_t i = 0; i < num; ++i) {
      spawnRandomParticle();
    }
  } else {
    size_t n = num - particles.size();
    std::cout << "Spawning " << n << " child particles." << std::endl;
    for (size_t i = 0; i < n; ++i) {
      spawnChildParticle(sum);
    }
  }
}

PoseV ParticleLocalization::getPose() {
  double sum = getOddsSum();
  PoseV p;
  p.pose.p.x = 0;
  p.pose.p.y = 0;
  p.pose.yaw = 0;

  //def weighted_incremental_variance(dataWeightPairs):
  //sumweight = 0
  double sumweight = 0;
  //mean = 0
  double meanx = 0, meany = 0, meanyaw = 0;
  //M2 = 0
  double m2x = 0, m2y = 0, m2yaw;

  double temp, deltax, deltay, deltayaw, rx, ry, ryaw, weight, f;

  //for x, weight in dataWeightPairs:  # Alternately "for x, weight in zip(data, weights):"
  for (PoseVList::iterator i = particles.begin(); i != particles.end(); ++i) {
    //temp = weight + sumweight
    weight = OtoP(i->v);
    temp = weight + sumweight;

    //delta = x - mean
    deltax = i->pose.p.x - meanx;
    deltay = i->pose.p.y - meany;
    deltayaw = i->pose.yaw - meanyaw;

    //R = delta * weight / temp
    f = weight / temp;
    rx = deltax * f;
    ry = deltay * f;
    ryaw = deltayaw * f;

    //mean = mean + R
    meanx += rx;
    meany += ry;
    meanyaw += ryaw;

    //M2 = M2 + sumweight * delta * R  # Alternatively, "M2 = M2 + weight * delta * (x-mean)"
    m2x += sumweight * deltax * rx;
    m2y += sumweight * deltay * ry;
    m2yaw += sumweight * deltayaw * ryaw;

    //sumweight = temp
    sumweight = temp;
  }

  //variance_n = M2/sumweight
  double vnx = m2x / sumweight;
  double vny = m2y / sumweight;
  double vnyaw = m2yaw / sumweight;

  //variance = variance_n * len(dataWeightPairs)/(len(dataWeightPairs) - 1)
  f = particles.size() / (particles.size() - 1.0);
  double vx = vnx * f;
  double vy = vny * f;
  double vyaw = vnyaw * f;

  p.pose.p.x = meanx;
  p.pose.p.y = meany;
  p.pose.yaw = meanyaw;

  p.v = vx + vy + vyaw;

  return p;
}

void ParticleLocalization::spawnRandomParticle() {
  double w = map->envWidth();
  double h = map->envHeight();
  double l = map->envLeft();
  double b = map->envBottom();

  PoseV p;
  do {
    p.pose.p.x = frand(l, w);
    p.pose.p.y = frand(b, h);

  } while (map->get(p.pose.p.x, p.pose.p.y) > clearOdds);

  p.pose.yaw = frand(-PI_2, PI2);
  p.v = 1;

  particles.push_back(p);
}

double ParticleLocalization::getOddsSum() {
  double sum = 0.0;
  for (PoseVList::iterator i = particles.begin(); i != particles.end(); ++i) {
    sum += i->v;
  }
  return sum;
}

double ParticleLocalization::spawnChildParticle() {
  double sum = getOddsSum();

  spawnChildParticle(sum);

  return sum;
}

void ParticleLocalization::spawnChildParticle(double sum) {
  double r = frand(0.0, sum);
  PoseV child;

  sum = 0;
  for (PoseVList::iterator i = particles.begin(); i != particles.end(); ++i) {
    sum += i->v;
    if (r < sum) {
      child = (*i);
      break;
    }
  }

  //Add some random "jitter" to the child
  child.pose.p.x += frand(-0.1, 0.2);
  child.pose.p.y += frand(-0.1, 0.2);
  child.pose.yaw += frand(-0.01, 0.02);

  particles.push_back(child);
}