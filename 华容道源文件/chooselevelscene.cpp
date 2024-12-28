#include "chooselevelscene.h"
#include "ui_chooselevelscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include "mainwindow.h"

ChooseLevelScene::ChooseLevelScene(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChooseLevelScene)
{
    ui->setupUi(this);
    // 初始化 mainwindow
    mainwindow = nullptr;

    // 监听选择关卡的返回按钮的信号
    connect(mainwindow, &MainWindow::chooseSceneBack, this, [=]() {
        mainwindow->hide(); // 将选择关卡场景隐藏掉
        this->show();      // 重新显示主场景
        delete mainwindow;  // 删除 mainwindow 对象
        mainwindow = nullptr;
    });




}


ChooseLevelScene::~ChooseLevelScene()
{
    delete ui;
}



void ChooseLevelScene::paintEvent(QPaintEvent *)
{
    //绘制背景图片
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/SATONODIAMOND.jpg");
    painter.setOpacity(0.5);//透明度设置为0.5
    painter.drawPixmap(0,0,this->width(),this->height(),pix);


}

void ChooseLevelScene::on_backBtn_clicked()
{
    QTimer::singleShot(500,this,[=]()
    {
        emit this->chooseSceneBack();
    });
}


void ChooseLevelScene::on_LEVEL2_clicked()
{
    this->hide();
    if (mainwindow == nullptr)
    {
        mainwindow = new MainWindow(nullptr,0);
        connect(mainwindow, &MainWindow::chooseSceneBack, this, [=]()
                {
                    mainwindow->hide();
                    this->show();
                    delete mainwindow;  // 删除 mainwindow 对象
                    mainwindow = nullptr;
                });
    }
    mainwindow->show();
}

void ChooseLevelScene::on_LEVEL3_clicked()
{

    this->hide();
    if (mainwindow == nullptr) {
        mainwindow = new MainWindow(nullptr,1);

        connect(mainwindow, &MainWindow::chooseSceneBack, this, [=]()
                {
                    mainwindow->hide();
                    this->show();
                    delete mainwindow;  // 删除 mainwindow 对象
                    mainwindow = nullptr;
                });
    }
    mainwindow->show();
}


void ChooseLevelScene::on_LEVEL4_clicked()
{
    this->hide();
    if (mainwindow == nullptr)
    {
        mainwindow = new MainWindow(nullptr,2);
        connect(mainwindow, &MainWindow::chooseSceneBack, this, [=]()
                {
                    mainwindow->hide();
                    this->show();
                    delete mainwindow;  // 删除 mainwindow 对象
                    mainwindow = nullptr;
                });
    }
    //mainwindow->setWidth(4); // 设置宽度为4
    mainwindow->show();
}


void ChooseLevelScene::on_LEVEL5_clicked()
{
    this->hide();
    if (mainwindow == nullptr)
    {
        mainwindow = new MainWindow(nullptr,3);
        connect(mainwindow, &MainWindow::chooseSceneBack, this, [=]()
                {
                    mainwindow->hide();
                    this->show();
                    delete mainwindow;  // 删除 mainwindow 对象
                    mainwindow = nullptr;
                });
        //mainwindow->setWidth(5); // 设置宽度为5
    }
    mainwindow->show();
}



