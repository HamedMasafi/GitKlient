#include "ignorefiledialog.h"

#include "git/gitmanager.h"

#include <QDebug>
#include <QFileInfo>

IgnoreFileDialog::IgnoreFileDialog(Git::Manager *git, const QString &filePath, QWidget *parent) :
      AppDialog(git, parent)
{
    setupUi(this);

    QFileInfo fi{filePath};

    if (fi.isDir())
        groupBoxFileName->hide();

    _path = fi.absolutePath() + "/";
    _path = _path.replace(git->path(), "");
    _name = fi.baseName();
    _ext = fi.suffix();

    if (_path == "/")
        radioButtonDirIgnoreFile->setEnabled(false);

    generateIgnorePattern();
}

void IgnoreFileDialog::generateIgnorePattern()
{
    QString s;
    if (radioButtonName->isChecked())
        s = _name + ".*";
    else if (radioButtonExt->isChecked())
        s = "*." + _ext;
    else {
        if (_ext.isEmpty())
            s = _name;
        else
            s = _name + "." + _ext;
    }

    if (radioButtonExactPath->isChecked())
        s = _path + s;

    lineEdit->setText(s);
    qDebug() << getIgnoreFile();
}

void IgnoreFileDialog::on_buttonBox_accepted()
{

}

QString IgnoreFileDialog::getIgnoreFile()
{
    if (radioButtonRootIgnoreFile->isChecked())
        return _git->path() + "/.gitignore";
    else if (radioButtonDirIgnoreFile->isChecked())
        return _git->path() + _path + ".gitignore";
    else
        return _git->path() + "/.git/info/exclude";
}
