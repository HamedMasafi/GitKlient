#include "appmainwindow.h"
#include "qdebug.h"

#include <QEventLoop>
#include <QCloseEvent>

AppMainWindow::AppMainWindow(QWidget *parent, Qt::WindowFlags f) : KXmlGuiWindow(parent, f) {}

int AppMainWindow::exec()
{
    QEventLoop eventLoop;
    _loop = &eventLoop;
    showModal();
    (void) eventLoop.exec(QEventLoop::DialogExec);
    _loop = nullptr;
    qDebug() << "returnCode=" << _returnCode;
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

void AppMainWindow::setVisible(bool visible)
{
    if (!visible && _loop)
        _loop->exit();
    KXmlGuiWindow::setVisible(visible);
}

//void AppMainWindow::closeDialog(int resultCode)
//{
//    _returnCode = resultCode;
//    if (_loop && _loop->isRunning())
//        _loop->quit();
//}

//void AppMainWindow::closeEvent(QCloseEvent *event)
//{
//    qDebug() << Q_FUNC_INFO;
//    Q_UNUSED(event)
//    if (_loop && _loop->isRunning())
//        _loop->quit();
//    KXmlGuiWindow::closeEvent(event);
//}

void AppMainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && _isModal)
        close();

    KXmlGuiWindow::keyPressEvent(event);
}
