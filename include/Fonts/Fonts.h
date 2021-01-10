#ifndef _FONTS_H
#define _FONTS_H

#include <Arduino.h>

// Filesystem fonts
// const String FilesystemFonts[] = { "ArialBold38" };
// #define FILESYSTEM_FONTS_MAX 1
// #define FSF_ARIAL_BOLD_38 FilesystemFonts[0]

// Flash fonts
#include "Fonts/DSEG7ClassicBold_44.h"
#include "Fonts/ArialBold14.h"
#include "Fonts/ArialBold28.h"
#include "Fonts/ArialBold38.h"

#define DSEG7_44      &DSEG7ClassicBold_44
#define ARIAL_BOLD_14 ArialBold14
#define ARIAL_BOLD_28 ArialBold28
#define ARIAL_BOLD_38 ArialBold38



#endif
