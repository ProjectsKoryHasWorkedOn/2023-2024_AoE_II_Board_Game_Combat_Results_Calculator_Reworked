#include <uicoloring.h>
#include <QPalette>
#include <QColor>
#include <QString>

// Set how we want elements to be colored

// @Phillip Does not seem to work for buttons (QPalette::Button) or the menu bar



void colorPalettes::setPaletteValues(){
  /* Light palette */
  // Set background color of the window
  lightPalette.setColor(QPalette::Window, QColor(240, 240, 240));

         // Set the background color of the tooltip
  lightPalette.setColor(QPalette::ToolTipBase, QColor(249, 249, 249));

         // Set the background color of the UI elements
  lightPalette.setColor(QPalette::Base, QColor(255, 255, 255));

         // Set the color of the text
  lightPalette.setColor(QPalette::WindowText, Qt::black);
  lightPalette.setColor(QPalette::ToolTipText, Qt::black);
  lightPalette.setColor(QPalette::ButtonText, Qt::black);
  lightPalette.setColor(QPalette::Text, Qt::black);

  // Set the color of highlighted text
  lightPalette.setColor(QPalette::Highlight, QColor(229, 243, 255));


  /* Dark palette */
  // Set background color of the window
  darkPalette.setColor(QPalette::Window, QColor(30, 30, 30));

  // Set the background color of the tooltip
  darkPalette.setColor(QPalette::ToolTipBase, QColor(60, 60, 60));

  // Set the background color of the UI elements
  darkPalette.setColor(QPalette::Base, QColor(45, 45, 45));

  // Set the color of the text
  darkPalette.setColor(QPalette::WindowText, Qt::white);
  darkPalette.setColor(QPalette::ToolTipText, Qt::white);
  darkPalette.setColor(QPalette::ButtonText, Qt::white);
  darkPalette.setColor(QPalette::Text, Qt::white);

  // Set the color of highlighted text
  darkPalette.setColor(QPalette::Highlight, QColor(229, 243, 255));
}


QString colorPalettes::getButtonBackgroundColor(){
  QString buttonBackgroundColor;

  if(colorPalettes::darkModeEnabled == true){
    buttonBackgroundColor = "background-color: rgb(130, 130, 130);";

  }
  else{
    buttonBackgroundColor = "background-color: rgb(253, 253, 253);";
  }


  return buttonBackgroundColor;
}


QString colorPalettes::getSpinBoxStyling(){
  QString spinBoxStyling;
  QString spinBoxBackgroundColor;
  QString spinBoxTextColor;

  if(colorPalettes::darkModeEnabled == true){
    spinBoxBackgroundColor = "background-color: rgb(45, 45, 45);";
    spinBoxTextColor = "color: white;";
  }
  else{
    spinBoxBackgroundColor = "background-color: rgb(253, 253, 253);";
    spinBoxTextColor = "color: black;";
  }

  spinBoxStyling = spinBoxBackgroundColor + spinBoxTextColor;

  return spinBoxStyling;
}


QString colorPalettes::getLineEditStyling(){
  QString lineEditStyling;
  QString lineEditBackgroundColor;
  QString lineEditTextColor;

  if(colorPalettes::darkModeEnabled == true){
    lineEditBackgroundColor = "QLineEdit {color: white; background-color: rgb(45, 45, 45);};";
  }
  else{
    lineEditBackgroundColor = "QLineEdit {color: black; background-color: rgb(253, 253, 253);};";
  }

  lineEditStyling = lineEditBackgroundColor + lineEditTextColor;

  return lineEditStyling;
}


// QString colorPalettes::get


QString colorPalettes::getDialogBoxStyling(){
  QString dialogBoxStyling;

  QString dialogBoxColor = "";
  QString lineEditStyling = "";

  QString widgetTextColor = "";

  lineEditStyling = getLineEditStyling();

  if(colorPalettes::darkModeEnabled == true){
    dialogBoxColor = "QInputDialog {background-color: black;}";
    widgetTextColor = "QLabel {color: rgb(255, 255, 255);};";
  }
  else{
    dialogBoxColor = "QInputDialog {background-color: rgb(253, 253, 253);}";
    widgetTextColor = "QLabel {color: rgb(0,0,0);};";
  }

  dialogBoxStyling = dialogBoxColor + lineEditStyling + widgetTextColor;

  return dialogBoxStyling;
}


QString colorPalettes::getMenuBarStyling(){
  QString menuBarStyling;
  QString menuBarBackgroundColor;
  QString menuBarItemBackgroundColor;
  QString menuBarItemToggleUntickedColor;
  QString menuBarItemToggleTickedColor;

  if(colorPalettes::darkModeEnabled == true){
    menuBarBackgroundColor = "QMenuBar {background-color: rgb(30, 30, 30);}";
    menuBarItemBackgroundColor = "QMenuBar::item {background-color: rgb(30, 30, 30);}";
    menuBarItemToggleTickedColor = "QMenuBar::item:checked {color: rgb(255, 255, 255);}";
    menuBarItemToggleUntickedColor = "QMenuBar::item:unchecked {color: rgb(255, 255, 255);}";
  }
  else{
    menuBarBackgroundColor = "QMenuBar {background-color: rgb(253, 253, 253);}";
    menuBarItemBackgroundColor = "QMenuBar::item {background-color: rgb(245, 245, 245);}";
        menuBarItemToggleTickedColor = "QMenuBar::item:checked {color: rgb(0, 0, 0);}";
    menuBarItemToggleUntickedColor = "QMenuBar::item:unchecked {color: rgb(0, 0, 0);}";
  }

  menuBarStyling = menuBarBackgroundColor + menuBarItemBackgroundColor + menuBarItemToggleTickedColor + menuBarItemToggleUntickedColor;

  return menuBarStyling;
}





QString colorPalettes::getLinkColor(){
  QString getLinkColor;

  if(colorPalettes::darkModeEnabled == true){
    getLinkColor = "\"color: rgb(100, 149, 237);\"";
  }
  else{

  }


  return getLinkColor;
}

