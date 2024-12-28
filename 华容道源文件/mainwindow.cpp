#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent,int level)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //对一些关键信息进行初始化
    Width = 0;
    isLatest = false;
    MinCount = 65535;
    ui->ChosenWidth->setCurrentIndex(level);

    //对下拉框ComboBox进行一些样式表的改变
    for(int i = 0;i < ui->ChosenWidth->count();i++)
    {
        QIcon icon(":/BUTTONS.png");         //构造对象
        ui->ChosenWidth->setItemIcon(i,icon);  //显示图标


    }
    ui->ChosenWidth->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    ui->ChosenWidth->view()->window()->setAttribute(Qt::WA_TranslucentBackground);

    //combobox下拉框样式表生效
    ui->ChosenWidth->setView(new QListView());

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
    pix.load(":/background1.png");
    painter.setOpacity(0.7);//透明度设置为0.5
    painter.drawPixmap(0,0,this->width(),this->height(),pix);


}

/*如果此对象已被安装为被监视对象的事件过滤器，则会过滤事件。
 * 在重新实现此函数时，如果要过滤掉事件，即停止进一步处理该事件，则返回 true；否则返回 false。*/
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    int row = 0,col = 0;
    if (event->type() == QEvent::MouseButtonPress) //鼠标点击
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); //将 event 指针转换为QMouseEvent 指针，这样你就可以使用 MouseButtonPress 来访问QMouseEvent类的成员和方法。
        //当鼠标按钮在 widget 中被按下或释放，或鼠标光标被移动时，鼠标事件就会发生。
        if(mouseEvent->button() == Qt::LeftButton)
        {
            char direction = 'n';
            for(row = 0;row < Width; row++)
                for(col = 0;col < Width; col++)
                    if(Blocks[row][col] == obj)
                    {
                        direction = isAroundSpare(row,col);
                        switch(direction)
                        {
                        case 'n':
                            return false;
                        case 'r':
                            SwepBlocks(row,col,row,col+1);
                            isLatest = false;
                            return true;
                        case 'd':
                            SwepBlocks(row,col,row+1,col);
                            isLatest = false;
                            return true;
                        case 'l':
                            SwepBlocks(row,col,row,col-1);
                            isLatest = false;
                            return true;
                        case 'u':
                            SwepBlocks(row,col,row-1,col);
                            isLatest = false;
                            return true;
                        }
                    }
        }
        else
            return false;
    }
    else if(event->type() == QEvent::KeyPress) //键盘事件
    {
        QKeyEvent *KeyPress = static_cast<QKeyEvent*>(event);//将 event 指针转换为 QKeyEvent 指针，这样你就可以使用 KeyPress 来访问 QKeyEvent 类的成员和方法。

        for(row = 0 ; row < Width ;row++)
            for(col = 0 ; col < Width ; col++)
                if(Blocks[row][col]->text()=="0")
                {
                    switch(KeyPress->key())
                    {
                    case Qt::Key_Up:
                        if(row!=Width-1)
                        {
                            SwepBlocks(row+1,col,row,col);
                            isLatest = false;
                            return true;
                        }
                        break;
                    case Qt::Key_Down:
                        if(row!=0)
                        {
                            SwepBlocks(row-1,col,row,col);
                            isLatest = false;
                            return true;
                        }
                        break;
                    case Qt::Key_Left:
                        if(col!=Width-1)
                        {
                            SwepBlocks(row,col+1,row,col);
                            isLatest = false;
                            return true;
                        }
                        break;
                    case Qt::Key_Right:
                        if(col!=0)
                        {
                            SwepBlocks(row,col-1,row,col);
                            isLatest = false;
                            return true;
                        }
                        break;
                    default:
                        return false;

                    }
                }
    }
    else
        return QWidget::eventFilter(obj, event);
}


void MainWindow::on_BtnRandom_clicked()
{
    MinCount = 65535;
    ui->LabMin->setText("0");
    ClearGrid();
    QFont font("阿里妈妈东方大楷",20,QFont::Bold);
    QLabel* singleBlock;
    QGridLayout* GameGrid = ui->GameGrid;
    Width = ui->ChosenWidth->currentIndex()+2;//初始2X2的index为0 后依次递增
    InitialRandom(); //随机初始化
    Blocks.resize(Width);//动态改变容器大小,节省空间
    for(int i = 0;i < Width ; i++)
    {
        Blocks[i].resize(Width);
        for(int j = 0;j < Width; j++)
        {
            singleBlock = new QLabel(QString::number(Nums[i*Width+j]));
            singleBlock->installEventFilter(this); //安装事件过滤器
            singleBlock->setAlignment(Qt::AlignCenter);

            singleBlock->setStyleSheet("QLabel { background-color: lightblue; }");//改变背景样式
            singleBlock->setScaledContents(true);//该属性表示标签是否会缩放其内容以填充所有可用空间。
            singleBlock->setFrameShape(QFrame::StyledPanel);
            // 设置 QLabel 的字体
            singleBlock->setFont(font);
            GameGrid->addWidget(singleBlock,i,j);
            Blocks[i][j] = singleBlock;
        }
    }
    //处理空格
    Blocks[Width-1][Width-1]->setVisible(false);
    Blocks[Width-1][Width-1]->grabKeyboard(); //空格捕获键盘事件
    //与鼠标实现逻辑不同

}

void MainWindow::on_BtnReset_clicked()
{
    ClearGrid();
    QLabel* singleBlock;
    QFont font("阿里妈妈东方大楷",20,QFont::Bold);
    QGridLayout* GameGrid = ui->GameGrid;//抓取物件
    Blocks.resize(Width);
    for(int i = 0;i < Width ; i++)
    {
        Blocks[i].resize(Width);
        for(int j = 0;j < Width; j++)
        {
            singleBlock = new QLabel(QString::number(Nums[i*Width+j]));
            singleBlock->installEventFilter(this);
            singleBlock->setAlignment(Qt::AlignCenter);
            singleBlock->setStyleSheet("QLabel { background-color: lightblue; }");//改变背景样式
            singleBlock->setScaledContents(true);
            singleBlock->setFrameShape(QFrame::StyledPanel);//使用一个样式化的面板作为边框。
            // 设置 QLabel 的字体
            singleBlock->setFont(font);
            GameGrid->addWidget(singleBlock,i,j);
            Blocks[i][j] = singleBlock;
        }
    }
    //处理空格
    Blocks[Width-1][Width-1]->setVisible(false);
    Blocks[Width-1][Width-1]->grabKeyboard();
}


void MainWindow::on_BtnNextStep_clicked()
{
    if(!isLatest) //路径不是最新
    {
        for(int i = 0;i < path.size();i++)
            path.pop();
        string ChuShiBuJu;//构造初始布局的字符串
        for(int i = 0;i < Width; i++)
            for(int j = 0;j < Width; j++)
                ChuShiBuJu += Blocks[i][j]->text().toStdString();
        bfs(ChuShiBuJu);
        isLatest = true;
    }
    char Direct; //表示路径中的操作方向
    if(!path.empty()) //当前路径为最新
    {
        Direct=path.top();
        path.pop();

        for(int row = 0 ; row < Width ; row++)
            for(int col = 0 ; col < Width ; col++)
                if(Blocks[row][col]->text() == "0")
                {
                    switch(Direct) //进行操作
                    {
                    case 'd':
                        if(row != Width - 1)
                        {
                            SwepBlocks(row + 1,col,row,col);
                            return ;
                        }
                        break;
                    case 'u':
                        if(row != 0)
                        {
                            SwepBlocks(row - 1,col,row,col);
                            return ;
                        }
                        break;
                    case 'r':
                        if(col!=Width - 1)
                        {
                            SwepBlocks(row,col + 1,row,col);
                            return ;
                        }
                        break;
                    case 'l':
                        if(col != 0)
                        {
                            SwepBlocks(row,col - 1,row,col);
                            return ;
                        }
                        break;
                    default:
                        return ;

                    }
                }
    }

}

void MainWindow::ClearGrid()
{
    if(Width == 0)
        return;
    ui->LabCount->setText("0");
    for(int i = 0;i < Width; i++)
        for(int j = 0;j < Width; j++)
            delete Blocks[i][j];
    Blocks.clear();
    for(int i=0;i<path.size();i++)
        path.pop();
    isLatest = false;
}

int MainWindow::MergeSort(std::vector<int> &nums, int left, int right)//归并排序求逆序数
{
    if (left == right) //如果左右索引相等，表示当前段只有一个元素，不需要排序，逆序数为 0
        return 0;
    else
    {
        int mid = (left + right) / 2; //计算当前段的中间索引
        //对left到mid，mid到right进行归并排序
        int C1 = MergeSort(nums, left, mid);
        int C2 = MergeSort(nums, mid + 1, right);
        int counts = C1 + C2; // 当前段的逆序数总计数

        int j = mid + 1; //右队列指针
        for(int i = left ; i <= mid ; i++) //左队列指针
        {
            while (j <= right && nums[i] > nums[j]) // 在排序过程中，对于每一个 i，通过比较 nums[i] 和 nums[j]，统计满足逆序条件的元素数量
                j++;
            counts += (j - mid - 1);// 将逆序数累加
        }

        std::sort(nums.begin() + left, nums.begin() + 1 + right);//在完成逆序数的统计后，对左半段和右半段进行排序。

        return counts;// 返回当前段的逆序数总计数
    }
}

void MainWindow::InitialRandom()
{

    int length = Width * Width - 1; //比总数少1
    int ReverseNumbers;//逆序数
    bool Valid = false;//是否合法

    Nums.clear();
    std::vector<int> temp;
    for (int i = 0; i < length; i++)
    {
        Nums.push_back(i + 1);
    }
    do
    {
        unsigned seed = std::chrono::system_clock::now ().time_since_epoch ().count (); //seed
        std::shuffle(Nums.begin(),Nums.end(),std::default_random_engine(seed));
        temp.clear();
        temp.assign(Nums.begin(),Nums.end());//复制一份
        ReverseNumbers = MergeSort(temp,0,length-1);
        if(ReverseNumbers % 2 == 0 && Width % 2 != 0) //判断是否有解
            Valid = true;
        else if(ReverseNumbers % 2 == 0 && Width % 2 == 0)
            Valid = true;
    }while(!Valid);
    Nums.push_back(0); //空格位添加0
}

char MainWindow::isAroundSpare(int row, int col)
{
    int dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; //方向参数
    char op[5] = "udlr";


    if(Blocks[row][col]->text()=="0")
        return 'n';

    for (int i = 0; i < 4; i++)
    {
        int ifCouldMoveX = row + dir[i][0], ifCouldMoveY = col + dir[i][1]; //四个方向探查
        if (ifCouldMoveX < 0 || ifCouldMoveY < 0 || ifCouldMoveX >= Width || ifCouldMoveY >= Width) //越界跳过
            continue;
        if(Blocks[ifCouldMoveX][ifCouldMoveY]->text()=="0")
            return op[i];
    }

    return 'n';
}

void MainWindow::SwepBlocks(int o_row, int o_col, int t_row, int t_col)
{
    QString num = Blocks[t_row][t_col]->text(); //临时变量用于交换
    bool vis =Blocks[t_row][t_col]->isVisible();

    Blocks[t_row][t_col]->setText(Blocks[o_row][o_col]->text());
    Blocks[t_row][t_col]->setVisible(Blocks[o_row][o_col]->isVisible());

    Blocks[o_row][o_col]->setText(num);
    Blocks[o_row][o_col]->setVisible(vis);
    ui->LabCount->setText(QString::number(ui->LabCount->text().toInt()+1)); //计数+1
    isVictory();

}

bool MainWindow::isVictory()
{
    for(int i = 0;i < Width; i++)
        for(int j = 0;j < Width; j++)
            if(!(i == Width - 1 && j == Width - 1) && Blocks[i][j]->text() != QString::number(i * Width + j + 1))
                return false;
    if(ui->LabCount->text().toInt()<MinCount)
    {
        MinCount = ui->LabCount->text().toInt();
        ui->LabMin->setText(QString::number(MinCount));
    }
    QMessageBox::information(NULL,"通关","恭喜获得胜利！\n点击\"重置当前\"挑战更短步骤！");
    return true;
}

int MainWindow::ManHattan(string s)
{
    int res = 0;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '0')
            continue;
        int TruePos = s[i] - 1;//期望位置
        res += abs(i / Width - TruePos / Width) + abs(i % Width - TruePos % Width);
    }
    return res;
}

void MainWindow::bfs(string st)
{
    unordered_map<string, int> dist; //无向图，记录不同布局的距离
    unordered_map<string, pair<string, char>> pre;//记录每个布局的前一步操作和布局状态的对应关系
    typedef pair<int, string> PIS; //键值对类型,用于优先队列中存储布局及其对应的距离
    string ed = "";//目标布局字符串
    for (int i = 1; i < Width * Width; i++)
        ed += static_cast<char>(i+48); //转字符
    //qDebug() << ed;
    ed += '0';
    int dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; //方向参数
    char op[5] = "udlr";  //方向标识

    priority_queue<PIS, vector<PIS>, greater<PIS>> q; //升序优先队列
    /*<PIS> 表示队列中的元素类型是 pair<int, string>。
    vector<PIS> 表示底层容器使用的是 vector。
    greater<PIS> 表示使用 greater 函数对象来确定元素的顺序，*/
    q.push({ManHattan(st), st}); //第一个元素是当前状态 st 的曼哈顿距离，第二个元素是当前状态 st 的字符串表示。
    dist[st] = 0; //初始布局键值为0
    while (q.size())  //当优先队列不为空时
    {
        auto top = q.top(); //取队列顶
        q.pop();
        string state = top.second; //获取布局
        if (state == ed) //当与期望结束布局一致时退出
            break;
        int step = dist[state]; //获取当前布局的距离
        int x, y;
        for (int i = 0; i < state.size(); i++) //获取空格所在位置
            if (state[i] == '0')
            {
                x = i / Width, y = i % Width;
                break;
            }
        string New_state = state; //复制当前布局
        for (int i = 0; i < 4; i++)
        {
            int ifCouldMoveX = x + dir[i][0], ifCouldMoveY = y + dir[i][1]; //四个方向探查
            if (ifCouldMoveX < 0 || ifCouldMoveY < 0 || ifCouldMoveX >= Width || ifCouldMoveY >= Width) //越界跳过
                continue;
            swap(New_state[x * Width + y], New_state[ifCouldMoveX * Width + ifCouldMoveY]);       //交换空格与探查对象
            if (!dist.count(New_state) || dist[New_state] > step + 1) //交换后布局不存在或者存在步数更多的相同布局
            {//检查当前变换后的布局 New_state 是否之前已经出现过，或者是新的布局但是通过当前路径到达的步数更短。
                dist[New_state] = step + 1;                 //添加、更新布局
                pair<string, int> o = {state, op[i]}; //布局与操作字符
                q.push({step + 1 + ManHattan(New_state), New_state});     //当前步数+曼哈顿距离，当前布局，入队
                pre[New_state] = o;                         //记录步骤
            }
            swap(New_state[x * Width + y], New_state[ifCouldMoveX * Width + ifCouldMoveY]); //换回原布局，准备探查下一个方向
        }
    }

    while (ed != st)
    {
        auto o = pre[ed]; //依次取出到达结果的路径
        path.push(o.second);
        ed = o.first; //寻找上一步
    }
}



void MainWindow::on_backBtn2_clicked()
{
    QTimer::singleShot(500, this, [=]() {
        emit this->chooseSceneBack();

    });
}

