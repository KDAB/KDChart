/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "TableModel.h"

#include <QByteArray>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QtDebug>
#include <QtGlobal>

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

TableModel::~TableModel()
{
}

int TableModel::rowCount(const QModelIndex &) const
{
    return m_rows.size();
}

int TableModel::columnCount(const QModelIndex &) const
{
    return m_rows.isEmpty() ? 0 : m_rows.first().size();
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    // FIXME kdchart queries (-1, -1) for empty models
    if (index.row() == -1 || index.column() == -1) {
        qDebug() << "TableModel::data: row:"
                 << index.row() << ", column:" << index.column()
                 << ", rowCount:" << rowCount() << ", columnCount:"
                 << columnCount() << Qt::endl
                 << "TableModel::data: FIXME fix kdchart views to not query"
                    " model data for invalid indices!";
        return QVariant();
    }

    /*    qDebug () << "TableModel::data: row: "<< index.row() << ", column: "
              << index.column() << endl;*/
    Q_ASSERT(index.row() >= 0 && index.row() < rowCount());
    Q_ASSERT(index.column() >= 0 && index.column() < columnCount());

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return m_rows[index.row()][index.column()];
    } else {
        return QVariant();
    }
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (m_supplyHeaderData) {
            if (orientation == Qt::Horizontal) {
                // column header data
                if (!m_horizontalHeaderData.isEmpty())
                    result = m_horizontalHeaderData[section];
            } else {
                // row header data:
                if (!m_verticalHeaderData.isEmpty())
                    result = m_verticalHeaderData[section];
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

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
{
    Q_ASSERT(index.row() >= 0 && index.row() < rowCount());
    Q_ASSERT(index.column() >= 0 && index.column() < columnCount());

    if (role == Qt::EditRole) {
        m_rows[index.row()][index.column()] = value;
        emit dataChanged(index, index);
        return true;
    } else {
        return false;
    }
}

static QStringList splitLine(const QString &line)
{
    QStringList sl = line.split(QLatin1Char(','));
    QStringList ret;
    for (int i = 0; i < sl.size(); i++) {
        // get rid of leading and trailing whitespace and quotes
        QString s = sl.at(i).simplified();
        if (s.startsWith(QLatin1Char('\"'))) {
            s.remove(0, 1);
        }
        if (s.endsWith(QLatin1Char('\"'))) {
            s.remove(s.length() - 1, 1);
        }
        ret.append(s);
    }
    return ret;
}

bool TableModel::loadFromCSV(const QString &filename)
{
    QFile file(filename);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        qDebug() << "TableModel::loadFromCSV: file" << filename
                 << "does not exist or could not be opened";
        return false;
    }

    QStringList lines;
    while (!file.atEnd()) {
        lines.append(QString::fromUtf8(file.readLine()));
    }

    setTitleText(QString());
    m_rows.clear();
    m_rows.resize(qMax(0, lines.size() - (m_dataHasHorizontalHeaders ? 1 : 0)));

    for (int row = 0; row < lines.size(); ++row) {
        QStringList cells = splitLine(lines.at(row));

        QVector<QVariant> values(qMax(0, cells.size() - (m_dataHasVerticalHeaders ? 1 : 0)));

        for (int column = 0; column < cells.size(); ++column) {
            QString cell = cells.at(column);

            if (row == 0 && m_dataHasHorizontalHeaders) {
                // interpret the first row as column headers:
                // the first one is an exception: interpret that as title
                if (column == 0 && m_dataHasVerticalHeaders) {
                    setTitleText(cell);
                } else {
                    m_horizontalHeaderData.append(cell);
                }
            } else {
                if (column == 0 && m_dataHasVerticalHeaders) {
                    // interpret first column as row headers:
                    m_verticalHeaderData.append(cell);
                } else {
                    // try to interpret cell values as floating point
                    bool convertedOk = false;
                    qreal numeric = cell.toDouble(&convertedOk);
                    const int destColumn = column - (m_dataHasVerticalHeaders ? 1 : 0);
                    values[destColumn] = convertedOk ? numeric : (cell.isEmpty() ? QVariant() : cell);
                }
            }
        }
        const int destRow = row - (m_dataHasHorizontalHeaders ? 1 : 0);
        if (destRow >= 0) {
            m_rows[destRow] = values;
        }
    }

    beginResetModel();
    endResetModel();

    if (m_rows.isEmpty()) {
        qDebug() << "TableModel::loadFromCSV: table loaded, but no "
                    "model data found.";
    }
    return true;
}

void TableModel::clear()
{
    beginResetModel();
    m_rows.clear();
    endResetModel();
}
