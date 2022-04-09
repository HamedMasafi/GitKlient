#ifndef GITTAG_H
#define GITTAG_H

#include <QString>


namespace Git {

class Tag
{
public:
    Tag();

    const QString &name() const;
    void setName(const QString &newName);

    const QString &message() const;
    void setMessage(const QString &newMessage);

    const QString &taggerEmail() const;
    void setTaggerEmail(const QString &newTaggerEmail);

private:
    QString _name;
    QString _message;
    QString _taggerEmail;
};

} // namespace Git

#endif // GITTAG_H
