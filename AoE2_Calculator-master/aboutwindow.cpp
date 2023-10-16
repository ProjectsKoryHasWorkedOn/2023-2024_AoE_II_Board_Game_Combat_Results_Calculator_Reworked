#include "aboutwindow.h"
#include "cross-window_palette.h"
#include "ui_aboutwindow.h"

aboutwindow::aboutwindow(QWidget* parent)
  : QDialog(parent), ui(new Ui::aboutwindow)
{
  ui->setupUi(this);
  ui->program_title_label->setText("<b>AoE II board game calculator</b>");
  ui->program_title_label->setPalette(selectedPalette);
  ui->version_label->setPalette(selectedPalette);
  ui->version_label->setText("<b>Version:</b> 2.0");
  ui->license_label->setText("<b>License:</b> Careware");
  ui->license_label->setPalette(selectedPalette);
  ui->license_description_label->setPalette(selectedPalette);
  ui->license_description_label->setTextInteractionFlags(
    Qt::TextBrowserInteraction);
  ui->license_description_label->setOpenExternalLinks(true);
  ui->webpage_label->setPalette(selectedPalette);
  ui->webpage_label->setText("<b>GitHub page:</b>");
  ui->webpage_link_label->setTextFormat(Qt::RichText);
  ui->license_description_label->setText(
    "If you found the program useful and would like to support its "
    "development, you may make a donation to a non-profit charitable "
    "organization like the <a href=\"https://www.australianwildlife.org/\" "
    "style="
    + palettes.getLinkColor() + ">Australian Wildlife Conservancy (AWC)</a>");
  ui->webpage_link_label->setText("<a href=\"https://github.com/ProjectsKoryHasWorkedOn/AoE_II_Board_Game_Combat_Results_Calculator_Reworked\" style=" + palettes.getLinkColor() + ">https://github.com/ProjectsKoryHasWorkedOn/AoE_II_Board_Game_Combat_Results_Calculator_Reworked</a>");
  ui->webpage_link_label->setTextInteractionFlags(Qt::TextBrowserInteraction);
  ui->webpage_link_label->setOpenExternalLinks(true);
  this->setPalette(selectedPalette);
}

aboutwindow::~aboutwindow()
{
  delete ui;
}

void aboutwindow::on_okayButton_clicked()
{
  hide();
}
