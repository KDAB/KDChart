#include "KDTextDocument.h"
#include <QRect>
#include <QAbstractTextDocumentLayout>
#include <QtDebug>

// This is an internal class that mimicks some of the behavior of a
// QLabel with rich text assigned, this is mostly a workaround around
// QTextDocumentLayout not being a public class.

QSize KDTextDocument::sizeHint()
{
    if( !mHintValid )
        (void)minimumSizeHint();
    return mSizeHint;
}

QSize KDTextDocument::minimumSizeHint()
{
    if( mHintValid )
        return mMinimumSizeHint;

    mHintValid = true;
    mSizeHint = sizeForWidth( -1 );
    QSize sz(-1, -1);

    // PENDING(kalle) Cache
    sz.rwidth() = sizeForWidth( 0 ).width();
    sz.rheight() = sizeForWidth( 32000 ).height();
    if( mSizeHint.height() < sz.height())
        sz.rheight() = mSizeHint.height();

    mMinimumSizeHint = sz;
    return sz;
}


QSize KDTextDocument::sizeForWidth(int w)
{
#ifdef K
    QPixmap *pix = lpixmap;
    int hextra = 2 * margin;
    int vextra = hextra;
    QFontMetrics fm(q->fontMetrics());
    int xw = fm.width('x');
    if (!mov && !pix && !pic) {
        int m = extraMargin;
        if (m < 0 && frameWidth) // no indent, but we do have a frame
            m = (xw / 2 - margin) * 2;
        if (m >= 0) {
            int align = QStyle::visualAlignment(q->layoutDirection(), QFlag(this->align));
            if ((align & Qt::AlignLeft) || (align & Qt::AlignRight))
                hextra += m;
            if ((align & Qt::AlignTop) || (align & Qt::AlignBottom))
                vextra += m;
        }
    }
#endif

    setPageSize(QSize(0, 100000));

//    qDebug() << "KDTextDocument::sizeForWidth() returns " << documentLayout()->documentSize().toSize();
    return documentLayout()->documentSize().toSize();
}
