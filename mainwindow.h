#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QTimer>

class PiecesList;
class ImageList;
class PuzzleWidget;
QT_BEGIN_NAMESPACE
class QListWidgetItem;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void loadImage(const QString &path);
    void setupPuzzle();
    void loadAllImages();

private slots:
    void on_start();
    void on_scut_url();
    void on_scut_tel();
    void on_scut_hist();
    void on_school_badge();
    void on_school_motto();
    void on_school_gate();
    void on_mem_gateway();
    void on_motivation();
    void on_game_over();
    void on_timer_Timeout();
    void on_itemDoubleClicked(QListWidgetItem *item);

private:
    void createActions();
    void setupWidgets();
    void createStatusBar();
    void changeImage(const QString& imgName);

    QAction *startAct;
    QAction *schMottoAct;
    QAction *schBadgeAct;
    QAction *schGateAct;
    QAction *memGatewayAct;

    QPixmap pzlImage;
    PiecesList *pieList;
    ImageList *imgList;
    PuzzleWidget *pzlWidget;
    QTimer *timer;
    qint64 startTime;
    QStringList imgNames;
};

#endif // MAINWINDOW_H
