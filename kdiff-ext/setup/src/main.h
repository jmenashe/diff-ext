#ifndef __main_h__
#define __main_h__

#include <kapplication.h>

class KDIFF_EXT_SETUP : public KApplication {
  Q_OBJECT
  public slots:
    void about_clicked();
};

#endif // __main_h__
