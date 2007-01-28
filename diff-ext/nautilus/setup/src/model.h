/*
 * Copyright (c) 2007, Sergey Zorin. All rights reserved.
 *
 * This software is distributable under the BSD license. See the terms
 * of the BSD license in the COPYING file provided with this software.
 *
 */
#ifndef __model_h__
#define __model_h__

#include <glib.h>

typedef struct {
  GString* diff_tool;
  GString* diff3_tool;
  gboolean enable_diff3;
} MODEL;

#endif /*__model_h__*/
