/*
 * map.cpp
 *
 *  Created on: May 16, 2012
 *      Author: derrick
 */

#include "mapping/map.h"

#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cstdarg>

#include <fstream>

#include "png.h"

void abort_(const char * s, ...) {
  va_list args;
  va_start(args, s);
  vfprintf(stderr, s, args);
  fprintf(stderr, "\n");
  va_end(args);
  exit(1);
}

int Map::saveMap(std::string filename) {
  std::string pngfile = filename + ".png";

  std::ofstream out(filename.c_str(), std::ofstream::out);

  out << pngfile << std::endl;
  out << gridRes() << std::endl;
  out << envLeft() << std::endl;
  out << envTop() << std::endl;

  return saveMapPng(pngfile);
}

int Map::loadMapPng(std::string filename, const double& left,
    const double& top) {

  double x, y;

  size_t width, height;

  png_structp png_ptr;
  png_infop info_ptr;
  int number_of_passes;
  png_bytep * row_pointers;

  unsigned char header[8]; // 8 is the maximum size that can be checked

  /* open file and test for it being a png */
  FILE *fp = fopen(filename.c_str(), "rb");
  if (!fp)
    abort_("[read_png_file] File %s could not be opened for reading",
        filename.c_str());
  fread(header, 1, 8, fp);
  if (png_sig_cmp(header, 0, 8))
    abort_("[read_png_file] File %s is not recognized as a PNG file",
        filename.c_str());

  /* initialize stuff */
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
    abort_("[read_png_file] png_create_read_struct failed");

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    abort_("[read_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[read_png_file] Error during init_io");

  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);

  png_read_info(png_ptr, info_ptr);

  width = png_get_image_width(png_ptr, info_ptr);
  height = png_get_image_height(png_ptr, info_ptr);

  number_of_passes = png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);

  /* read file */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[read_png_file] Error during read_image");

  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
  for (size_t yi = 0; yi < height; yi++)
    row_pointers[yi] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));

  png_read_image(png_ptr, row_pointers);

  fclose(fp);

  double o;
  int c = png_get_channels(png_ptr, info_ptr);
  int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
  std::cout << "channels: " << c << std::endl;
  std::cout << "rowbytes: " << rowbytes << std::endl;
  size_t n = 0;
  unsigned char v;

  for (size_t yi = 0; yi < height; ++yi) {
    y = top - (yi * gridRes() + gridRes() / 2.0);

    for (size_t xi = 0; xi < rowbytes; xi += c) {
      x = left + xi * gridRes() + gridRes() / 2.0;
      v = row_pointers[yi][xi];
      if (v == 0) {
        o = MAX_ODDS;
      } else {
        o = PtoO(1.0 - (v / 255.0));
      }

      //std::cout << "(" << x << ", " << y << ") = " << o << " : " << v << std::endl;
      set(x, y, o);
    }
  }

  /* cleanup heap allocation */
  for (size_t yi = 0; yi < height; ++yi)
    free(row_pointers[yi]);
  free(row_pointers);

  return 0;
}

int Map::saveMapPng(std::string filename) {
  double x, y;

  size_t width = envWidth() / gridRes(), height = envHeight() / gridRes();

  png_byte color_type;
  png_byte bit_depth;

  png_structp png_ptr;
  png_infop info_ptr;
  int number_of_passes;
  png_bytep * row_pointers;

  FILE *fp = fopen(filename.c_str(), "wb");
  if (!fp)
    abort_("[write_png_file] File %s could not be opened for writing",
        filename.c_str());

  /* initialize stuff */
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
    abort_("[write_png_file] png_create_write_struct failed");

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    abort_("[write_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during init_io");

  png_init_io(png_ptr, fp);

  /* write header */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during writing header");

  png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_GRAY,
      PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);

  for (size_t yi = 0; yi < height; ++yi) {
    row_pointers[yi] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));
    y = envTop() - (yi * gridRes() + gridRes() / 2.0);

    for (size_t xi = 0; xi < width; ++xi) {
      x = envLeft() + xi * gridRes() + gridRes() / 2.0;
      row_pointers[yi][xi] = (1 - OtoP(get(x, y))) * 255;
    }
  }

  /* write bytes */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during writing bytes");

  png_write_image(png_ptr, row_pointers);

  /* end write */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during end of write");

  png_write_end(png_ptr, NULL);

  /* cleanup heap allocation */
  for (size_t yi = 0; yi < height; ++yi)
    free(row_pointers[yi]);
  free(row_pointers);

  fclose(fp);

  return 0;
}
