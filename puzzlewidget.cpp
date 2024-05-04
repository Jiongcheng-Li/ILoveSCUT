#include "puzzlewidget.h"
#include "pieceslist.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>

PuzzleWidget::PuzzleWidget(int imageSize, QWidget *parent)
    : QWidget(parent), m_ImageSize(imageSize)
{
    playing = false;

    setAcceptDrops(true);
    setMinimumSize(m_ImageSize, m_ImageSize);
    setMaximumSize(m_ImageSize, m_ImageSize);
}

void PuzzleWidget::setPixmap(QPixmap& pixmap)
{
    pzlImage = pixmap;
}

void PuzzleWidget::clear()
{
    pieces.clear();
    highlightedRect = QRect();
    inPlace = 0;
    update();
}

void PuzzleWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(PiecesList::puzzleMimeType()))
        event->accept();
    else
        event->ignore();
}

void PuzzleWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    QRect updateRect = highlightedRect;
    highlightedRect = QRect();
    update(updateRect);
    event->accept();
}

void PuzzleWidget::dragMoveEvent(QDragMoveEvent *event)
{
    const QPoint pt = event->pos();
    QRect updateRect = highlightedRect.united(squareUnderPt(pt));

    if (event->mimeData()->hasFormat(PiecesList::puzzleMimeType())
            && getPieceIdx(squareUnderPt(pt)) == -1)
    {

        highlightedRect = squareUnderPt(pt);
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
    {
        highlightedRect = QRect();
        event->ignore();
    }

    update(updateRect);
}

void PuzzleWidget::dropEvent(QDropEvent *event)
{
    const QPoint pt = event->pos();
    const int piSize = pieceSize();

    if (event->mimeData()->hasFormat(PiecesList::puzzleMimeType())
            && getPieceIdx(squareUnderPt(pt)) == -1)
    {

        QByteArray pieceData = event->mimeData()->data(PiecesList::puzzleMimeType());
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        Piece piece;
        piece.rect = squareUnderPt(pt);
        dataStream >> piece.pixmap >> piece.location;

        pieces.append(piece);

        highlightedRect = QRect();
        update(piece.rect);

        event->setDropAction(Qt::MoveAction);
        event->accept();

        if (piece.location.x() == piece.rect.left() / piSize && piece.location.y() == piece.rect.top() / piSize)
        {
            inPlace++;
            //            qDebug() << inPlace;
            if (inPlace == PIECE_COUNT * PIECE_COUNT)
                emit puzzleCompleted();
        }
    }
    else
    {
        highlightedRect = QRect();
        event->ignore();
    }
}

int PuzzleWidget::getPieceIdx(const QRect &pieceRect) const
{
    const int size = pieces.size();
    int i;

    for (i = 0; i < size; ++i)
    {
        if (pieces.at(i).rect == pieceRect)
            return i;
    }
    return -1;
}

void PuzzleWidget::mousePressEvent(QMouseEvent *event)
{
    const int piSize = pieceSize();
    const QPoint pt = event->pos();
    QRect square = squareUnderPt(pt);
    const int found = getPieceIdx(square);

    if (found == -1)
        return;

    Piece piece = pieces.takeAt(found);

    if (piece.location.x() == square.left() / piSize && piece.location.y() == square.top() / piSize)
    {
        inPlace--;
        //        qDebug() << inPlace;
    }

    update(square);

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << piece.pixmap << piece.location;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(PiecesList::puzzleMimeType(), itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(pt - square.topLeft());
    drag->setPixmap(piece.pixmap);

    if (drag->exec(Qt::MoveAction) != Qt::MoveAction) // If fail in drag, insert back the piece
    {
        pieces.insert(found, piece);
        update(squareUnderPt(pt));

        if (piece.location.x() == square.left() / piSize && piece.location.y() == square.top() / piSize)
        {
            inPlace++;
            //            qDebug() << inPlace;
        }
    }
}

void PuzzleWidget::paintEvent(QPaintEvent *event)
{
    const int piSize = pieceSize();
    int x, y;
    QRect rec = rect();
    QPainter painter(this);

    if (!playing)
    {
        painter.drawPixmap(rec, pzlImage);
        return;
    }

    rec = rec.adjusted(0, 0, -1, -1);
    painter.fillRect(event->rect(), Qt::lightGray);
    painter.drawRect(rec);

    // Draw gridline
    for (x = 0; x <= m_ImageSize; x += piSize)
        painter.drawLine(x, 0, x, m_ImageSize);
    for (y = 0; y <= m_ImageSize; y += piSize)
        painter.drawLine(0, y, m_ImageSize, y);

    if (highlightedRect.isValid())
    {
        painter.setBrush(Qt::yellow); //QColor("#ffcccc"));
        painter.setPen(Qt::NoPen);
        painter.drawRect(highlightedRect.adjusted(0, 0, -1, -1));
    }

    for (const Piece &piece : pieces)
        painter.drawPixmap(piece.rect, piece.pixmap);
}

const QRect PuzzleWidget::squareUnderPt(const QPoint &position) const
{
    const int piSize = pieceSize();
    return QRect(position.x() / piSize * piSize, position.y() / piSize * piSize, piSize, piSize);
}

int PuzzleWidget::pieceSize() const
{
    return m_ImageSize / PIECE_COUNT;
}

int PuzzleWidget::imageSize() const
{
    return m_ImageSize;
}
