/*
 * viswindow.h
 *
 *  Created on: May 18, 2012
 *      Author: derrick
 */

#ifndef VISWINDOW_H_
#define VISWINDOW_H_

#include "visualization/vistypes.h"

VisImage * initVis(size_t width, size_t height, bool idle, bool *wait);

void redraw(bool force);

void killVis();

#endif /* VISWINDOW_H_ */
