#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <memory.h>
#include <stdio.h>
#include <string.h>
using namespace std;
#define MIN -2147483625
#define MAX 2147483625
#define MAXDEPTH 6
#define MaxSpace 8
#define MOSTNUM 62

int currBotColor; // 我所执子颜色（1为黑，-1为白，棋盘状态亦同）
int gridInfo[MaxSpace][MaxSpace] = { 0 }; // 先x后y，记录棋盘状态
int blackPieceCount = 2, whitePieceCount = 2;
int resultX = 0;
int resultY = 0;
int MyPosCount = 0, OpPosCount = 0;

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

// 向Direction方向改动坐标，并返回是否越界
inline bool MoveStep(int &x, int &y, int Direction) {
	if (Direction == 0 || Direction == 6 || Direction == 7)
		x++;
	if (Direction == 0 || Direction == 1 || Direction == 2)
		y++;
	if (Direction == 2 || Direction == 3 || Direction == 4)
		x--;
	if (Direction == 4 || Direction == 5 || Direction == 6)
		y--;
	if (x < 0 || x > 7 || y < 0 || y > 7)
		return false;
	return true;
}

// 在坐标处落子，检查是否合法或模拟落子
bool ProcStep(int xPos, int yPos, int color, bool checkOnly = false) {
	int effectivePoints[8][2];
	int dir, x, y, currCount;
	bool isValidMove = false;
	if (gridInfo[xPos][yPos] != 0)
		return false;
	for (dir = 0; dir < 8; dir++) {
		x = xPos;
		y = yPos;
		currCount = 0;
		while (1) {
			if (!MoveStep(x, y, dir)) {
				currCount = 0;
				break;
			}
			if (gridInfo[x][y] == -color) {
				currCount++;
				effectivePoints[currCount][0] = x;
				effectivePoints[currCount][1] = y;
			}
			else if (gridInfo[x][y] == 0) {
				currCount = 0;
				break;
			}
			else {
				break;
			}
		}
		if (currCount != 0) {
			isValidMove = true;
			if (checkOnly)
				return true;
			if (color == 1) {
				blackPieceCount += currCount;
				whitePieceCount -= currCount;
			}
			else {
				whitePieceCount += currCount;
				blackPieceCount -= currCount;
			}
			while (currCount > 0) {
				x = effectivePoints[currCount][0];
				y = effectivePoints[currCount][1];
				gridInfo[x][y] *= -1;
				currCount--;
			}
		}
	}
	if (isValidMove) {
		gridInfo[xPos][yPos] = color;
		if (color == 1)
			blackPieceCount++;
		else
			whitePieceCount++;
		return true;
	}
	else
		return false;
}

// 检查color方有无合法棋步
bool CheckIfHasValidMove(int color) {
	int x, y;
	for (y = 0; y < 8; y++)
		for (x = 0; x < 8; x++)
			if (ProcStep(x, y, color, true))
				return true;
	return false;
}

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

// 使用α-β pruning对搜索树进行查找
double ABPruning(int depth, double alpha, double beta, int color) {
	//the array of possible excuting
	int possible[MOSTNUM][2];
	int posCount = 0; // the number of them

					  //find the legal position
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (ProcStep(x, y, color, true)) {
				possible[posCount][0] = x;
				possible[posCount++][1] = y;
			}
		}
	}

	//get the border
	if (depth == MAXDEPTH) {
		return Estimation();  //first one
	}
	if (posCount == 0) {
		bool flag = false;
		if (depth % 2 == 1)
			flag = true; //it is op
		if (depth == 0) {
			resultX = resultY = -1;
			return 0;
		} // depth is not 0
		else
			return Estimation(flag);
	}                                    //second one

										 //save the grid
	int tmp[MaxSpace][MaxSpace];
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			tmp[i][j] = gridInfo[i][j];

	//have a try
	for (int i = 0; i < posCount; i++) {  //emunate the possible movement
		int x = possible[i][0];
		int y = possible[i][1];
		ProcStep(x, y, color);  //move
		double evalue = -ABPruning(depth + 1, -beta, -alpha, -color);

		//backTrace
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				gridInfo[i][j] = tmp[i][j];

		if (evalue >= beta) {
			return beta;
		}
		if (evalue > alpha) {
			alpha = evalue;
			if (depth == 0) {
				resultX = x;
				resultY = y;
			}
		}
	}
	return alpha;
}


int main()
{
	int x, y;

	// 初始化棋盘
	gridInfo[3][4] = gridInfo[4][3] = 1;  //|白|黑|
	gridInfo[3][3] = gridInfo[4][4] = -1; //|黑|白|

										  //input the number of the round
	int turnID;

	cin >> turnID;
	currBotColor = -1; // 第一回合收到坐标是-1, -1，说明我是黑方
	for (int i = 0; i < turnID - 1; i++) {
		// 分析自己收到的输入和自己过往的输出，并恢复状态
		cin >> x >> y;
		// 根据这些输入输出逐渐恢复状态到当前回合
		if (x >= 0)
			ProcStep(x, y, -currBotColor); // 模拟对方落子
		else if (i == 0 && x < 0)
			currBotColor = 1;
		cin >> x >> y;
		if (x >= 0)
			ProcStep(x, y, currBotColor); // 模拟己方落子
	}

	// 看看本回合对方输入
	cin >> x >> y;
	if (x >= 0)
		ProcStep(x, y, -currBotColor); // 模拟对方落子
	else if (turnID == 1 && x < 0)
		currBotColor = 1;

	// 做出决策（你只需修改以下部分）

	ABPruning(0, MIN, MAX, currBotColor);

	// 决策结束，输出结果（你只需修改以上部分）
	cout << resultX << " " << resultY << endl;
	return 0;
}
