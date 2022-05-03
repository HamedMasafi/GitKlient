#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>

class QEventLoop;
class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    enum DialogCode { Rejected, Accepted };

    MainWindow(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    int exec();
    void showModal();
    void accept();
    void reject();

protected:
    void closeDialog(int resultCode);
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QEventLoop *_loop{nullptr};
    bool _isModal{false};
    int _returnCode{0};
};

#endif // MAINWINDOW_H
