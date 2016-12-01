// BestSolution.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//定义目标函数
double tarFun(cd b0, cd b1, cd b2, cd b3, cd b4, cd b5, cd b6, cd b7, cd b8, cd b9, cd testN, cd testP, cd testK){
	return -(b0 + b1 * testN + b2 * testN * testN + b3 * testP + b4 * testP * testP
		+ b5 * testK + b6 * testK * testK + b7 * testN * testP + b8 * testN * testK + b9 * testP *testK);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//定义b0-b9的值，在最终程序中不会出现
	double _b0 = 324.858043532649;
	double _b1 = -4.98660540860191;
	double _b2 = 0.201985700184229;
	double _b3 = 19.67594900721;
	double _b4 = 0.585584785179794;
	double _b5 = -18.1714619297807;
	double _b6 = -0.253909876852084;
	double _b7 = -0.798886164623468;
	double _b8 = 0.822333598683863;
	double _b9 = -1.1188438677493;
	const double clsflN = 25;
	const double clsflP = 8;
	const double clsflK = 6;

	//为防止模型过早收敛，使用相同的方法迭代3次
	double lastResult = -99999;
	vector<double> lastSolution;
	int totalIteration = 1;
	while (totalIteration != 5){
		srand((unsigned)time(NULL) * totalIteration);
		//初始顶点
		vector<vector<double> > solution;
		vector<double> temp;
		temp.push_back(clsflN);
		temp.push_back(clsflP);
		temp.push_back(clsflK);
		solution.push_back(temp);

		for (int i = 0; i != 5; ++i){
			//产生其他顶点
			double sN = 0.5 * clsflN + clsflN * rand() / double(RAND_MAX);
			double sP = 0.5 * clsflP + clsflP * rand() / double(RAND_MAX);
			double sK = 0.5 * clsflK + clsflK * rand() / double(RAND_MAX);
			auto vt = temp.begin();
			temp[0] = sN;
			temp[1] = sP;
			temp[2] = sK;
			solution.push_back(temp);
		}
		//定义每次迭代的函数值，迭代结束后该值为最优值
		double finalReasult;
		//定义每次迭代产生的解集的均值，迭代结束后为最优解
		vector<double> meanSolution(3, 0);
		//定义每次迭代的平均距离
		double meanDistance = 999999;
		//定义最小顶点距离
		const double setDistance = 0.00001;
		//定义最大迭代次数
		const int setItrations = 3000;
		//定义当前迭代次数
		int currentItrations = 0;

		//开始迭代
		while (meanDistance > setDistance && currentItrations < setItrations){

			//最坏点与第二最坏点的值
			double firstNegativePoint = -999999;
			double sceondNegativePoint = -999999;
			//指向最坏点与第二最坏点的迭代器
			auto firstPointer = solution.begin();
			auto sceondPointer = solution.begin();

			double funTemp;
			//搜索最坏点
			for (auto svt = solution.begin(); svt != solution.end(); ++svt){
				funTemp = tarFun(_b0, _b1, _b2, _b3, _b4, _b5, _b6, _b7, _b8, _b9, (*svt)[0], (*svt)[1], (*svt)[2]);
				if (firstNegativePoint < funTemp){
					firstNegativePoint = funTemp;
					firstPointer = svt;
				}
			}
			//搜索第二最坏点
			for (auto svt = solution.begin(); svt != solution.end(); ++svt){
				if (svt == firstPointer) continue;
				funTemp = tarFun(_b0, _b1, _b2, _b3, _b4, _b5, _b6, _b7, _b8, _b9, (*svt)[0], (*svt)[1], (*svt)[2]);
				if (sceondNegativePoint < funTemp){
					sceondNegativePoint = funTemp;
					sceondPointer = svt;
				}
			}

			//求出最坏点的对称点
			//首先要求去除最坏点的均值顶点

			//定义均值顶点
			vector<double> meanPoint;

			double totalN = 0;
			double totalP = 0;
			double totalK = 0;

			for (auto svt = solution.begin(); svt != solution.end(); ++svt){
				if (firstPointer == svt) continue;
				totalN += (*svt)[0];
				totalP += (*svt)[1];
				totalK += (*svt)[2];
			}
			meanPoint.push_back(totalN / 5);
			meanPoint.push_back(totalP / 5);
			meanPoint.push_back(totalK / 5);

			//定义反射系数
			double alpha = 1.3;

			//定义对称点
			vector<double> reversePoint;

			int flNumber = 0;
			for (auto mpPointer = meanPoint.begin(); mpPointer != meanPoint.end(); ++mpPointer){
				reversePoint.push_back((1 + alpha) * (*mpPointer) - alpha * (*firstPointer)[flNumber]);
				++flNumber;
			}


			//验证对称点
			double rpComput = tarFun(_b0, _b1, _b2, _b3, _b4, _b5, _b6, _b7, _b8, _b9, reversePoint[0], reversePoint[1], reversePoint[2]);
			//额外的跳出条件
			int endFristWhile = 0;
			while (rpComput > sceondNegativePoint && endFristWhile <= 1000){
				reversePoint[0] = (reversePoint[0] + meanPoint[0]) / 2;
				reversePoint[1] = (reversePoint[1] + meanPoint[1]) / 2;
				reversePoint[2] = (reversePoint[2] + meanPoint[2]) / 2;
				rpComput = tarFun(_b0, _b1, _b2, _b3, _b4, _b5, _b6, _b7, _b8, _b9, reversePoint[0], reversePoint[1], reversePoint[2]);
				++endFristWhile;
			}

			//定义常数约束条件修改参数
			double theta = 0.000001;

			//验证约束条件
			bool checkN = reversePoint[0] > 1.5 * clsflN || reversePoint[0] < 0.5 * clsflN;
			bool checkP = reversePoint[1] > 1.5 * clsflP || reversePoint[1] < 0.5 * clsflP;
			bool checkK = reversePoint[2] > 1.5 * clsflK || reversePoint[2] < 0.5 * clsflK;
			while (checkN || checkP || checkK){
				if (checkN){
					if (reversePoint[0] > 1.5 * clsflN) reversePoint[0] = 1.5 *clsflN - theta;
					if (reversePoint[0] < 0.5 * clsflN) reversePoint[0] = 0.5 *clsflN + theta;
				}
				if (checkP){
					if (reversePoint[1] > 1.5 * clsflP) reversePoint[1] = 1.5 *clsflP - theta;
					if (reversePoint[1] < 0.5 * clsflP) reversePoint[1] = 0.5 *clsflP + theta;
				}
				if (checkK){
					if (reversePoint[2] > 1.5 * clsflK) reversePoint[2] = 1.5 *clsflK - theta;
					if (reversePoint[2] < 0.5 * clsflK) reversePoint[2] = 0.5 *clsflK + theta;
				}
				rpComput = tarFun(_b0, _b1, _b2, _b3, _b4, _b5, _b6, _b7, _b8, _b9, reversePoint[0], reversePoint[1], reversePoint[2]);
				//额外的跳出条件
				int endSecondWhile = 0;
				while (rpComput > sceondNegativePoint && endSecondWhile <= 1000){
					reversePoint[0] = (reversePoint[0] + meanPoint[0]) / 2;
					reversePoint[1] = (reversePoint[1] + meanPoint[1]) / 2;
					reversePoint[2] = (reversePoint[2] + meanPoint[2]) / 2;
					rpComput = tarFun(_b0, _b1, _b2, _b3, _b4, _b5, _b6, _b7, _b8, _b9, reversePoint[0], reversePoint[1], reversePoint[2]);
					++endSecondWhile;
				}
				checkN = reversePoint[0] > 1.5 * clsflN || reversePoint[0] < 0.5 * clsflN;
				checkP = reversePoint[1] > 1.5 * clsflP || reversePoint[1] < 0.5 * clsflP;
				checkK = reversePoint[2] > 1.5 * clsflK || reversePoint[2] < 0.5 * clsflK;
			}

			//使用对称点代替最坏点
			*firstPointer = reversePoint;

			//计算顶点距离,这里按平均距离计算
			double totalDistance = 0;
			double squares;
			int times = 0;
			for (auto svti = solution.begin(); svti != solution.end() - 1; ++svti){
				for (auto svtj = svti + 1; svtj != solution.end(); ++svtj){
					squares = ((*svti)[0] - (*svtj)[0]) * ((*svti)[0] - (*svtj)[0]);
					squares += ((*svti)[1] - (*svtj)[1]) * ((*svti)[1] - (*svtj)[1]);
					squares += ((*svti)[2] - (*svtj)[2]) * ((*svti)[2] - (*svtj)[2]);
					totalDistance += sqrt(squares);
					times += 1;
				}
			}
			meanDistance = totalDistance / times;
			double solutionN = 0;
			double solutionP = 0;
			double solutionK = 0;
			for (auto svt = solution.begin(); svt != solution.end(); ++svt){
				solutionN += (*svt)[0];
				solutionP += (*svt)[1];
				solutionK += (*svt)[2];
			}
			meanSolution[0] = solutionN / 6;
			meanSolution[1] = solutionP / 6;
			meanSolution[2] = solutionK / 6;
			finalReasult = -tarFun(_b0, _b1, _b2, _b3, _b4, _b5, _b6, _b7, _b8, _b9, meanSolution[0], meanSolution[1], meanSolution[2]);
			++currentItrations;
		}
		if (lastResult < finalReasult){
			lastResult = finalReasult;
			lastSolution = meanSolution;
		}
		++totalIteration;
	}
	cout << lastResult << endl;
	system("PAUSE");
	return 0;

}


