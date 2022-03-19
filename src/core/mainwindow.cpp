#include "mainwindow.h"

#include <QEventLoop>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags f) : KXmlGuiWindow(parent, f) {}

void MainWindow::exec()
{
    _loop = new QEventLoop(this);
    showModal();
    _loop->exec();
}

void MainWindow::showModal()
{
    _isModal = true;
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose, true);
    show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    if (_loop)
        _loop->quit();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && _isModal)
        close();

    KXmlGuiWindow::keyPressEvent(event);
}
