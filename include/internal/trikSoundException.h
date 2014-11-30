#ifndef TRIKSOUNDEXCEPTION_H
#define TRIKSOUNDEXCEPTION_H

#include <exception>

#include <QString>

class TrikSoundException: public std::exception
{
public:
    TrikSoundException(const char* msg):
        mMsg(msg)
    {}

    virtual ~TrikSoundException() throw()
    {}

    const char* what() const throw()
    {
        return mMsg.toAscii().data();
    }

private:
    QString mMsg;
};

#endif // TRIKSOUNDEXCEPTION_H
