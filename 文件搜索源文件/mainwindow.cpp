#include "mainwindow.h"
#include "ui_mainwindow.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    count = 0;
/*QSyntaxHighlighter 类是实现 QTextDocument 语法高亮器的基类。
 * 语法高亮器会自动高亮显示 QTextDocument 中的部分文本。
 * 语法高亮器常在用户以特定格式输入文本（如源代码）时使用，可帮助用户阅读文本并识别语法错误。*/
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,dic()
{
    ui->setupUi(this);
    dic.ui = ui;
    highlighter = new Highlighter(ui->textOpened->document());//该属性包含文本编辑器的底层文档。
    setFixedSize(966,587);
}


MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::paintEvent(QPaintEvent *)
{
    //绘制背景图片
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/1691768301823.jpg");
    painter.setOpacity(0.5);//透明度设置为0.5
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}

void MainWindow::on_BtnOpenFile_clicked()
{
    /* QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"/home",tr("Images (*.png *.xpm *.jpg)"));*/
    //打开文件按钮的点击事件
    //ui->BtnOpenFile->move(ui->BtnOpenFile->geometry().x(),ui->BtnOpenFile->geometry().y()+6);
    QString Word1 = "打开文件";
    QString homePath = QDir::homePath();//父进程启动此应用程序时的目录。
    QString Geshi = "文本文件(*.txt)";
    QString File = QFileDialog::getOpenFileName(this,Word1,homePath,Geshi);
    //检验是否打开成功
    if(File.isEmpty())
    {
        return ;
    }
    //清空字典，适应打开一个文本又打开别的文本的情况
    dic.Empty();
    ui->textOpened->clear();
    if(dic.LoadFile(File))
    {

        ui->BtnHLA->setEnabled(true);
        ui->BtnSearch->setEnabled(true);
        highlighter->SetText("");//重置查找词
        highlighter->rehighlight();//清空高亮显示
        //[slot] void QSyntaxHighlighter::rehighlightBlock(const QTextBlock &block)
        //将高亮重新应用到给定的 QTextBlock 块。


    }
}

//void MainWindow::on_BtnOpenFile_released()
//{
//    ui->BtnOpenFile->move(ui->BtnOpenFile->geometry().x(),ui->BtnOpenFile->geometry().y()-6);
//};

//不会做 再说

//搜索按钮点击
void MainWindow::on_BtnSearch_clicked()
{
    string word = ui->lineWord->text().toStdString();//返回一个包含此 QString 中数据的 std::string 对象
    regex limit_reg("[a-z|A-Z]+");//匹配前面的子表达式一次或多次。要匹配 + 字符，请使用 \+。
    if(regex_match(word,limit_reg))//试图匹配正则表达式到整个字符序列
    {
        dic.LocateWord(word);
        highlighter->SetText(ui->lineWord->text());
        highlighter->rehighlight();
    }
    else
    {
        QMessageBox::critical(this,"输错了!!","请输入仅含有英文字符(a-z,A-Z)且不包含空格的标准的单词",QMessageBox::Close);

    }

}

//选中位置列表项的变化 单独高亮
void MainWindow::on_listLocations_itemSelectionChanged()
{
    // 选中位置列表项的变化事件
    highlighter->SetNum(ui->listLocations->currentRow());
    highlighter->rehighlight();
}

//高亮所有查找单词
void MainWindow::on_BtnHLA_clicked()
{
    ui->listLocations->clearSelection();
    highlighter->SetNum(-1);
    highlighter->rehighlight();
}

//定位单词的位置
void Dictionary::LocateWord(string word) const
{
    QString Num;
    QString Loc;
    int pos = HashCreate(word);  // 计算单词在哈希表中的位置
    int layer = SearchHashTable(word);  // 在哈希表中查找单词所在的层
    if(layer != -1)
    {
        //QString status = QString("Processing file %1 of %2: %3").arg(i).arg(total).arg(fileName);
        Num = QString("%1").arg(HashTable[pos][layer].location.size());
        ui->labNumbers->setText(Num);
        ui->listLocations->clear();//清空列表
        vector<pair<int,int>>::iterator location = HashTable[pos][layer].location.begin();
        for(; location != HashTable[pos][layer].location.end(); location++)
        {
            Loc.clear();
            QTextStream(&Loc) << "第" << location->first + 1 << "行," << "第" << location->second + 1 <<"个词";
            ui->listLocations->addItem(Loc);
        }

    }
    else
    {
        ui->labNumbers->setText("0");
        ui->listLocations->clear();
    }
}


void Dictionary::Empty()
{
    // 清空哈希表
    for (int i = 0; i < HASH_SIZE; i++)
        HashTable[i].clear();
}


//加载文档
bool Dictionary::LoadFile(QString &aFileName)
{
    int row = 0, col = 0; //用于统计行列
    QString Q_str;
    string line;
    string word;
    vector<string> v_line;
    regex limit_reg("[a-z|A-Z]+"); //无符号字母

    QFile aFile(aFileName);
    if(!aFile.exists() || !aFile.open(QIODevice::ReadOnly|QIODevice::Text))
        return false;
    QTextStream aStream(&aFile);
    QFileInfo OpenFileInfo;//显示所示文档的路径
    QString OpenFilePath;
    OpenFileInfo = QFileInfo(aFile);
    OpenFilePath = OpenFileInfo.filePath();
    ui->NowText2->setText(OpenFilePath);
    aStream.setAutoDetectUnicode(true);//自动转换成UTF-8

    while (!aStream.atEnd()) //按换行符获取行
    {
        Q_str = aStream.readLine();
        ui->textOpened->append(Q_str);
        line = Q_str.toStdString();//把QString转化成string对象
        sregex_token_iterator pos(line.begin(), line.end(), limit_reg, 0); //正则匹配获取单词
        decltype(pos) end;//模板类regex_token_iterator<>提供分词迭代器
        /*sregex_token_iterator是针对string类型的特化，
         * 需要注意的是最后一个参数，这个参数可以指定一系列整数值，
         * 用来表示你感兴趣的内容，此处的-1表示对于匹配的正则表达式之前的子序列感兴趣；
         * 而若指定0，则表示对于匹配的正则表达式感兴趣；
         * 还可对正则表达式进行分组，之后便能输入任意数字对应指定的分组。*/
        for (; pos != end; pos++)
        {
            InsertHashTable(pos->str(), row, col); //将单词插入哈希表
            col++;
        }
        col = 0; row++;
    }
    return true;
}


//建立哈希函数
int Dictionary::HashCreate(string& word) const
{
    int seed = 31;
    int hash = 0;
    int wordlength = static_cast<int>(word.length());
    for (int i = 0; i < wordlength; i++)
        hash = (hash * seed + word[i]) % HASH_SIZE; //哈希算法
    return hash % HASH_SIZE;
}


void Dictionary::InsertHashTable(string word, int row, int col)
{
    // 将单词插入哈希表
    bool find = false;
    int pos = HashCreate(word); //获取单词哈希值
    vector<HashNode>::iterator Point;
    for (Point = HashTable[pos].begin(); Point != HashTable[pos].end(); Point++) //遍历该哈希值的桶
    {
        if (Point->key == word) //该单词已经出现过
        {
            Point->location.push_back(make_pair(row, col)); //追加位置，将给定的其他字符串追加到本字符串的末尾。
            find = true;
            break;
        }
    }
    if (!find) //该单词未出现
    {
        HashTable[pos].push_back(HashNode(word, row, col)); //将该单词放入桶
    }
}

int Dictionary::SearchHashTable(string& word) const
{
    // 在哈希表中搜索单词
    int layer = 0;
    int pos = HashCreate(word);
    vector<HashNode>::iterator Point;
    for (Point = HashTable[pos].begin(); Point != HashTable[pos].end(); ++Point) //遍历该哈希值的桶
    {
        if (Point->key == word) //在桶内找到了单词
        {
            return layer; //返回在桶内的层级
        }
        layer++;
    }
    return -1; //没有找到返回-1
}



void Highlighter::highlightBlock(const QString &text)
{
    // 高亮显示文本块中匹配的单词
    if(Filetext.isEmpty())
    {
        return;
    }
    QTextCharFormat myClassFormat;
    myClassFormat.setFontWeight(QFont::Bold);
    myClassFormat.setBackground(Qt::yellow);
    QString pattern = Filetext;

    //QRegExp expression(pattern);
    QRegularExpression expression(pattern);
    QRegularExpressionMatchIterator i = expression.globalMatch(text);
    /*QRegularExpressionMatchIterator 对象是一个类似于 Java 的正向迭代器；
    可以通过调用 QRegularExpression::globalMatch() 函数获得。
    一个新的 QRegularExpressionMatchIterator 将被放置在第一个结果之前。
    然后，可以调用 hasNext() 函数检查是否有更多可用结果；
    如果有，next() 函数将返回下一个结果并推进迭代器。*/
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        if(count == num ||num == -1)
            setFormat(match.capturedStart(), match.capturedLength(), myClassFormat);
        count += 1;
    }//[protected] void QSyntaxHighlighter::setFormat(int start, int count, const QTextCharFormat &format)
    /*此函数应用于语法高亮显示器的当前文本块（即传递给 highlightBlock() 函数的文本）。
     *  指定的格式将应用于从起始位置开始的长度为计数字符的文本（如果计数为 0，则不执行任何操作）。
     *  format 中设置的格式属性会在显示时与直接存储在文档中的格式信息合并，例如之前使用 QTextCursor 函数设置的格式信息。
     * 请注意，通过此函数设置的格式不会修改文档本身。*/
}

void Highlighter::SetText(QString text)
{
    // 设置需要查找的单词
    Filetext=text;
    num = -1;
    Filetext.insert(0,"\\b");// 在单词前面插入 \b，表示单词的边界
    Filetext.append("\\b");// 在单词后面追加 \b，表示单词的边界
    //\b	匹配一个单词边界，即字与空格间的位置
    //确保匹配的单词是整个单词而不是部分匹配。

}

void Highlighter::SetNum(int exec)
{
    // 设置需要高亮的位置
    num = exec;
    count = 0;
}

