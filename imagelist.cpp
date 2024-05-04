#include "imagelist.h"

ImageList::ImageList(int pieceSize, QWidget *parent)
    : QListWidget(parent), m_PieceSize(pieceSize)
{
    setDragEnabled(false);
    setViewMode(QListView::IconMode);
    setIconSize(QSize(m_PieceSize * 2, m_PieceSize * 2));
    setSpacing(3);
    setAcceptDrops(false);

    setMinimumWidth(2.4 * pieceSize);
    setMaximumWidth(2.4 * pieceSize);
}

void ImageList::addPixmap(const QString imgName)
{
    addItem(new QListWidgetItem(QPixmap(":/images/image/" + imgName),
                                imgName.left(imgName.indexOf(".jpg"))));
}
