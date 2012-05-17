/*
 * mapconvert.cpp
 *
 *  Created on: May 16, 2012
 *      Author: derrick
 */

#include "mapping/dynamicmap.h"
#include <iostream>

void usage(char *ex) {
  std::cerr << "Usage: " << ex
      << " <Map Resolution> <Left> <Top> <Jpeg File> <Map File>" << std::endl
      << std::endl;

  std::cerr << "This program converts a jpeg image into a map file using the "
      << "specified\nparameters." << std::endl << std::endl;

  std::cerr
      << "Map Resolution - The number of meters that a single pixel represents."
      << std::endl;

  std::cerr
      << "Left - The x-coordinate that corresponds to the left edge of the jpeg"
      << std::endl;

  std::cerr
      << "Top - The y-coordinate that corresponds to the top edge of the jpeg"
      << std::endl;

  std::cerr << "Jpeg File - The jpeg file to read in" << std::endl;

  std::cerr << "Map File - The filename to save the map data to" << std::endl;
  std::cerr << std::endl;
}

int main(int argc, char **argv) {

  if (argc != 6) {
    usage(argv[0]);
    exit(1);
  }

  double res = atof(argv[1]);
  double left = atof(argv[2]);
  double top = atof(argv[3]);

  std::string jpegfile(argv[4]);
  std::string mapfile(argv[5]);

  DynamicMap map(res, 5, 5, 1.0);

  /*if (map.loadMapJpeg(jpegfile, res, left, top)) {
    std::cerr << "Error loading data from jpeg image." << std::endl;
    exit(2);
  }

  if (map.saveMap(mapfile)) {
    std::cerr << "Error saving map data to file." << std::endl;
    exit(3);
  }*/

  return 0;
}

