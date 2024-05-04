#ifndef PUZZLEWIDGET_H
#define PUZZLEWIDGET_H

#include <QPoint>
#include <QPixmap>
#include <QList>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
QT_END_NAMESPACE

class PuzzleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PuzzleWidget(int imageSize, QWidget *parent = nullptr);
    void setPixmap(QPixmap& pixmap);
    void clear();

    int pieceSize() const;
    int imageSize() const;

signals:
    void puzzleCompleted();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    struct Piece {
        QPixmap pixmap;
        QRect rect;
        QPoint location;
    };

    int getPieceIdx(const QRect &pieceRect) const;
    const QRect squareUnderPt(const QPoint &position) const;

public:
    bool playing;

protected:
    QList<Piece> pieces;
    QPixmap pzlImage;
    QRect highlightedRect;
    int inPlace;
    int m_ImageSize;
};

#endif // PUZZLEWIDGET_H
