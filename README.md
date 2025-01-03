# LSK2023-DS
# HuaRongDao
# Bugs：

1、播放音乐只能实现暂停，无法实现再次启动  QMediaPlayer是否可行？ 是否是isPlaying未有正确激活？
2、4X4 5X5自动寻路会卡死，导致程序崩溃。
3、ComboBox无法实现圆角（框选处会有方形的背景露出），只能使用方形，较不美观
4.ComboBox无法实现down-arrow的icon改变
5、游戏砖块的icon无法实现自适应方块大小，也就是自适应显示
9、是否可以优化A*算法？如何优化？？
# Fixed：
0、如何实现窗口之间的切换？
6、连续点击Up/Down/Left/Right的键盘操作会导致重复移动
7、背景图片无法正确设置样式表
8、界面布局不美观

## 文本文件单词的检索与计数（搜索的应用）
【问题描述】

建立一个文本文件，每个单词不包含空格并且不跨行，单词由字符序列构成且区分大小写；统计给定单词在文本文件中出现的总次数；检索输出某个单词出现在文本中的行号、在该行中出现的次数以及位置。

【基本要求】

一个完整的文本文件单词的检索与计数软件应具有以下功能：

1）建立文本文件：文件名由用户用键盘输入；

2）给定单词的计数：输入一个不含空格的单词，统计输出该单词在文本中的出现的次数；

3）检索给定单词：输入一个单词，检索并输出该单词所在的行号，该行中出现的次数以及在该行中的相应位置。

 菜单选项包含：建立文件、单词定位、单词计数、退出程序，选择1-4执行相应的操作，其他字符为非法。
 
要求设计界面，合理展示整个软件的功能实现过程。

## 汽车华容道（图搜索的应用）
【问题描述】

华容道是古老的中国民间益智游戏，以其变化多端、百玩不厌的特点与魔方、独立钻石

棋一起被国外智力专家并称为世界”“智力游戏界的三个不可思议“。汽车华容道，是以经典游戏华容道进行改编，要通过移动其他阻挡汽车位置，将自己的小车成功地从车辆停放混乱的

停车场里开出来。停车场是一个填满汽车N*N方格，常见的有3x3、4x4、5x5和6x6模式。

如下图所示：

本题要求设计并实现一款PC版的汽车华容道游戏软件。


【基本要求】

一个完整的汽车华容道软件应具有以下功能：

（1）每次生成随机但“有解”的汽车华容道布局，每个汽车大小可以简化为相同（如2x1

垂直条形块Vertical），但摆放位置可以是1x2水平Horizontal或者2x1垂直Vertical。采用可视化形式在图形界面中展示场景；

（2）支持“手动”移动汽车模式，除了鼠标操作，还支持通过操作键盘上的四个方向键（也可自定义键）来控制汽车的移动，注意：汽车移动过程中不允许重叠，也不允许跨越移动。

（3）支持“自动”移动汽车模式，即“华容道自动求解”，基于图搜索算法实现求解；

（4）界面美观，操作方便，背景音乐播放；

（5）绘制出汽车移动路径；或者使用动画模拟出汽车移动过程。

【高阶要求】

（1）软件设置若干默认个关卡，每个关卡可以设置移动步数和时间限制；

（2）每个汽车大小可以不同。

【实现提示-1】

完全随机位置的数值华容道仅有50%的概率是有解的。这里介绍相关规则： 逆序数，即一个数字序列，将其中所有数字依次两两对比，若大数在前，小数在后，那么这就是一对逆序数。这里说到的逆序数，指的是数字序列中逆序数的数量。如1、2、3、
4、5、6、8、7，逆序数只有1个，即8和7。
同时，初始状态的数字华容道，会有一个空格，一般会设置在最末行的右下角。但也可
以根据实际的需求，设置在其他行。请留意，初始空格所在的行数，是决定是否有解的一个
重要因素。
数字华容道，必然有解，只存在于如下3个细分情形：

（1）若格子列数为奇数，则逆序数必须为偶数；

（2）若格子列数为偶数，且逆序数为偶数，则当前空格所在行数与初始空格所在行数
的差为偶数；

（3）若格子列数为偶数，且逆序数为奇数，则当前空格所在行数与初始空格所在行数
的差为奇数。

【实现提示-2】

A*算法常用于二维地图路径规划，算法所采用的启发式搜索可以利用实际问题所具备
的启发式信息来指导搜索，从而减少搜索范围，控制搜索规模，降低实际问题的复杂度。可
以实现汽车华容道的自动求解过程。

 
实现原理：状态空间搜索，就是将问题求解过程表现为从初始状态到目标状态寻找这个路径的过程。由于求解问题的过程中分枝有很多，主要是求解过程中求解条件的不确定性，
不完备性造成的，使得求解的路径很多这就构成了一个图，我们说这个图就是状态空间。问题的求解实际上就是在这个图中找到一条路径可以从开始到结果。这个寻找的过程就是状态
空间搜索。常用的状态空间搜索有深度优先和广度优先，它们的缺陷在于都是在一个给定的
状态空间中穷举。因此，需要使用启发式搜索。

启发式搜索就是在状态空间中的搜索对每一个搜索的位置进行评估，得到最好的位置，
再从这个位置进行搜索直到目标，这样可以省略大量无谓的搜索路径以提到效率。在启发式
搜索中，对位置的估价是十分重要的。启发中的估价是用估价函数表示的，如：f(n) = d(n) + h(n)，其中f(n)是节点n的估价函数，d(n)实在状态空间中从初始节点到n节点的实际代价，
h(n)是从n到目标节点最佳路径的估计代价。

在华容道游戏中，可以采用如下的评价函数，f(n) = d(n) + h(n)，其中d(n)为当前状态从初始状态开始移动的步数，h(n)计算当前状态与目标状态相比错位的个数。搜索过程总是往f(n)最小的分枝方向进行，以便快速达到最终状态。

【参考网址】

[1] https://www.jianshu.com/p/1c1849d876b2 

[2] https://www.taptap.com/app/87488可以参考的APP 

[3] https://blog.csdn.net/qq_36946274/article/details/81982691

