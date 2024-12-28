#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <string>
#include <regex>
#include <vector>
#include <QPainter>
#include <QPixmap>
#include <QHashFunctions>
#include <QObject>
#include <QFileInfo>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
const int HASH_SIZE = 100;
using namespace std;
struct HashNode
{
    string key;
    vector<pair<int, int>> location;
    HashNode(string &a, int row, int col) :key(a) { location.push_back(make_pair(row, col)); }
};
class Dictionary
{
public:
    Ui::MainWindow *ui;
    Dictionary()
    {
        HashTable = new vector<HashNode>[HASH_SIZE];
    }
    ~Dictionary() {}
    void CountWord(string word) const;//计数
    void LocateWord(string word) const;//定位
    void Empty();//清空文档
    bool LoadFile(QString &FileName);//加载文档

protected:
    vector<HashNode>* HashTable;//哈希表的创建
    int HashCreate(string& word) const;//哈希值的生成
    void InsertHashTable(string word,int row,int col);//插入哈希表
    int SearchHashTable(string &word) const;//搜索哈希表




};

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    Highlighter(QTextDocument *parent = 0);
    void SetText(QString text);//设置文本
    void SetNum(int exec = -1);// 设置需要高亮的单词
protected:
    void highlightBlock(const QString &text);
    QString Filetext;
    int num,count;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_BtnOpenFile_clicked();
    void on_BtnSearch_clicked();
    void on_BtnHLA_clicked();
    void paintEvent(QPaintEvent *);
    void on_listLocations_itemSelectionChanged();
private:
    Ui::MainWindow *ui;
    Dictionary dic;
    Highlighter *highlighter;
};


#endif // MAINWINDOW_H
