#include "KDChartAxis.h"
#include "KDChartAxis_p.h"
// #include <QtXml/QDomDocumentFragment>
// #include <KDChartTextAttributes.h>
// #include <QStringList>
// #include <KDChartGridAttributes.h>
// #include <qpen.h>
// #include <qvariant.h>
// #include <QDateTime>
// #include <KDChartAxisParams.h>

using namespace KDChart;

Axis::Axis ( AbstractDiagram* parent )
    : _d ( new Private() )
{   // FIXME decide internal structure, how to save parent
    init();
}

Axis::~Axis()
{
    delete _d; _d = 0;
}

Axis::Private::Private()
{
    // PENDING(miroslav) Code from KDChartAxis::Private::Private goes here
}


Axis::Private::~Private()
{
    // PENDING(miroslav) Code from KDChartAxis::Private::~Private goes here
}


void Axis::init()
{
}


// old stuff follows, needs to be reintegrated:

KDChartAxis::Private::Private() :
    type( AxisTypeUnknown ),
    position( AxisPosBottom ),
    title(),
    titleTextAttributes(),
    labels(),
    shortLabels(),
    labelTextAttributes(),
    gridAttributes(),
    labelsTouchEdges( false ),
    startLabel(),
    endLabel(),
    scaleTo( 0 ),
    digitsAfterDecimalPoint( 0 ),
//    labelsDateTimeFormat( QString::fromUtf8( KDCHART_AXIS_LABELS_AUTO_DATETIME_FORMAT ) ), // PENDING(kalle) fix class
    labelTextsFromDataRow( LabelsFromDataRowGuess ),
    labelTextsDataRow( 0 ),
    decimalPoint( QObject::tr( ".", "decimal point" ) ),
    thousandsPoint( QObject::tr( ",", "thousands seperator" ) ),
    labelsPrefix(),
    labelsPostfix(),
    labelsTotalLen( 0 ),
    labelsPadFill( ' ' ),
    labelsBlockAlign( true ),
    areaMode( AxisAreaModeAutoSize ),
    areaMin( -100 ),
    areaMax( 0 ),
    trueAreaSize( 0 ),
    trueAreaRect( QRect() ),
    useAvailableSpaceFrom( 0 ),
    useAvailableSpaceTo( -1000 ),
    calcMode( AxisCalcLinear ),
    referenceAxis( 0 ),
    lineVisible( true ),
    linePen( QPen( Qt::black, 1 ) ),
    valueStartIsExact( true ),
    // startValue( KDCHART_AXIS_LABELS_AUTO_LIMIT ), // PENDING(kalle) Fix class
    // endValue( KDCHART_AXIS_LABELS_AUTO_LIMIT ), // PENDING(kalle) Fix class
    lowerRangeLimit( 0 ),
    upperRangeLimit( 0 ),
    // valueDelta( KDCHART_AXIS_LABELS_AUTO_DELTA ), // PENDING(kalle) Fix class
    valueDeltaScale( ValueScaleNumerical ),
    // valueLeaveOut( KDCHART_AXIS_LABELS_AUTO_LEAVEOUT ), // PENDING(kalle) Fix class
    valuesDecreasing( false ),
    trueDeltaPixels( 0.0 ),
    trueLow( 0.0 ),
    trueHigh( 0.0 ),
    trueDelta( 0.0 ),
    trueDtLow(),
    trueDtHigh(),
    trueDtScale( ValueScaleDay ),
    zeroLineStartX( 0.0 ),
    zeroLineStartY( 0.0 ),
    dtLowPosX( 0.0 ),
    dtLowPosY( 0.0 ),
    dtHighPosX( 0.0 ),
    dtHighPosY( 0.0 ),
    maxEmptyInnerSpan( 90 ),
    steadyValueCalc( true )
{
}

KDChartAxis::Private::~Private() {}



#define d d_func()

KDChartAxis::KDChartAxis( QWidget* parent ) :
    KDChartArea( new Private(), parent )
{
    init();
}

KDChartAxis::~KDChartAxis()
{
}


void KDChartAxis::init()
{
}

#ifdef TEMPORARILY_REMOVED

KDChartArea * KDChartAxis::clone() const
{
    KDChartAxis * retVal = new KDChartAxis();

    retVal->copyRelevantDetailsFrom( this );

    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: void KDChartAxis::clone()" );

    return retVal;
}
#endif

void KDChartAxis::copyRelevantDetailsFrom( const KDChartAxis* axis )
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: void KDChartAxis:: KDChartAxis::copyRelevantDetailsFrom( const KDChartAxis* axis )" );
}

void KDChartAxis::paint( QPainter *, const PaintContext & context ) const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: void KDChartAxis::paint( QPainter *, const PaintContext & context ) const" );
}

QDomDocumentFragment KDChartAxis::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment KDChartAxis::toXML() const" );
    return QDomDocumentFragment();
}


static bool vertPosition( KDChartAxis::AxisPos pos )
{
    return pos == KDChartAxis::AxisPosLeft || pos == KDChartAxis::AxisPosCircular || pos == KDChartAxis::AxisPosRight;
}
static bool horizPosition( KDChartAxis::AxisPos pos )
{
    return pos == KDChartAxis::AxisPosBottom || pos == KDChartAxis::AxisPosSagittal || pos == KDChartAxis::AxisPosTop;
}

bool KDChartAxis::hasVerticalPosition() const
{
    return vertPosition( position() );
}

bool KDChartAxis::hasHorizontalPosition() const
{
    return horizPosition( position() );
}


void KDChartAxis::setType( AxisType axisType )
{
    d->type = axisType;
    switch( axisType ){
        case AxisTypeY:
            if( !vertPosition( position() ) )
                setPosition( AxisPosLeft );
            break;
        case AxisTypeX:
            if( !horizPosition( position() ) )
                setPosition( AxisPosBottom );
            break;
        default:
            qWarning( "Warning: not supported axis type in KDChartAxis::setType()" );
            break;
    }
}

KDChartAxis::AxisType KDChartAxis::type() const
{
    return d->type;
}

void KDChartAxis::setPosition( AxisPos position )
{
    d->position = position;
}

KDChartAxis::AxisPos KDChartAxis::position() const
{
    return d->position;
}

void KDChartAxis::setTitle( const QString& title )
{
    d->title = title;
}

QString KDChartAxis::title() const
{
    return d->title;
}

void KDChartAxis::setTitleTextAttributes( const TextAttributes& titleTextAttributes )
{
    d->titleTextAttributes = titleTextAttributes;
}

TextAttributes KDChartAxis::titleTextAttributes() const
{
    return d->titleTextAttributes;
}

void KDChartAxis::setLabels( const QStringList &labels )
{
    d->labels = labels;
}

QStringList KDChartAxis::labels() const
{
    return d->labels;
}

void KDChartAxis::setShortLabels( const QStringList &shortLabels )
{
    d->shortLabels = shortLabels;
}

QStringList KDChartAxis::shortLabels() const
{
    return d->shortLabels;
}

void KDChartAxis::setLabelTextAttributes( const TextAttributes& a )
{
    d->labelTextAttributes = a;
}

TextAttributes KDChartAxis::labelTextAttributes() const
{
    return d->labelTextAttributes;
}

void KDChartAxis::setGridAttributes( const GridAttributes& a )
{
    d->gridAttributes = a;
}

GridAttributes KDChartAxis::gridAttributes() const
{
    return d->gridAttributes;
}

void KDChartAxis::setAxisLabelsTouchEdges( bool labelsTouchEdges )
{
    d->labelsTouchEdges = labelsTouchEdges;
}

bool KDChartAxis::axisLabelsTouchEdges() const
{
    return d->labelsTouchEdges;
}

void KDChartAxis::setStartLabel( const QString& startLabel )
{
    d->startLabel = startLabel;
}

QString KDChartAxis::startLabel() const
{
    return d->startLabel;
}

void KDChartAxis::setEndLabel( const QString& endLabel )
{
    d->endLabel = endLabel;
}

QString KDChartAxis::endLabel() const
{
    return d->endLabel;
}

void KDChartAxis::setLabelFormat( int scaleTo, int digitsAfterDecimalPoint )
{
    d->scaleTo = scaleTo;
    d->digitsAfterDecimalPoint = digitsAfterDecimalPoint;
}

int KDChartAxis::labelScale() const
{
    return d->scaleTo;
}

int KDChartAxis::digitsAfterDecimalPoint() const
{
    return d->digitsAfterDecimalPoint;
}

void KDChartAxis::setLabelsDateTimeFormat( const QString& format )
{
    d->labelsDateTimeFormat = format;
}

QString KDChartAxis::labelsDateTimeFormat() const
{
    return d->labelsDateTimeFormat;
}

void KDChartAxis::setLabelTextsFromDataRow( int row, LabelsFromDataRow mode )
{
    d->labelTextsDataRow = row;
    d->labelTextsFromDataRow = mode;
}

KDChartAxis::LabelsFromDataRow KDChartAxis::axisLabelTextsFromDataRow() const
{
    return d->labelTextsFromDataRow;
}

int KDChartAxis::labelTextsDataRow() const
{
    return d->labelTextsDataRow;
}

void KDChartAxis::setAxisLabelsRadix( const QString& decimalPoint,
                                      const QString& thousandsPoint )
{
    d->decimalPoint = decimalPoint;
    d->thousandsPoint = thousandsPoint;
}

QString KDChartAxis::axisLabelsDecimalPoint() const
{
    return d->decimalPoint;
}

QString KDChartAxis::axisLabelsThousandsPoint() const
{
    return d->thousandsPoint;
}

void KDChartAxis::setAxisLabelsFormat( const QString& prefix,
                                       const QString& postfix,
                                       int totalLen, // needed?
                                       const QChar& padFill,
                                       bool blockAlign )
{
    d->labelsPrefix = prefix;
    d->labelsPostfix = postfix;
    d->labelsTotalLen = totalLen;
    d->labelsPadFill = padFill;
    d->labelsBlockAlign = blockAlign;
}

QString KDChartAxis::axisLabelsPrefix() const
{
    return d->labelsPrefix;
}

QString KDChartAxis::axisLabelsPostfix() const
{
    return d->labelsPostfix;
}

int KDChartAxis::axisLabelsTotalLen() const
{
    return d->labelsTotalLen;
}

QChar KDChartAxis::axisLabelsPadFill() const
{
    return d->labelsPadFill;
}

bool KDChartAxis::axisLabelsBlockAlign() const
{
    return d->labelsBlockAlign;
}

void KDChartAxis::setAxisAreaMode( AxisAreaMode axisAreaMode )
{
    d->areaMode = axisAreaMode;
}

KDChartAxis::AxisAreaMode KDChartAxis::axisAreaMode() const
{
    return d->areaMode;
}

void KDChartAxis::setAxisAreaMin( int axisAreaMin )
{
    d->areaMin = axisAreaMin;
}

int KDChartAxis::axisAreaMin() const
{
    return d->areaMin;
}

void KDChartAxis::setAxisUseAvailableSpace( int useAvailableSpaceFrom,
                                            int useAvailableSpaceTo )
{
    d->useAvailableSpaceFrom = useAvailableSpaceFrom;
    d->useAvailableSpaceTo = useAvailableSpaceTo;
}

int KDChartAxis::axisUseAvailableSpaceFrom() const
{
    return d->useAvailableSpaceFrom;
}

int KDChartAxis::axisUseAvailableSpaceTo() const
{
    return d->useAvailableSpaceTo;
}

void KDChartAxis::setAxisAreaMax( int areaMax )
{
    d->areaMax = areaMax;
}


int KDChartAxis::axisAreaMax() const
{
    return d->areaMax;
}

void KDChartAxis::setAxisArea( AxisAreaMode areaMode,
                               int areaMin,
                               int areaMax )
{
    d->areaMode = areaMode;
    d->areaMin = areaMin;
    d->areaMax = areaMax;
}

void KDChartAxis::setAxisCalcMode( AxisCalcMode calcMode )
{
    d->calcMode = calcMode;
}

KDChartAxis::AxisCalcMode KDChartAxis::axisCalcMode() const
{
    return d->calcMode;
}

void KDChartAxis::setIsometricReferenceAxis( KDChartAxis *referenceAxis )
{
    d->referenceAxis = referenceAxis;
}

KDChartAxis * KDChartAxis::isometricReferenceAxis() const
{
    return d->referenceAxis;
}

void KDChartAxis::setTrueAreaSize( int trueAreaSize ) { d->trueAreaSize = trueAreaSize; }
int KDChartAxis::trueAreaSize() const { return d->trueAreaSize; }
void KDChartAxis::setTrueAreaRect( const QRect& trueAreaRect ) { d->trueAreaRect = trueAreaRect; }
QRect KDChartAxis::trueAreaRect() const { return d->trueAreaRect; }

void KDChartAxis::setLineVisible( bool lineVisible )
{
    d->lineVisible = lineVisible;
}

bool KDChartAxis::lineVisible() const
{
    return d->lineVisible;
}

void KDChartAxis::setLinePen( const QPen& pen )
{
    d->linePen = pen;
}

QPen KDChartAxis::linePen() const
{
    return d->linePen;
}

void KDChartAxis::setValueStartIsExact( bool isExactValue )
{
    d->valueStartIsExact = isExactValue;
}

bool KDChartAxis::valueStartIsExact() const
{
    return d->valueStartIsExact;
}

void KDChartAxis::setStartValue( const QVariant& axisValueStart )
{
    d->startValue = axisValueStart;
}

QVariant KDChartAxis::startValue() const
{
    return d->startValue;
}

void KDChartAxis::setEndValue( const QVariant& axisValueEnd )
{
    d->endValue = axisValueEnd;
}

QVariant KDChartAxis::endValue() const
{
    return d->endValue;
}

void KDChartAxis::setLowerRangeLimit( int lowerLimit )
{
    d->lowerRangeLimit = lowerLimit;
}

int KDChartAxis::lowerRangeLimit() const
{
    return d->lowerRangeLimit;
}

void KDChartAxis::setUpperRangeLimit( int upperLimit )
{
    d->upperRangeLimit = upperLimit;
}

int KDChartAxis::upperRangeLimit() const
{
    return d->upperRangeLimit;
}

void KDChartAxis::setValueDelta( const double valueDelta,
                                 ValueScale scale )
{
    d->valueDelta = valueDelta;
    d->valueDeltaScale = scale;
}

double KDChartAxis::valueDelta() const
{
    return d->valueDelta;
}

KDChartAxis::ValueScale KDChartAxis::valueDeltaScale() const
{
    return d->valueDeltaScale;
}

void KDChartAxis::setValueLeaveOut( const int leaveOut )
{
    d->valueLeaveOut = leaveOut;
}

int KDChartAxis::valueLeaveOut() const
{
    return d->valueLeaveOut;
}

void KDChartAxis::setValuesDecreasing( bool valuesDecreasing )
{
    d->valuesDecreasing = valuesDecreasing;
}

bool KDChartAxis::valuesDecreasing() const
{
    return d->valuesDecreasing;
}

void KDChartAxis::setTrueDeltaPixels( double nDeltaPixels )
{
    d->trueDeltaPixels = nDeltaPixels;
}

double KDChartAxis::trueDeltaPixels() const
{
    return d->trueDeltaPixels;
}

void KDChartAxis::setTrueLowHighDelta( double nLow, double nHigh, double nDelta )
{
    d->trueLow = nLow;
    d->trueHigh = nHigh;
    d->trueDelta = nDelta;
}

double KDChartAxis::trueLow() const
{
    // PENDING(khz) FIXME
#ifdef FIXME
    return d->trueLow;
#else
    qWarning( "KDChartAxis::trueLow()  Sorry, not implemented: Computing of true axis low." );
    return -50.0;
#endif
}

double KDChartAxis::trueHigh() const
{
    // PENDING(khz) FIXME
#ifdef FIXME
    return d->trueHigh;
#else
    qWarning( "KDChartAxis::trueHigh()  Sorry, not implemented: Computing of true axis high." );
    return 50.0;
#endif
}

double KDChartAxis::trueDelta() const
{
    return d->trueDelta;
}

void KDChartAxis::setTrueDtLowHighDeltaScale( const QDateTime &dtLow, const QDateTime &dtHigh,
                                              ValueScale dtDeltaScale )
{
    d->trueDtLow = dtLow;
    d->trueDtHigh = dtHigh;
    d->trueDtScale = dtDeltaScale;
}

void KDChartAxis::setTrueDtLow( const QDateTime& dtLow )
{
    d->trueDtLow = dtLow;
}

void KDChartAxis::setTrueDtHigh( const QDateTime& dtHigh )
{
    d->trueDtHigh = dtHigh;
}

void KDChartAxis::setTrueDtScale( ValueScale scale )
{
    d->trueDtScale = scale;
}

QDateTime KDChartAxis::trueDtLow() const
{
    return d->trueDtLow;
}

QDateTime KDChartAxis::trueDtHigh() const
{
    return d->trueDtHigh;
}

KDChartAxis::ValueScale KDChartAxis::trueDtDeltaScale() const
{
    return d->trueDtScale;
}


void  KDChartAxis::setZeroLineStart( double x, double y )
{
    d->zeroLineStartX = x;
    d->zeroLineStartY = y;
}
double KDChartAxis::zeroLineStartX() const { return d->zeroLineStartX; }
double KDChartAxis::zeroLineStartY() const { return d->zeroLineStartY; }

void KDChartAxis::setDtLowPos( double x, double y )
{
    d->dtLowPosX = x;
    d->dtLowPosY = y;
}
double KDChartAxis::dtLowPosX() const { return d->dtLowPosX; }
double KDChartAxis::dtLowPosY() const { return d->dtLowPosY; }

void KDChartAxis::setDtHighPos( double x, double y )
{
    d->dtHighPosX = x;
    d->dtHighPosY = y;
}
double KDChartAxis::dtHighPosX() const { return d->dtHighPosX; }
double KDChartAxis::dtHighPosY() const { return d->dtHighPosY; }


void KDChartAxis::setMaxEmptyInnerSpan( const int maxEmpty )
{
    d->maxEmptyInnerSpan = maxEmpty;
}

int KDChartAxis::axisMaxEmptyInnerSpan() const
{
    return d->maxEmptyInnerSpan;
}

void KDChartAxis::setAxisSteadyValueCalc( bool steadyValueCalc )
{
    d->steadyValueCalc = steadyValueCalc;
}

bool KDChartAxis::axisSteadyValueCalc() const
{
    return d->steadyValueCalc;
}

void KDChartAxis::setGeometry( const QRect& rect, double averageValueP1000 )
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: void KDChartAxis::setGeometry()" );
}

QRect KDChartAxis::geometry() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: void KDChartAxis::geometry() const" );
    return QRect();
}
