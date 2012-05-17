/*
 * mapconvert.cpp
 *
 *  Created on: May 16, 2012
 *      Author: derrick
 */

#include "mapping/dynamicmap.h"
#include <iostream>
#include <fstream>

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

  std::ofstream out(mapfile.c_str(), std::ofstream::out);

  out << jpegfile << std::endl;
  out << res << std::endl;
  out << left << std::endl;
  out << top << std::endl;

  out.close();

  return 0;
}

