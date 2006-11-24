#include <QtGlobal>
#include <QtDebug>
#include <QFile>
#include <QByteArray>
#include <QString>
#include <QStringList>

#include "TableModel.h"

TableModel::TableModel ( QObject * parent )
    : QAbstractTableModel ( parent ), m_supplyHeaderData( true )
{
}

TableModel::~TableModel ()
{
}

int TableModel::rowCount ( const QModelIndex & ) const
{
    if ( m_rows.size() > 0 )
    {
        return m_rows.size() - 1;
    } else {
        return 0;
    }
}

int TableModel::columnCount ( const QModelIndex & ) const
{
    if ( m_rows.size() == 0 )
    {
        return 0;
    } else {
        return m_rows[0].size() - 1;
    }
}

QVariant TableModel::data ( const QModelIndex & index, int role) const
{
    // FIXME kdchart queries (-1, -1) for empty models
    if ( index.row() == -1 || index.column() == -1 )
    {
        qDebug() << "TableModel::data: row: "
                 << index.row() << ", column: " << index.column()
                 << ", rowCount: " << rowCount() << ", columnCount: "
                 << columnCount() << endl
                 << "TableModel::data: FIXME fix kdchart views to not query"
            " model data for invalid indices!" << endl;
        return QVariant();
    }

//     qDebug () << "TableModel::data: row: "<< index.row() << ", column: "
//               << index.column() << endl;
    Q_ASSERT ( index.row() >= 0 && index.row() < rowCount() );
    Q_ASSERT ( index.column() >= 0 && index.column() < columnCount() );

    if ( role == Qt::DisplayRole || role == Qt::EditRole )
    {
        return m_rows[index.row() + 1] [index.column() + 1] ;
    } else {
        return QVariant();
    }
}

QVariant TableModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
    QVariant result;

    switch ( role )
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if ( m_supplyHeaderData ) {
            if ( orientation == Qt::Horizontal )
            {   // column header data:
                result = m_rows[0][section + 1];
            } else {
                // row header data:
                result = m_rows[section + 1][0];
            }
        }
        break;
    case Qt::TextAlignmentRole:
//        result = QVariant ( Qt::AlignHCenter | Qt::AlignHCenter );
        break;
    case Qt::DecorationRole:
    case Qt::ToolTipRole:
        break;
    default:
//        qDebug () << "TableModel::headerData: unknown role " << role << "." << endl;
        break;
    }
    return result;
}


bool TableModel::setData ( const QModelIndex & index, const QVariant & value, int role/* = Qt::EditRole */ )
{
    Q_ASSERT ( index.row() >= 0 && index.row() < rowCount() );
    Q_ASSERT ( index.column() >= 0 && index.column() < columnCount() );

    if ( role == Qt::EditRole )
    {
        m_rows[index.row() + 1] [index.column() + 1] = value;
        emit dataChanged( index, index );
        return true;
    } else {
        return false;
    }
}

bool TableModel::loadFromCSV ( const QString& filename )
{
    QFile file ( filename );
    QStringList data;

    if ( file.exists() && file.open ( QIODevice::ReadOnly )  )
    {
        while ( !file.atEnd() )
        {
            QString line = file.readLine();
            data.append ( line );
        }

        if ( data.size() > 0 )
        {
//             qDebug() << "TableModel::loadFromCSV: " << data.size()
//                      << " data rows found." << endl;
            m_rows.resize ( data.size() );

            // debugging code:
            int previousColumnCount = 0;

            for ( int row = 0; row < data.size(); ++row )
            {
                QStringList parts = data.at( row ).split ( QString( ',' ) );

                Q_ASSERT ( previousColumnCount == parts.size() || previousColumnCount == 0 );
                Q_ASSERT ( ( previousColumnCount = parts.size() ) || true );

                QVector<QVariant> values( parts.size() );

                for ( int column = 0; column < parts.size(); ++column )
                {
                    // get rid of trailing or leading whitespaces and quotes:
                    QString cell ( parts.at( column ).simplified() );

                    if ( cell.startsWith( "\"") )
                    {
                        cell.remove ( 0, 1 );
                    }
                    if ( cell.endsWith( "\"") )
                    {
                        cell.remove ( cell.length()-1, 1 );
                    }
                    parts[column] = cell;

                    if ( row == 0 )
                    {   // interpret the first row as column headers:
                        values[column] = QVariant ( parts.at ( column ) );
                        setHeaderData ( column,  Qt::Horizontal,  QVariant ( parts.at ( column ) ),
                                        Qt::DisplayRole );
                    } else {
                        if ( column == 0 )
                        {   // interpret first column as row headers:
                            values[column] = QVariant ( parts.at ( column ) );
                            setHeaderData ( row,  Qt::Vertical, QVariant ( parts.at ( column ) ) );
                        } else {
                            // interpret cell values as floating point:
                            bool convertedOk = false;
                            double value = parts.at ( column ).toDouble ( &convertedOk );
                            if ( convertedOk )
                            {
//                             Q_ASSERT_X ( convertedOk, "TableModel::loadFromCSV",
//                                          "Double values expected except first column or first row!" );
                                values[column] = QVariant ( value );
                            } else {
                                values[column] = parts.at( column );
                            }
                        }
                    }
                }

                m_rows[row] = values;
            }

        } else {
            m_rows.resize ( 0 );
        }

        reset();
        if ( m_rows.size () > 0 )
        {
//             qDebug() << "TableModel::loadFromCSV: table loaded, "
//                      << rowCount() << " rows, " << columnCount() << "columns."
//                      << endl;
        } else {
            qDebug() << "TableModel::loadFromCSV: table loaded, but no "
                "model data found." << endl;
        }
        return true;
    } else {
        qDebug() << "TableModel::loadFromCSV: file" << filename
                << "does not exist / or could not be opened" << endl;
        return false;
    }
}

void TableModel::clear()
{
    m_rows.resize( 0 );
    reset();
}
