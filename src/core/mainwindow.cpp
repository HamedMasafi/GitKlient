#include "mainwindow.h"

#include <QEventLoop>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags f) : KXmlGuiWindow(parent, f) {}

int MainWindow::exec()
{
    _loop = new QEventLoop(this);
    showModal();
    _loop->exec();
    return _returnCode;
}

void MainWindow::showModal()
{
    _isModal = true;
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose, true);
    show();
}

void MainWindow::closeDialog(int resultCode)
{
    _returnCode = resultCode;
    if (_loop)
        _loop->quit();
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
