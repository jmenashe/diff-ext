#ifndef __model_h__
#define __model_h__

#include <glib.h>

typedef struct {
  GString* diff_tool;
  GString* diff3_tool;
  gboolean enable_diff3;
} MODEL;

#endif /*__model_h__*/
