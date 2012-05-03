#ifndef localparticle_h_
#define localparticle_h_

#include "core/types.h"
#include "localization/localization.h"

typedef struct {
  Pose pos;
  double p;
} PoseP;

typedef std::vector<PoseP> PosePList;

class ParticleLocalization : public Localization {
public:
  ParticleLocalization(size_t nParticles);
  
  Pose getError(const Pose& robot, const Pose& dPose);
  
  void setNumParticles(size_t n);
  size_t getNumParticles();
  
private:
  PosePList _particles;
  size_t _nParticles;
};

#endif /* localparticle_h_ */
