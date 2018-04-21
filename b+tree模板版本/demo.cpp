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



// �������һ����
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

// �����в���ĳ����
void intTest2(BPlusTree<int>* pTree, int data)
{
	char sPath[255] = { 0, };

	(void)pTree->Search(data, sPath);
	int a = 0;
	pTree->KeyToValue(data, a);
	printf("\n%s\n%d\n", sPath, a);
}

// �����в���ĳ����
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


// ������ɾ��ĳ����
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
// ��ӡ
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

// ��ӡ
void intTest6(BPlusTree<int>* pTree)
{
	pTree->PrintTree();
}

// �������м��
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
		printf("    *           ��ӭ����B+����ʾ������ѡ����Ӧ���ܡ�                *\n");
		printf("    *           1���������һ��B+����                                 *\n");
		printf("    *           2����B+���в���һ������                               *\n");
		printf("    *           3����B+���в���һ������                               *\n");
		printf("    *           4����B+����ɾ��һ������                               *\n");
		printf("    *           5��ɾ�����ԣ�                                         *\n");
		printf("    *           6����ʾ����B+����                                     *\n");
		printf("    *           7���������B+����                                     *\n");
		printf("    *           0���˳�����                                         *\n");
		printf("    *******************************************************************\n");
		printf("\n    ����ѡ���ǣ�");


		scanf("%d", &x);
		switch (x)
		{
		case 1:
			printf("���������ݸ���(10-150)��");
			scanf("%d", &y);
			intTest1(pTree, y);
			break;

		case 2:
			printf("������Ҫ���ҵ���ֵ��");
			scanf("%d", &y);
			intTest2(pTree, y);
			break;

		case 3:
			printf("������Ҫ�������ֵ��");
			scanf("%d,%d", &y, &z);
			intTest3(pTree, y, z);
			break;

		case 4:
			printf("������Ҫɾ������ֵ��");
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