#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QPdfDocument>
#include <QPdfPageNavigator>

#include "pdfwindow.hpp"

PdfWindow::PdfWindow(const QString& docLocation, QWidget* parent)
  : QDialog{parent}, m_document{new QPdfDocument{this}}
{
  ui.setupUi(this);
  ui.pdfView->setDocument(m_document);

  if (QFile::exists(docLocation)) {
    m_document->load(docLocation);
    pageSelected(0);
    setWindowTitle(QFileInfo{docLocation}.fileName());
  }
  else {
    const QString message = tr("%1 is not a valid local file").arg(docLocation);
    QMessageBox::critical(this, tr("Failed to open"), message);
  }
}

void PdfWindow::pageSelected(int page)
{
  ui.pdfView->pageNavigator()->jump(page, QPointF{});
}
