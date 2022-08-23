#ifndef COMMANDPULL_H
#define COMMANDPULL_H

#include "abstractcommand.h"

#ifdef GIT_GUI
namespace Ui{
class CommandPullWidget;
}
#endif

namespace Git {

class CommandPull : public AbstractCommand
{    
public:
    CommandPull();
    ~CommandPull() override;
    QStringList generateArgs() const override;

    bool squash() const;
    void setSquash(bool newSquash);

    bool noFf() const;
    void setNoFf(bool newNoFf);

    bool ffOnly() const;
    void setFfOnly(bool newFfOnly);

    bool noCommit() const;
    void setNoCommit(bool newNoCommit);

    bool prune() const;
    void setPrune(bool newPrune);

    bool tags() const;
    void setTags(bool newTags);

    void parseOutput(const QByteArray &output, const QByteArray &errorOutput) override;
    bool supportWidget() const override;
    QWidget *createWidget() override;

    const QString &remote() const;
    void setRemote(const QString &newRemote);

    const QString &branch() const;
    void setBranch(const QString &newBranch);

private:
    bool _squash{false};
    bool _noFf{false};
    bool _ffOnly{false};
    bool _noCommit{false};
    bool _prune{false};
    bool _tags{false};
    QString _remote;
    QString _branch;
#ifdef GIT_GUI
    QWidget *_widget;
    Ui::CommandPullWidget *_ui;
#endif
};

} // namespace Git

#endif // COMMANDPULL_H
