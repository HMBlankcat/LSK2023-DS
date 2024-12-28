#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QVector>
#include <QLabel>
#include <QGridLayout>
#include <vector>
#include <algorithm>
#include <random>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <queue>
#include <stack>
#include <QTimer>
#include <QPainter>
#include <QPixmap>
#include <QListView>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent,int level);
    ~MainWindow();
    bool eventFilter(QObject *obj, QEvent *event); //鼠标事件监视
    void paintEvent(QPaintEvent *);
private slots:

    void on_BtnRandom_clicked();

    void on_BtnReset_clicked();

    void on_BtnNextStep_clicked();

    void on_backBtn2_clicked();

signals:
    void chooseSceneBack();
private:
    Ui::MainWindow *ui;
    QVector<QVector<QLabel*>> Blocks; //Vector对象用于存储单元格指针
    std::vector<int> Nums; //存储初始序列
    int MinCount;
    int Width; //存储华容道规模
    void ClearGrid(); //清除华容道
    int MergeSort(std::vector<int> &nums,int left, int right); //归并求逆序数
    void InitialRandom(); //初始化随机华容道
    char isAroundSpare(int row,int col); //判断周围是否有空格
    void SwepBlocks(int o_row,int o_col,int t_row,int t_col); //交换格子
    bool isVictory(); //判断胜利

    //以下开始启发式搜索
    stack<char> path; //解决路径
    bool isLatest; //是否最新标识符

    int ManHattan(string s);  //计算曼哈顿距离
    void bfs(string st); //A*





};
#endif // MAINWINDOW_H
