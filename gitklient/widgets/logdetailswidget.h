#ifndef LOGDETAILSWIDGET_H
#define LOGDETAILSWIDGET_H

#include <QTextBrowser>

namespace Git {
class Log;
}

class LogDetailsWidget : public QTextBrowser
{
    Q_OBJECT
    Git::Log *_log;

public:
    LogDetailsWidget(QWidget *parent = nullptr);
    Git::Log *log() const;
    void setLog(Git::Log *newLog);

private:
    void createText();
    QString createHashLink(const QString &hash) const;
    QString createFileLink(const QString &hash) const;

private slots:
    void self_anchorClicked(const QUrl &url);

signals:
    void hashClicked(const QString &hash);
    void fileClicked(const QString &file);
};

#endif // LOGDETAILSWIDGET_H
