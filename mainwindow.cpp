#include "mainwindow.h"
#include "pieceslist.h"
#include "imagelist.h"
#include "puzzlewidget.h"

#include <QtWidgets>
#include <stdlib.h>
#include <QDateTime>
#include <QDesktopServices>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    setupWidgets();
    createStatusBar();

    timer = new QTimer;
    timer->setInterval(1000);

    connect(timer, &QTimer::timeout, this, &MainWindow::on_timer_Timeout);
    connect(imgList, &QListWidget::itemDoubleClicked, this, &MainWindow::on_itemDoubleClicked);

    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setWindowTitle(QStringLiteral("吾爱吾校"));
    loadAllImages();
    setWindowIcon(QIcon(":/images/WinIcon.png"));

    memGatewayAct->trigger();
}

void MainWindow::on_start()
{
    if (!pzlWidget->playing)
    {
        setupPuzzle();
        timer->start();
        imgList->setEnabled(false);
        pzlWidget->playing = true;
    }
    else
    {
        pzlWidget->playing = false;
        imgList->setEnabled(true);
        pzlWidget->update();
        timer->stop();
    }
}

void MainWindow::on_scut_url()
{
    QDesktopServices::openUrl(QUrl("https://www.scut.edu.cn"));
}

void MainWindow::on_scut_tel()
{
    QDesktopServices::openUrl(QUrl("http://web.scut.edu.cn/2018/0531/c15721a269066/page.htm"));
}

void MainWindow::on_scut_hist()
{
    QDesktopServices::openUrl(QUrl("http://www2.scut.edu.cn/xsg/"));
}

void MainWindow::on_school_badge()
{
    changeImage(QStringLiteral("校徽"));
}

void MainWindow::on_school_motto()
{
    changeImage(QStringLiteral("校训"));
}

void MainWindow::on_school_gate()
{
    changeImage(QStringLiteral("校门"));
}

void MainWindow::on_mem_gateway()
{
    changeImage(QStringLiteral("牌坊"));
}

void MainWindow::on_motivation()
{
    QMessageBox::about(this, QStringLiteral("开发动机"),QStringLiteral(
                           "<p style=\"text-align:center\"><font color=\"red\" size=\"7\">吾爱吾校</font></p>"
                           "<br/>为我们大华工游子保留一个思念母校的港湾."
                           "<br/>莫笑少年江湖梦,谁不年少梦江湖!"
                           "<br/>"
                           "<br/>编程语言: C++11"
                           "<br/>开发时间: 2021中秋假期"
                           "<br/>"
                           "<br/>作者: Dr.Prof.Li (<font color=\"blue\">Chief Scientist</font>)"
                           "<br/>版本: V1.01"
                           "<br/>联系: 通过华工数学校友群、华工中山校友群、二流子校友群加微信"
                           ));
}

void MainWindow::loadImage(const QString &fileName)
{
    QPixmap newImage;

    if (!newImage.load(fileName))
    {
        QMessageBox::warning(this, tr("Open Image"),
                             tr("The image file could not be loaded."),
                             QMessageBox::Close);
        return;
    }
    pzlImage = newImage;

    int size = qMin(pzlImage.width(), pzlImage.height());
    pzlImage = pzlImage.copy((pzlImage.width() - size) / 2,
                             (pzlImage.height() - size) / 2, size, size)
            .scaled(pzlWidget->width(), pzlWidget->height(),
                    Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    pzlWidget->setPixmap(pzlImage);

    setupPuzzle();
}

void MainWindow::on_game_over()
{
    QString text;

    text = QStringLiteral("耗时") + QString::number((QDateTime::currentDateTime()
                                                   .toMSecsSinceEpoch() - startTime) / 1000) + " s";
    QMessageBox::information(this, QStringLiteral("挑战成功!"), text, QMessageBox::Ok);

    timer->stop();
    pzlWidget->playing = false;
    imgList->setEnabled(true);
    startAct->setChecked(false);
}

void MainWindow::on_timer_Timeout()
{
    QString status;

    status = QString::number((QDateTime::currentDateTime().toMSecsSinceEpoch() - startTime) / 1000) + " s";
    statusBar()->showMessage(status);
}

void MainWindow::on_itemDoubleClicked(QListWidgetItem *item)
{
    loadImage(":/images/image/" + item->data(Qt::DisplayRole).toString() + ".jpg");
}

void MainWindow::setupPuzzle()
{
    pieList->clear();

    // Separate the puzzle image, then add them to the left list
    for (int y = 0; y < PIECE_COUNT; ++y)
    {
        for (int x = 0; x < PIECE_COUNT; ++x)
        {
            int pieceSize = pzlWidget->pieceSize();
            QPixmap pieceImage = pzlImage.copy(x * pieceSize, y * pieceSize, pieceSize, pieceSize);
            QPainter painter(&pieceImage);
            painter.drawRect(0, 0, pieceSize - 1, pieceSize - 1);
            pieList->addPiece(pieceImage, QPoint(x, y));
        }
    }

    // Shuffle the left list
    for (int i = 0; i < pieList->count(); ++i)
    {
        if (QRandomGenerator::global()->bounded(4) != 1)
        {
            QListWidgetItem *item = pieList->takeItem(i);
            pieList->insertItem(0, item);
        }
    }

    pzlWidget->clear();
    startTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    //    timer->start();
}

void MainWindow::loadAllImages()
{
    imgNames = QDir(":/images/image/").entryList();
    foreach(QString imgName, imgNames)
        imgList->addPixmap(imgName);
}

void MainWindow::createActions()
{
    QToolBar *mainToolBar = addToolBar(tr("Main"));

    startAct = new QAction(QStringLiteral("开始拼图"));
    startAct->setStatusTip(QStringLiteral("重开开始当前拼图游戏"));
    startAct->setCheckable(true);
    connect(startAct, &QAction::triggered, this, &MainWindow::on_start);
    mainToolBar->addAction(startAct);

    mainToolBar->addSeparator();

    schMottoAct = new QAction(QStringLiteral("校徽"));
    schMottoAct->setStatusTip(QStringLiteral("校徽拼图"));
    connect(schMottoAct, &QAction::triggered, this, &MainWindow::on_school_badge);
    mainToolBar->addAction(schMottoAct);

    schBadgeAct = new QAction(QStringLiteral("校训"));
    schBadgeAct->setStatusTip(QStringLiteral("校训拼图"));
    connect(schBadgeAct, &QAction::triggered, this, &MainWindow::on_school_motto);
    mainToolBar->addAction(schBadgeAct);

    schGateAct = new QAction(QStringLiteral("校门"));
    schGateAct->setStatusTip(QStringLiteral("校门拼图"));
    connect(schGateAct, &QAction::triggered, this, &MainWindow::on_school_gate);
    mainToolBar->addAction(schGateAct);

    memGatewayAct = new QAction(QStringLiteral("牌坊"));
    memGatewayAct->setStatusTip(QStringLiteral("牌坊拼图"));
    connect(memGatewayAct, &QAction::triggered, this, &MainWindow::on_mem_gateway);
    mainToolBar->addAction(memGatewayAct);

    mainToolBar->addSeparator();

    QAction *scutUrlAct = new QAction(QStringLiteral("母校主页"));
    scutUrlAct->setStatusTip(QStringLiteral("打开母校主页"));
    connect(scutUrlAct, &QAction::triggered, this, &MainWindow::on_scut_url);
    mainToolBar->addAction(scutUrlAct);

    QAction *telAct = new QAction(QStringLiteral("常用电话"));
    telAct->setStatusTip(QStringLiteral("校内常用电话"));
    connect(telAct, &QAction::triggered, this, &MainWindow::on_scut_tel);
    mainToolBar->addAction(telAct);

    QAction *histAct = new QAction(QStringLiteral("校史"));
    histAct->setStatusTip(QStringLiteral("网上校史馆"));
    connect(histAct, &QAction::triggered, this, &MainWindow::on_scut_hist);
    mainToolBar->addAction(histAct);

    mainToolBar->addSeparator();

    QAction *motAct = new QAction(QStringLiteral("动机"));
    motAct->setStatusTip(QStringLiteral("开发这个软件的动机"));
    connect(motAct, &QAction::triggered, this, &MainWindow::on_motivation);
    mainToolBar->addAction(motAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(QStringLiteral("准备就绪"));
}

void MainWindow::changeImage(const QString &imgName)
{
    QListWidgetItem *item = imgList->findItems(imgName, Qt::MatchExactly)[0];

    imgList->setCurrentItem(item);
    on_itemDoubleClicked(item);
}

void MainWindow::setupWidgets()
{
    QFrame *frame = new QFrame;
    QHBoxLayout *frameLayout = new QHBoxLayout(frame);
    pzlWidget = new PuzzleWidget(500);

    pieList = new PiecesList(pzlWidget->pieceSize(), this);
    imgList = new ImageList(pzlWidget->pieceSize(), this);

    connect(pzlWidget, &PuzzleWidget::puzzleCompleted,
            this, &MainWindow::on_game_over, Qt::QueuedConnection);

    frameLayout->addWidget(imgList);
    frameLayout->addWidget(pzlWidget);
    frameLayout->addWidget(pieList);

    setCentralWidget(frame);
}
