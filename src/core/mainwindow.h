#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>

class QEventLoop;
class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
    QEventLoop *_loop{nullptr};
    bool _isModal{false};

public:
    MainWindow(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    void exec();
    void showModal();

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

};

#endif // MAINWINDOW_H
