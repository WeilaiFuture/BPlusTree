#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "BPlusTree.h"

class RandNum{
private:
	int* num = NULL;
	int index = 0;
public:
	RandNum(int numCout){
		num = (int *)malloc(sizeof(int)*numCout);
		srand(1);
		for (int i = 0; i < numCout; i++){
			num[i] = i + 1;
		}
		for (int i = 0; i < numCout; i++){
			int temp = 0;
			int randNum = rand() % numCout;
			temp = num[i];
			num[i] = num[randNum];
			num[randNum] = temp;
		}
	}
	RandNum(){
		num = (int *)malloc(sizeof(int) * 1000);
		srand(1);
		for (int i = 0; i < 1000; i++){
			num[i] = i + 1;
		}
		for (int i = 0; i < 1000; i++){
			int temp = 0;
			int randNum = rand() % 1000;
			temp = num[i];
			num[i] = num[randNum];
			num[randNum] = temp;
		}
	}
	~RandNum(){
		delete num;
	}
	int getRandNum(){
		return num[index++];
	}

};



// 随机建立一棵树
void intTest1(BPlusTree<int>* pTree, int count)
{
	pTree->ClearTree();
	RandNum randNum = RandNum(1000);
	for (int i = 0; i < count; i++)
	{
		int x = randNum.getRandNum();
		(void)pTree->Insert(x, x);
	}
	printf("successed!\n");
}

// 在树中查找某数据
void intTest2(BPlusTree<int>* pTree, int data)
{
	char sPath[255] = { 0, };

	(void)pTree->Search(data, sPath);
	int a = 0;
	pTree->KeyToValue(data, a);
	printf("\n%s\n%d\n", sPath, a);
}

// 在树中插入某数据
void intTest3(BPlusTree<int>* pTree, int data, int offset)
{
	bool success = pTree->Insert(data, offset);
	if (true == success)
	{
		printf("\nsuccessed!\n");
	}
	else
	{
		printf("\nfailed!\n");
	}
}


// 在树中删除某数据
void intTest4(BPlusTree<int>* pTree, int data)
{
	bool success = pTree->Delete(data);
	if (true == success)
	{
		printf("\nsuccessed!\n");
	}
	else
	{
		printf("\nfailed!\n");
	}
}
// 打印
void intTest5(BPlusTree<int>* pTree)
{
	RandNum * insertRandNum = new RandNum(80);
	RandNum * deleteRandNum = new RandNum(80);
	for (int i = 0; i < 80; i++){
		int a = insertRandNum->getRandNum();
		pTree->Insert(a, a);
	}
	for (int i = 0; i < 80; i++){
		int a = deleteRandNum->getRandNum();
		pTree->Delete(a);
		if (!pTree->CheckTree())
			printf("wrong");
	}
}

// 打印
void intTest6(BPlusTree<int>* pTree)
{
	pTree->PrintTree();
}

// 对树进行检查
void intTest7(BPlusTree<int>* pTree)
{
	bool success = pTree->CheckTree();
	if (true == success)
	{
		printf("\nsuccessed!\n");
	}
	else
	{
		printf("\nfailed!\n");
	}
}
int main(int argc, char* argv[])
{
	BPlusTree<int>* pTree = new BPlusTree<int>();

	int x = 1;
	int y = 0;
	int z = 0;
	while (0 != x)
	{
		printf("\n\n");
		printf("    *******************************************************************\n");
		printf("    *           欢迎进入B+树演示程序，请选择相应功能。                *\n");
		printf("    *           1。随机建立一棵B+树；                                 *\n");
		printf("    *           2。在B+树中查找一个数；                               *\n");
		printf("    *           3。在B+树中插入一个数；                               *\n");
		printf("    *           4。在B+树中删除一个数；                               *\n");
		printf("    *           5。删除测试；                                         *\n");
		printf("    *           6。显示整个B+树；                                     *\n");
		printf("    *           7。检查整个B+树；                                     *\n");
		printf("    *           0。退出程序；                                         *\n");
		printf("    *******************************************************************\n");
		printf("\n    您的选择是：");


		scanf("%d", &x);
		switch (x)
		{
		case 1:
			printf("请输入数据个数(10-150)：");
			scanf("%d", &y);
			intTest1(pTree, y);
			break;

		case 2:
			printf("请输入要查找的数值：");
			scanf("%d", &y);
			intTest2(pTree, y);
			break;

		case 3:
			printf("请输入要插入的数值：");
			scanf("%d,%d", &y, &z);
			intTest3(pTree, y, z);
			break;

		case 4:
			printf("请输入要删除的数值：");
			scanf("%d", &y);
			intTest4(pTree, y);
			break;

		case 5:
			intTest5(pTree);
			break;
		case 6:
			intTest6(pTree);
			break;

		case 7:
			intTest7(pTree);
			break;

		case 0:
			delete pTree;
			return 0;
			break;

		default:
			break;
		}
	}

	delete pTree;
	return 0;
}