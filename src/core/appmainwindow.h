#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>

class QEventLoop;
class AppMainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    enum DialogCode { Rejected, Accepted };

    explicit AppMainWindow(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    int exec();
    void showModal();
    void accept();
    void reject();
    void setVisible(bool visible) override;

protected:
//    void closeDialog(int resultCode);
//    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QEventLoop *_loop{nullptr};
    bool _isModal{false};
    int _returnCode{0};

};

#endif // MAINWINDOW_H
