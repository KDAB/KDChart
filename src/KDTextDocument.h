#ifndef KDTEXTDOCUMENT_H
#define KDTEXTDOCUMENT_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QTextDocument>
#include <QSize>

/**
  * KDTextDocument is an internally used enhanced QTextDocument
  * \internal
  */
class KDTextDocument : public QTextDocument
{
    Q_OBJECT

public:
    explicit KDTextDocument( QObject* parent = 0 );
    explicit KDTextDocument( const QString& text, QObject* parent = 0 );
    ~KDTextDocument();

    QSize sizeHint();
    QSize minimumSizeHint();

private:
    QSize sizeForWidth( int width );

private:
    bool mHintValid;
    QSize mSizeHint;
    QSize mMinimumSizeHint;
};


#endif /* KDTEXTDOCUMENT_H */

