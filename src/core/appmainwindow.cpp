#include "appmainwindow.h"
#include "qdebug.h"

#include <QEventLoop>
#include <QCloseEvent>

AppMainWindow::AppMainWindow(QWidget *parent, Qt::WindowFlags f) : KXmlGuiWindow(parent, f) {}

int AppMainWindow::exec()
{
    _loop = new QEventLoop(this);
    showModal();
    _loop->exec();
    return _returnCode;
}

void AppMainWindow::showModal()
{
    _isModal = true;
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose, true);
    show();
}

void AppMainWindow::accept()
{
    _returnCode = Accepted;
}

void AppMainWindow::reject()
{
    _returnCode = Rejected;
}

void AppMainWindow::closeDialog(int resultCode)
{
    _returnCode = resultCode;
    if (_loop)
        _loop->quit();
}

void AppMainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(event)
    if (_loop)
        _loop->quit();
}

void AppMainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && _isModal)
        close();

    KXmlGuiWindow::keyPressEvent(event);
}
