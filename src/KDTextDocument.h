#ifndef KDTEXTDOCUMENT_H
#define KDTEXTDOCUMENT_H

#include <QTextDocument>

class KDTextDocument : public QTextDocument
{
    Q_OBJECT

public:
    KDTextDocument( QObject* parent = 0 ) :
        QTextDocument( parent ), mHintValid( false ) {}
    KDTextDocument( const QString& text, QObject* parent = 0 ) :
        QTextDocument( text, parent ), mHintValid( false ) {}

    virtual QSize sizeHint();
    virtual QSize minimumSizeHint();

protected:
    QSize sizeForWidth( int width );

    mutable bool mHintValid;
    mutable QSize mSizeHint;
    mutable QSize mMinimumSizeHint;
};


#endif /* KDTEXTDOCUMENT_H */

