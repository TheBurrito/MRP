#ifndef types_h_
#define types_h_

#include <vector>

typedef struct {
	double x;
	double y;
} Pos2;

typedef struct {
	double theta;
	double d;
} Polar2;

typedef std::vector<Polar2> Polar2List;

typedef struct {
	Pos2 p;
	double yaw;
} Pose;

typedef std::vector<Pos2> Pos2List;

typedef struct {
	Pos2 p;
	Polar2 pol;
} PosPol;

typedef std::vector<PosPol> PosPolList;

typedef struct {
	Pos2 p;
	double v;
} Pos2V;

typedef std::vector<Pos2V> Pos2VList;

inline bool operator<(const Pos2& a, const Pos2& b) {
  if (a.x != b.x) {
    return a.x < b.x;
  } else {
    return a.y < b.y;
  }
}

#endif /* types_h_ */
