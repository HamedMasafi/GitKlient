#include "logdetailswidget.h"
#include "GitKlientSettings.h"
#include "git/gitlog.h"
#include "git/gitmanager.h"
#include "git/models/logsmodel.h"

#include <KLocalizedString>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
#include <QCalendar>
#endif

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
    if (!_log) {
        clear();
        return;
    }
    auto files = Git::Manager::instance()->changedFiles(_log->commitHash());
    QStringList filesHtml;

    for (auto i = files.begin(); i != files.end(); ++i) {
        QString color;
        switch (i.value()) {

        case Git::Manager::Modified:
            color = GitKlientSettings::diffModifiedColor().name();
            break;
        case Git::Manager::Added:
            color = GitKlientSettings::diffAddedColor().name();
            break;
        case Git::Manager::Removed:
            color = GitKlientSettings::diffRemovedColor().name();
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

#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QString date;
    qDebug() << "cal="<<GitKlientSettings::calendarType();
    QCalendar cal(GitKlientSettings::calendarType());
    /*switch (GitKlientSettings::calendarType()) {
    case SettingsHelper::CalendarType::Gregorian:
        cal = QCalendar(QCalendar::System::Gregorian);
        break;
    case SettingsHelper::CalendarType::Jalali:
        cal = QCalendar(QCalendar::System::Julian);
        break;
    case SettingsHelper::CalendarType::Milankovic:
        cal = QCalendar(QCalendar::System::Milankovic);
        break;
    case SettingsHelper::CalendarType::IslamicCivil:
        cal = QCalendar(QCalendar::System::IslamicCivil);
        break;
    }*/
    if (cal.isValid())
        date = _log->commitDate().toLocalTime().toString("yyyy-MM-dd HH:mm:ss", cal);
    else
        date = _log->commitDate().toLocalTime().toString();
#else
    auto date = _log->commitDate().toLocalTime().toString();
#endif

    clear();
    QString html;
    appendHeading(html, _log->subject());
    if (_log->refLog() != QString())
        appendParagraph(html, i18n("Ref"), _log->refLog());
    appendParagraph(html, i18n("Committer"), QStringLiteral("%1 &lt;%2&gt;").arg(_log->committerName(), _log->committerEmail()));
    appendParagraph(html, i18n("Author"), QStringLiteral("%1 &lt;%2&gt;").arg(_log->authorName(), _log->authorEmail()));
    appendParagraph(html, i18n("Date"), date);
    appendParagraph(html, i18n("Hash"), _log->commitHash());

    if (!_log->parents().empty())
        appendParagraph(html,
                        _log->parents().size() == 1 ? i18n("Parent") : i18n("Parents"),
                        parentHashHtml);
    if (!_log->childs().empty())
        appendParagraph(html,
                        _log->childs().size() == 1 ? i18n("Child") : i18n("Children"),
                        childsHashHtml);

    appendParagraph(html, i18n("Changed files"), filesHtml);

    setHtml(html);
}

void LogDetailsWidget::appendHeading(QString &html, const QString &title, const short level)
{
    html.append(QStringLiteral("<h%2>%1</h%2>").arg(title).arg(level));
}

void LogDetailsWidget::appendParagraph(QString &html, const QString &text)
{
    html.append(QStringLiteral("<p>%1</p>").arg(text));
}

void LogDetailsWidget::appendParagraph(QString &html, const QString &name, const QString &value)
{
    html.append(QStringLiteral("<p><b>%1:</b> %2</p>").arg(name, value));
}

void LogDetailsWidget::appendParagraph(QString &html, const QString &name, const QStringList &list)
{
    if (!list.size())
        return;

    html.append(QStringLiteral("<p><b>%1</b><ul>").arg(name));
    for (auto &l: list)
        html.append(QStringLiteral("<li>%1</li>").arg(l));
    html.append(QStringLiteral("</ul>"));
}

QString LogDetailsWidget::createHashLink(const QString &hash) const
{
    auto log = Git::Manager::instance()->logsModel()->findLogByHash(hash);
    if (!log)
        return QString();

    if (m_enableCommitsLinks)
        return QStringLiteral(R"(<a href="hash:%1">%2</a> )")
            .arg(log->commitHash(), log->subject());

    return log->subject();
}

QString LogDetailsWidget::createFileLink(const QString &file)
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

bool LogDetailsWidget::enableCommitsLinks() const
{
    return m_enableCommitsLinks;
}

void LogDetailsWidget::setEnableCommitsLinks(bool newEnableCommitsLinks)
{
    if (m_enableCommitsLinks == newEnableCommitsLinks)
        return;
    m_enableCommitsLinks = newEnableCommitsLinks;
    emit enableCommitsLinksChanged();
}
