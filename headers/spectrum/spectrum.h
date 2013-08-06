// Includes all the files required by Spectrum, so include this.

#ifndef SPECTRUM_IN_H
#define SPECTRUM_IN_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
// Potential incompatibility with Linux systems
// May need to rewrite mxlmap.cpp to use alternate methods of finding file size
#include <io.h>
// End incompatibility
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

using namespace std;

// Engine includes
#include "object.h"
#include "exception.h"
#include "engine/assets.h"
#include "engine/rect.h"
#include "engine/sprite.h"
#include "engine/tileset.h"
#include "engine/mxlmap.h"
#include "engine/gameobject.h"
#include "engine/room.h"

#include "engine/engine.h"

#endif 