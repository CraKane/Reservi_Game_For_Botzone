# Reservi_Game_For_Botzone
It is a game AI about the reservi for botzone. (https://www.botzone.org.cn/)


BOTZONE
  https://www.botzone.org.cn/
  It is a platform of chess game for every one created by PKU(Peking_University) in China.

ABOUT MY FILE
  There is a game called reservi in the platform.
  You can write an AI for this game.
  
ABOUT MY AI
  the way of interaction: 
    simple interaction.
    
    tips:
      There are TWO interaction method in reversi from botzone.
      One is the simple interaction.
      And another one is JSON interaction.
      
    As for me, my code is for the simple interaction.
    If you like or you want to have a try to JSON interaction, 
    you can download the sample code, learn and constructure your own JSON interaction CODEs.
    
  the algorithm used:
    Alpha-Beta pruning & NegaMax algorithm.
    
  the depth I search:
    I just search 6 depths for avoiding the overtime due to the limited time 1's in the platform.
    If you try without the limmited time, you can change it as you like.
    
  the estimation I using :
    double value[8][8] = {
          { 100,0.5,15,15,15,15,0.5,100 },
          { 0.5 ,-10,3 ,3 ,3 ,3 ,-10,0.5 },
          { 15,3,6 ,6 ,6 ,6 ,3,15 },
          { 15,3,6 ,0 ,0 ,6 ,3,15 },
          { 15,3,6 ,0 ,0 ,6 ,3,15 },
          { 15,3,6 ,6 ,6 ,6 ,3,15 },
          { 0.5,-10,3 ,3 ,3 ,3 ,-10,0.5 },
          { 100,0.5,15,15,15,15,0.5,100 }
    };
    // 估值函数，估计下在某一步局面的得分
    double Estimation(bool flag = false) {
      double score = 0;
      int OpCoCount = 0;
      int MyCoCount = 0;
      MyPosCount = 0, OpPosCount = 0;
      for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
          if ((x == 0 || x == 7) && (y == 0 || y == 7)) {
            if (gridInfo[x][y] == currBotColor)
              MyCoCount++;
            else if (gridInfo[x][y] == -currBotColor)
              OpCoCount++;
          }
          if (gridInfo[x][y] == currBotColor)
            score += value[x][y];
          else if (gridInfo[x][y] == -currBotColor)
            score -= value[x][y];
          if (ProcStep(x, y, currBotColor, true))
            MyPosCount++;
          else if (ProcStep(x, y, -currBotColor, true))
            OpPosCount++;
        }
      }
      int b; int c;
      if (flag) {
        b = OpPosCount - MyPosCount;
        c = OpCoCount - MyCoCount;
      }
      else {
        b = MyPosCount - OpPosCount;
        c = MyCoCount - OpCoCount;
      }
      if (MyCoCount >= 2)//占两个角以后 侧重score
        return score * 0.65 + b * 8 * 0.35 + c * 10;
      else//在那之前 侧重行动力
        return score * 0.4 + b * 8 * 0.6 + c * 20;
    }
    the most important factor I think: 
      the movement: how many locations you can move
      the FOUR corners are stable
      the location score:
        the table I wrote above is very opportunistic because I test again and again, it is the experience.
      the weight:
        It means the tendency or strategy of you in a game.
        I change the value of it by test & experience.
    
STATE IN THE RANKING LIST
  about 30 to 100.

FUTURE WORK
  the algorithm has no bug according to the test results, BUT the estimation can be betttter, 
  so I will try my best to improve my estimation function in the future.

THANKS
  Thank you for CSDN & WIKI & My friends which is good at the reversi, So I can create the powerful estimation function as a strong AI,
  NOT a zhizhang(silly bot) in Chinese.
  I will run further on!
