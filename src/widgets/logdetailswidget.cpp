#include "logdetailswidget.h"
#include "git/gitlog.h"
#include "git/gitmanager.h"
#include "GitKlientSettings.h"

//#include <QCalendar>

LogDetailsWidget::LogDetailsWidget(QWidget *parent) : QTextBrowser(parent)
{
    connect(this,&QTextBrowser::anchorClicked, this, &LogDetailsWidget::self_anchorClicked);
}

Git::Log *LogDetailsWidget::log() const
{
    return _log;
}

void LogDetailsWidget::setLog(Git::Log *newLog)
{
    if(_log == newLog)
        return;

    _log = newLog;
    createText();
}

void LogDetailsWidget::createText()
{
    auto files = Git::Manager::instance()->changedFiles(_log->commitHash());
    QString filesHtml;

    for (auto i = files.begin(); i != files.end(); ++i) {
        QString color;
        switch (i.value()) {

        case Git::Manager::Modified:
            color = "blue";
            break;
        case Git::Manager::Added:
            color = "green";
            break;
        case Git::Manager::Removed:
            color = "red";
            break;

        case Git::Manager::Unknown:
        case Git::Manager::Unmodified:
        case Git::Manager::Renamed:
        case Git::Manager::Copied:
        case Git::Manager::UpdatedButInmerged:
        case Git::Manager::Ignored:
        case Git::Manager::Untracked:
            break;
        }
        filesHtml.append(
            QStringLiteral("<li><font color=%1>%2</a></li>").arg(color, createFileLink(i.key())));
    }
    QString parentHashHtml;
    for (auto &parent: _log->parents())
        parentHashHtml.append(createHashLink(parent));

    QString childsHashHtml;
    for (auto &child: _log->childs())
        childsHashHtml.append(createHashLink(child));

  /*  QCalendar::System system = QCalendar::System::Gregorian;
    switch (GitKlientSettings::calendarType()) {
    case GitKlientSettings::EnumCalendarType::Jalali:
        system = QCalendar::System::Jalali;
    }
    auto cal = QCalendar(system);*/
    auto date = _log->commitDate().toLocalTime().toString("yyyy-MM-dd");

    clear();
    QString html;
    appendParagraph(html, _log->subject());
    appendParagraph(html, tr("Branch"), _log->refLog());
    appendParagraph(html, tr("Committer"), QStringLiteral("%1 &lt;%2&gt;").arg(_log->committerName(), _log->committerEmail()));
    appendParagraph(html, tr("Author"), QStringLiteral("%1 &lt;%2&gt;").arg(_log->authorName(), _log->authorEmail()));
    appendParagraph(html, tr("Date"), date);
    appendParagraph(html, "Hash", createHashLink(_log->commitHash()));

    if (_log->parents().size())
        appendParagraph(html,
                        _log->parents().size() == 1 ? tr("Parent") : tr("Parents"),
                        parentHashHtml);
    if (_log->childs().size())
        appendParagraph(html,
                        _log->childs().size() == 1 ? tr("Child") : tr("Childs"),
                        childsHashHtml);
    appendParagraph(html, _log->body());
    appendParagraph(html, QStringLiteral("<b>Changed files:</b><ul>%1</ul>").arg(filesHtml));

    setHtml(html);

   /* setHtml(
        QStringLiteral(R"(
        Extra--%12--(%13)<br />
        <b>%1</b><br />
        Branch: %11<br />
        Committer: %4 &lt;%5&gt; <br />
        Date: %9 <br />
        Author: %6 &lt;%7&gt; <br />
        <br />
        Hash: <a href="%2">%2</a> <br />
        Parent: %3 <br />
        Childs: %14 <br />
        <br />
        %8
        <br />
        <b>Changed files:</b>
        <ul>%10</ul>
    )")
            .arg(_log->subject(),
                 _log->commitHash(),
                 parentHashHtml,
                 _log->committerName(),
                 _log->committerEmail(),
                 _log->authorName(),
                 _log->authorEmail(),
                 _log->body())
            .arg(date, filesHtml, _log->branch(), _log->extraData(), _log->refLog(), childsHashHtml));*/
}

void LogDetailsWidget::appendParagraph(QString &html, const QString &text) const
{
    html.append(QStringLiteral("<p>%1</p>").arg(text));
}

void LogDetailsWidget::appendParagraph(QString &html, const QString &name, const QString &value) const
{
    html.append(QStringLiteral("<p>%1: %2</p>").arg(name, value));
}

QString LogDetailsWidget::createHashLink(const QString &hash) const
{
    auto log = Git::Manager::instance()->logs().findByHash(hash);
    if (!log)
        return QString();

    return QStringLiteral(R"(<a href="hash:%1">%2</a> )")
        .arg(log->commitHash(), log->subject());
}

QString LogDetailsWidget::createFileLink(const QString &file) const
{
    return QStringLiteral(R"(<a href="file:%1">%1</a> )").arg(file);
}

void LogDetailsWidget::self_anchorClicked(const QUrl &url)
{
    auto scheme = url.scheme().toLower();

    if (scheme=="hash")
        emit hashClicked(url.path());

    if (scheme=="file")
        emit fileClicked(url.path());
}
