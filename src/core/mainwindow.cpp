#include "mainwindow.h"
#include "qdebug.h"

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

void MainWindow::accept()
{
    _returnCode = Accepted;
}

void MainWindow::reject()
{
    _returnCode = Rejected;
}

void MainWindow::closeDialog(int resultCode)
{
    _returnCode = resultCode;
    if (_loop)
        _loop->quit();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(event)
    if (_loop)
        _loop->quit();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && _isModal)
        close();

    KXmlGuiWindow::keyPressEvent(event);
}
