#ifndef CHOOSELEVELSCENE_H
#define CHOOSELEVELSCENE_H
#include "mainwindow.h"
#include <QMainWindow>
namespace Ui {
class ChooseLevelScene;
}

class ChooseLevelScene : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChooseLevelScene(QWidget *parent = nullptr);
    ~ChooseLevelScene();
    //重写绘图事件
    void paintEvent(QPaintEvent *);
    //游戏场景对象指针
    MainWindow * mainwindow = NULL;

private:
    Ui::ChooseLevelScene *ui;
signals:
    //写一个自定义信号，告诉主场景  点击了返回
    void chooseSceneBack();
private slots:
    void on_backBtn_clicked();
    void on_LEVEL2_clicked();
    void on_LEVEL3_clicked();
    void on_LEVEL4_clicked();
    void on_LEVEL5_clicked();

};

#endif // CHOOSELEVELSCENE_H
