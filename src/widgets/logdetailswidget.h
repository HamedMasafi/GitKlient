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
    Q_PROPERTY(bool enableCommitsLinks READ enableCommitsLinks WRITE setEnableCommitsLinks NOTIFY enableCommitsLinksChanged)

public:
    LogDetailsWidget(QWidget *parent = nullptr);
    Git::Log *log() const;
    void setLog(Git::Log *newLog);

    bool enableCommitsLinks() const;
    void setEnableCommitsLinks(bool newEnableCommitsLinks);

private:
    void createText();
    void appendHeading(QString &html, const QString &title, const short level = 2) const;
    void appendParagraph(QString &html, const QString &text) const;
    void appendParagraph(QString &html, const QString &name, const QString &value) const;
    void appendParagraph(QString &html, const QString &name, const QStringList &list) const;
    QString createHashLink(const QString &hash) const;
    QString createFileLink(const QString &hash) const;

    bool m_enableCommitsLinks{false};


private slots:
    void self_anchorClicked(const QUrl &url);

signals:
    void hashClicked(const QString &hash);
    void fileClicked(const QString &file);
    void enableCommitsLinksChanged();
};

#endif // LOGDETAILSWIDGET_H
