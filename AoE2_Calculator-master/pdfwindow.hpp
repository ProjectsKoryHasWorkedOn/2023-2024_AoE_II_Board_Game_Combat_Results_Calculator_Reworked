#ifndef PDFWINDOW_HPP
#define PDFWINDOW_HPP

#include "ui_pdfwindow.h"

QT_BEGIN_NAMESPACE
class QPdfView;
QT_END_NAMESPACE

class PdfWindow : public QDialog {
  Q_OBJECT

public:
  explicit PdfWindow(const QString& docLocation, QWidget* parent = nullptr);

private slots:
  void pageSelected(int page);

private:
  Ui::PdfWindow ui;
  QPdfDocument* m_document;
};

#endif // PDFWINDOW_HPP
