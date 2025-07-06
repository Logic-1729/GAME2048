# Project 1: $\mathrm{Game\ 2048}$

> SJTU CS1958-01 2025Fall 第一次大作业

## 内容简介

在本次作业中，你需要实现一个2048小游戏（如果你不知道2048是什么，你可以参考 [2048](https://2048game.com/)）。

## 项目框架

目前已经实现了这个项目的大致结构，你只需要完成 `Game2048_Logic.cpp`，其中可能用到的常量、变量与函数可以参见 `Game2048_Logic.h`。为了方便各位熟悉与调试，仓库根目录里的 `example.zip` 里有一个实现所有功能后的 2048.exe 来帮助各位熟悉键位与操作,以便你在本地测试自己的 2048.exe 是否正确。

### 代码实现

```
Game2048/
├── CMakeLists.txt                # CMake 构建配置文件
├── README.md                     # 项目说明
├── bin/                          # 可执行文件和运行时数据
├── build/                        # 构建输出目录
├── cmake/                        # CMake 工具链
├── cmake-build-debug/            # CLion等IDE的
├── include/                      # 头文件目录
│   ├── ControlStyle.h
│   ├── Game2048_Data.h
│   ├── Game2048_Logic.h
│   ├── Game2048_Messages.h
│   ├── Game2048_UI.h
│   └── Game2048.h
├── src/                          # 源代码目录
│   ├── Game2048_Data.cpp
│   ├── Game2048_Logic.cpp //TODO
│   ├── Game2048_Messages.cpp
│   ├── Game2048_UI.cpp
│   ├── Game2048.cpp
│   └── main.cpp
└── test.sh                       # 性能测试脚本
```

我们已经实现这个2048小游戏的 GUI 前端，你所需要完成的 `Game2048_Logic.cpp` 只包括这个2048游戏的主要运行逻辑，并完善 HINT 的功能。

## 基础任务

通行版2048的规则主要有以下几条：

> 1. 游戏在 $4 \times 4$ 的方格棋盘上进行。  
> 2. 游戏两种操作，一种是移动操作，另一种是放置操作。  
> 3. 移动操作的规则如下:  
> 3.1. 可以向上/下/左/右四个方向的其中之一移动。选定好方向后，所有数字都会向该方向靠拢，相同的两个数字相撞时会合并成一个新数字，这个数字是它们的和。  
> 3.2. 在一次移动中，每个数字最多只能合并一次，优先合并靠近移动方向棋盘边缘的数字。  
> 3.3. 每次合并后，将会获得一定的分数，获得的分数等于所有合并后数字相加之和。若无任何合并发生，则不得分。  
> 3.4. 若移动前后棋盘上的所有数字大小和位置均没有变化，则不算做有效移动，否则即是有效移动。  
> 4. 放置操作的规则如下: 只能在棋盘上没有数字的位置放置一个数字，10%的几率放置一个 4，90%的几率放置一个 2，且放置在每个空位的几率均等。  
> 5. 游戏的运行逻辑是：  
> 5.1. 开始时棋盘为空，分数为 0。  
> 5.2. 先进行两步放置操作，然后轮流进行移动操作和放置操作，中间的每一步都必须是有效的。  
> 5.3. 若不能够进行有效移动，则游戏结束，此时的得分为最终得分。  
> 5.4. 若某一时刻，棋盘上出现了2048，则游戏提前结束，并统计得分。

为此，你需要实现 `Game2048_Logic.cpp` 中的以下函数：

```
Game2048Logic::Game2048Logic() {}

bool Game2048Logic::ProcessMove(int direction) {}

void Game2048Logic::AddNewNumber() {}

bool Game2048Logic::IsGameOver() const {}

bool Game2048Logic::HasReached2048() const  {}

void Game2048Logic::NewGame()  {}

bool Game2048Logic::IsAllZero(const std::array<std::array<unsigned int, 4>, 4>& board) const {}

void Game2048Logic::CopyBoard(std::array<std::array<unsigned int, 4>, 4>& dest, 
                        const std::array<std::array<unsigned int, 4>, 4>& src) const {}
```

除此之外，我们需要你支持撤销操作，为此你需要实现 `Game2048_Logic.cpp` 中的以下函数：

``` 
void Game2048Logic::Redo() {}

void Game2048Logic::SaveRecord() {}
```

在实现撤销操作的过程中，请注意一些显而易见的细节，比如新建的游戏不可执行撤销操作，但是在死局之后执行的新建操作可以被执行；再比如撤销操作不可被无限次执行，这些细节详见 `Game2048_Logic.h`。

除此之外，我们还提供了 `Game2048Logic::ChooseDirection()` 与 `Game2048Logic::Hint()`，在基础任务部分，你无需对他们进行修改。

## 拓展任务

在这一板块中，你需要优化 `Game2048Logic::Hint()`，这一函数的效果是仿照人工~~智障~~智能来帮助人更好地进行决策。但现在给出的 `Game2048Logic::Hint()` 函数实现的效果是随机朝四个方向移动，这样做的得分效率显然比较低下，也几乎不可能通关。

为此，请你优化 `Game2048Logic::Hint()` 函数，获取更高的平均得分。为了保证 test.sh 能够正常运行，请保证在 Hint() 函数的功能不变的前提下添加任意辅助函数或者头文件。注意你无需给出必胜决策/最优决策，你只要做一些更 optimal 的调整即可，比如：

```
1. 按照下 > 左 > 右 > 上的优先级对棋盘进行操作
2. 优先上；否则若左右均可移动，则预测移动后的空单元格数量，选择保留更多空格的方向；若仅左或右可行，则直接选择可行方向；若左右均不可行，则最后尝试向下移动
3. Expectimax
```

这些做法完全不唯一，在此我们提供我们实现过的一些方法供各位参考 Average Score。[Here](https://notes.sjtu.edu.cn/AT7a_9C7SF2fpmOflkq3DQ?both)。当然我们更希望大家能够自行查阅互联网对自己的函数进行改进，请在修改完你的函数后，在 wsl 中运行 test.sh 来得到你的 Average Score，请你修改你的函数来获取更高的得分！

**WARNING:** 在此期间，不允许使用撤销操作，一旦被发现，本部分分数将作零分处理。

## 须知

### 截止时间

第四周周日（10/13）24:00

### 如何完成程序

你有两种方式下载该仓库完成作业：

1. 在 GitHub 页面点击 Code - Download ZIP，仓库代码将会被下载到本地，随后你可以直接在本地仓库上进行编辑。
2. 若你掌握 Git 的使用，你可以 clone 该仓库进行操作，或者以该仓库为 Template 创建自己的仓库，在自己的仓库中进行操作（我们推荐使用最后一种操作方式）。

### 编译

本项目将在 wsl 中运行，在开始前，请打开 wsl，依次执行以下命令来安装需要的库与插件:

```
wsl --version        # 查看wsl是否安装
sudo apt update      # 更新wsl
sudo apt install -y mingw-w64     # 安装Windows API有关的库
sudo apt install wine             # Windows系统下可跳过
```

对于 Basic 部分而言，请你在完成 `Game2048_Logic.cpp` 之后打开 wsl 的根目录，依次执行以下命令：

```
cd Game2048         # 如果当前目录已在Game2048下可跳过
rm -rf build/
mkdir build
cd build
cmake ..
make -j$(nproc)
cd ..
cd bin
wine 2048.exe       # Windows系统可手动打开bin/2048.exe
```

然后你就能看到你的 2048.exe 运行；

对于 Extension 部分而言，请你在完成 `Game2048_Logic.cpp` 中的 Hint 之后，打开 wsl 的根目录，依次执行以下命令：（注意在这一阶段无需执行 Basic 部分的编译，脚本中包含了编译错误的检查）

```
cd Game2048         # 如果当前目录已在Game2048下可跳过
chmod +x test.sh    # 第一次运行授予权限即可，之后无需运行
./test.sh
```

### 提交方式

请在 OnlineJudge 上直接提交你的 github 仓库地址。

### 评分规则

本项目 Basic 占 50%，Extension 占 30%，Code Review 占 20%。

对于 Basic 部分，只要你正确完成了 2048 的游戏逻辑，包括移动规则、计分规则、撤销规则，你就能完全获得这部分的分数，折合总分 50pts。

对于 Extension 部分，其最终得分取决于你的 Average Score，对此，我们制定了以下公式用来换算你在本部分获得的分数:

$$
\begin{cases}
0, & \text{score} < 1000 \\\\
\left\lceil \dfrac{score^2}{2 \times 10^5} \right\rceil + 30, & 1000 \leq score < 3000 \quad (35\text{-}75\text{ pts}) \\\\
\left\lceil 12.27 \log_2\left(\dfrac{score}{1000}\right) \right\rceil + 55.55, & 3000 \leq score < 8000 \quad (75\text{-}93\text{ pts}) \\\\
\min \left( \left\lceil 42.92 \sqrt{ \log_2 \left( \dfrac{score}{1000} \right) } \right\rceil + 18.16, 110 \right), & score \geq 8000 \quad (93\text{-}110\text{ pts})
\end{cases}
$$

其中如果你在这部分获得了超过100pts的分数，将完全获得这部分的分数，溢出部分将合并到本次大作业的总分；否则你的分数将会乘以0.3折合到本次大作业。

对于 Code Review 部分，我们将会考察包括但不限于：

* Game2048_Logic.cpp 中函数的具体实现细节
* 这个项目的框架结构布置
* git 与 linux 命令行的实践操作

理论上的总分是110pts，但是你的得分上限为 100pts，你在 Extension 部分溢出的分数直接算入大作业总分。

## Acknowledgement
感谢 2024级黄佳瑞，2024级刘宇轩以及2024级ACM王越天，他们对这个项目的IDEA、代码框架、评分标准、平台的可移植性、压力测试上提供了许多宝贵的建议，特此感谢。
    
鄙人才疏学浅，对于各种细节懒得深究，如果对于文档中出现的问题或者建议，劳驾各位高抬贵手发送至`logic_1729@sjtu.edu.cn`,还请各位多多指教QAQ
    
$\phantom{aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa}$                                            ——2025.07.06 $\mathrm{by\ PhantomPhoenix}$
