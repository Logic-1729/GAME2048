# Game2048Extension评分参考标准
>以下均为$\text{PhantomPhoenix}$实现的代码，新的解法欢迎交流，如果你觉得实现有误欢迎联系他本人，与笔者无关

## Solution 1:RANDOM WASD,N=1e6
> Wish you good luck.

AVERAGE SCORE: 1094.95
HIGHEST SCORE: 6068
LOWEST SCORE: 36
WIN RATE: 0%

## Solution 2:DOWNLEFT,N=1e6
> LOOP "LEFT DOWN" "RIGHT DOWN", OTHERWISE UP

AVERAGE SCORE: 1608.51
HIGHEST SCORE: 11112
LOWEST SCORE: 72
WIN RATE: 0%

## Solution 3:DOWNLEFT PRO
> RANDOM BY WEIGHT (RIGHT DOWN UP), OTHERWISE LEFT

AVERAGE SCORE: 2064.09
HIGHEST SCORE: 11996
LOWEST SCORE: 92
WIN RATE: 0%

## Solution 4:LOOP WASD N=1e6
AVERAGE SCORE: 2307.31
HIGHEST SCORE: 11924
LOWEST SCORE: 204
WIN RATE: 0%

## Solution 5:PRIORITY N=1e6
> DO THE FOUR OPERATIONS WITH A PRIORITY SEQUENCE

AVERAGE SCORE: 2543.8
HIGHEST SCORE: 12108
LOWEST SCORE: 96
WIN RATE: 0%

## Solution 6:PRIORITY 2 N=1e6
> PRIORITY UP, OTHERWISE PREDICT THE EMPTY CELL'S NUMBER IF WE DO LEFT AND RIGHT, CHOOSE THE DIERECTION WHICH WILL KEEP MORE EMPTY CELLS IN LEFT AND RIGHT

AVERAGE SCORE: 2548.42
HIGHEST SCORE: 12740
LOWEST SCORE: 116
WIN RATE: 0%

## Solution 7:PRIORITY 3 N=100000
<!--> FIX WITH THE PREDICTION IN PRIORITY 2, PREDICT THE EMPTY CELL'S NUMBER IF WE DO NOT ONLY LEFT AND RIGHT, BUT MORE OPERATIONS-->

> Evaluate not only the board after one next step, but the board after more subsequent steps as well. The following result is one possible implement, that is based on Solution 6, and takes evaluation of the number of empty cells after two steps into account. Also, maybe you can try some other ways of evaluation.

AVERAGE SCORE: 7392.05
HIGHEST SCORE: 24628
LOWEST SCORE: 360
WIN RATE: 0.085%

## Solution 8:ENDGAME ALGORIGTHM N=1e6
> 参考下发文件2048（微信终结版）实现的代码

AVERAGE SCORE: 10189.9
HIGHEST SCORE: 30108
LOWEST SCORE: 344
WIN RATE: 2.6952%

## Solution 9:ExpectiMax N=1e5
> 参考[https://zhuanlan.zhihu.com/p/701333456](https://zhuanlan.zhihu.com/p/701333456)
> 或者搜索启发式搜索会有差不多的东西

AVERAGE SCORE: 17042.9
HIGHEST SCORE: 22860
LOWEST SCORE: 2412
WIN RATE: 48.2%

## Extra

Reinforcement learning is not recommanded, because of the large amount of code and the high demand for computing power.
