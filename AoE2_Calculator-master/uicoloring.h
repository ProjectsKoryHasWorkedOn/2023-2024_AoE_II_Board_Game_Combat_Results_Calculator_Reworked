#ifndef UICOLORING_H
#define UICOLORING_H
#include <QPalette>
#include <QString>


class colorPalettes {
public:
  QPalette darkPalette;
  QPalette lightPalette;

  void setPaletteValues();

  bool darkModeEnabled;

  QString getButtonBackgroundColor();

  QString getMenuBarStyling();

  QString getLinkColor();

  QString getSpinBoxStyling();

  QString getDialogBoxStyling();


};

#endif // UICOLORING_H
