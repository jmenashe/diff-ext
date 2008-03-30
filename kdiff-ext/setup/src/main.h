#include <qwidgetlist.h>
#include <kapplication.h>
#include <kaboutapplication.h>

class KDIFF_EXT_SETUP : public KApplication {
  Q_OBJECT
  public slots:
    void about_clicked() {
      KAboutApplication* about = new KAboutApplication(QApplication::topLevelWidgets()->at(0));
      about->show();
    }
};
