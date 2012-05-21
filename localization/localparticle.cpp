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
    const double& clearSpaceOdds, const double& chanceRandom,
    const double& posJitter, const double& yawJitter) {
  num = numParticles;
  this->sensor = sensor;
  this->map = map;
  thresh = discardThresh;
  clearOdds = clearSpaceOdds;
  randC = chanceRandom;

  pj = posJitter;
  yj = yawJitter;

  for (size_t i = 0; i < num; ++i) {
    spawnRandomParticle();
  }
}

void ParticleLocalization::motionUpdate(const Pose& dPose) {
  for (PoseVList::iterator i = particles.begin(); i != particles.end(); ++i) {
    i->pose = i->pose + dPose;

    /*if (map->get(i->pose.p.x, i->pose.p.y) > 4.0) {
      i->v = 0;
    }*/

    while (i->pose.yaw < -PI_2) i->pose.yaw += PI2;
    while (i->pose.yaw > PI_2) i->pose.yaw -= PI2;
  }
}

void ParticleLocalization::sensorUpdate() {
  double p;
  for (PoseVList::iterator i = particles.begin(); i != particles.end(); ++i) {
    if (map->get(i->pose.p.x, i->pose.p.y) > 2.0) {
      i->v = 0;
    } else {
      i->v = sensor->localizationProb(*map, i->pose);
    }
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
  size_t n = num - particles.size();
  size_t c = 0, r = 0;

  /*if (particles.size() < num * 0.05) {
    //Spawn all new particles randomly due to lack of population
    std::cout << "Spawning " << n << " random particles." << std::endl;
    for (size_t i = 0; i < n; ++i) {
      spawnRandomParticle();
    }
  } else {*/
    for (size_t i = 0; i < n; ++i) {
      if (frand(0.0, 1.0) < randC) {
        spawnRandomParticle();
        ++r;
      } else {
        spawnChildParticle(sum);
        ++c;
      }
    }

    //std::cout << "Spawned " << c << " children and " << r << " randomly." << std::endl;
  //}
}

PoseV ParticleLocalization::getPose() {
  PoseV sum, avg;
  double d, n, v, w;

  sum.pose.p.x = 0;
  sum.pose.p.y = 0;
  sum.pose.yaw = 0;
  sum.v = 0;

  avg.pose.p.x = 0;
  avg.pose.p.y = 0;
  avg.pose.yaw = 0;
  avg.v = 0;

  n = particles.size();

  for (PoseVList::iterator i = particles.begin(); i != particles.end(); ++i) {
    sum.pose.p.x += i->pose.p.x * i->v;
    sum.pose.p.y += i->pose.p.y * i->v;
    sum.pose.yaw += i->pose.yaw * i->v;
    w += i->v;
  }

  //v = n * w;
  avg.pose.p.x = sum.pose.p.x / w;
  avg.pose.p.y = sum.pose.p.y / w;
  avg.pose.yaw = sum.pose.yaw / w;

  //Now find the weighted sq error for each particle
  for (PoseVList::iterator i = particles.begin(); i != particles.end(); ++i) {
    double dx = i->pose.p.x - avg.pose.p.x;
    double dy = i->pose.p.y - avg.pose.p.y;
    avg.v += hypot(dx, dy) * i->v;
  }
  avg.v /= w;

  return avg;
}

/*PoseV ParticleLocalization::getPose() {
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
    double o = i->v;
    weight = OtoP(o);
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
}*/

void ParticleLocalization::spawnRandomParticle() {
  double w = map->envWidth();
  double h = map->envHeight();
  double l = map->envLeft();
  double b = map->envBottom();

  double d;
  PoseV p;
  do {
    p.pose.p.x = frand(l, w);
    p.pose.p.y = frand(b, h);

    d = hypot(p.pose.p.x, p.pose.p.y);

  } while (map->get(p.pose.p.x, p.pose.p.y) > clearOdds
      || d < 1.0);

  p.pose.yaw = frand(-PI, PI2);
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

  double d = hypot(child.pose.p.x, child.pose.p.y);

  if (d < 1.0) {
    spawnRandomParticle();
    return;
  }

  //Add some random "jitter" to the child
  child.pose.p.x += frand(-pj, 2*pj);
  child.pose.p.y += frand(-pj, 2*pj);
  child.pose.yaw += frand(-yj, 2*yj);

  particles.push_back(child);
}
