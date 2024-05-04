#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <QListWidget>

class ImageList : public QListWidget
{
    Q_OBJECT
public:
    explicit ImageList(int pieceSize, QWidget *parent = nullptr);
    void addPixmap(const QString imgName);

protected:
    int m_PieceSize;
};

#endif // IMAGELIST_H
