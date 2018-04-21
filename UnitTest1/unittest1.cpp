#include "stdafx.h"
#include "CppUnitTest.h"
#include "BPlusTree.h"
#include "stdlib.h"
#include <vector>
#include "time.h"
#include <string>
using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


class IntRandNum{
private:
	int* num = NULL;
	int index = 0;
public:
	IntRandNum(int numCout){
		num = (int *)malloc(sizeof(int)*numCout);
		//srand((unsigned)time(NULL));
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
	IntRandNum(){
		num = (int *)malloc(sizeof(int) * 1000);
		//srand((unsigned)time(NULL));
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
	~IntRandNum(){
		delete num;
	}
	int getRandNum(){
		return num[index++];
	}
};


class StringRandNum{
private:
	int SIZE_CHAR = 32; //生成32 + 1位C Style字符串
	char *CCH = "_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
public:
	vector<string> strArray;
	StringRandNum(int numCout){
		//srand((unsigned)time(NULL));
		for (int i = 0; i < numCout; i++){
			string str;
			bool flag = true;
			do{
				for (int i = 0; i < SIZE_CHAR; ++i)
				{
					int x = rand() / (RAND_MAX / (sizeof(CCH) - 1));
					str.insert(str.end(), CCH[x]);
				}
				for (string a : strArray){
					if (a == str){
						flag = false;
					}
				}
			} while (!flag);
			strArray.push_back(str);
		}
	}
	StringRandNum(){
		//srand((unsigned)time(NULL));
		for (int i = 0; i < 1000; i++){
			string str;
			bool flag = true;
			do{
				for (int i = 0; i < SIZE_CHAR; ++i)
				{
					int x = rand() / (RAND_MAX / (strlen(CCH) - 1));
					str.insert(str.end(), CCH[x]);
				}
				for (string a : strArray){
					if (a == str){
						flag = false;
					}
				}
			} while (!flag);
			strArray.push_back(str);
		}
	}
	string getRandNum(){
		string str;
		str = strArray.back();
		strArray.pop_back();
		return str;
	}
};

namespace unittest
{
	TEST_CLASS(UnitTest1)
	{
	public:
		//内部节点类测试
		//内部节点类测试
		//内部节点类测试
		//内部节点类测试
		TEST_METHOD(NODE构造函数测试)
		{
			CNode<int> *node = new CNode<int>();
			Assert::IsTrue((node->GetType()) == NODE_TYPE_LEAF && (node->GetCount()) == 0 && (node->GetFather()) == NULL);
			delete node;
		}
		/*TEST_METHOD(内部节点设置获取element)
		{
			CInternalNode<int> * cInternalNode = new CInternalNode<int>();
			cInternalNode->SetElement(1, 100);
			cInternalNode->SetElement(2, 101);
			Assert::IsTrue(cInternalNode->GetElement(1) == 100 && cInternalNode->GetElement(2) == 101 && cInternalNode->GetElement(3) == NULL&&cInternalNode->GetElement(MAXNUM_KEY + 1) == NULL);
			//测试一节点和二节点是否可以正常存取，同时测试空节点及溢出节点的获取
			delete cInternalNode;
		}*/
		TEST_METHOD(内部节点设置获取指针测试)
		{
			CInternalNode<int> * cInternalNode = new CInternalNode<int>();
			CInternalNode<int> * insertNode = new CInternalNode<int>();
			cInternalNode->SetPointer(1, insertNode);
			Assert::IsTrue(cInternalNode->GetPointer(1) == insertNode&&cInternalNode->GetPointer(2) == NULL&&
				cInternalNode->GetPointer(MAXNUM_KEY + 1) == NULL);
			delete cInternalNode;
		}
		/*TEST_METHOD(内部节点插入删除操作)//order为2
		{
			CInternalNode<int> * cInternalNode = new CInternalNode<int>();
			CInternalNode<int> * insertNode = new CInternalNode<int>();
			for (int i = 1, j = 1, z = 1; i < MAXNUM_KEY; i++, j++, z++){
				if (i == MAXNUM_KEY / 2)
					j++;
				if (i == MAXNUM_KEY / 2 + 1)
					z++;
				cInternalNode->SetElement(i, j);
				cInternalNode->SetPointer(i, cInternalNode + z);//这些指针是不能访问的，这里只做测试使用
				cInternalNode->InsertAddCount();
			}//通过这种方式，使内部节点变为 值1 3 4|指针1 2 4 5 然后插入 值2|指针3的节点 则内部节点最终变为 1234|12345
			cInternalNode->SetPointer(MAXNUM_KEY, cInternalNode + MAXNUM_KEY + 1);
			Assert::IsTrue(cInternalNode->GetElement(1) == 1 && cInternalNode->GetPointer(1) == cInternalNode + 1 &&
				cInternalNode->GetCount() == 3);
			cInternalNode->Insert(MAXNUM_KEY / 2, cInternalNode + MAXNUM_KEY / 2 + 1);
			int test = cInternalNode->GetPointer(2) - cInternalNode->GetPointer(1);
			for (int i = 2; i <= MAXNUM_KEY; i++){
				Assert::IsTrue(cInternalNode->GetElement(i) - cInternalNode->GetElement(i - 1) == 1
					&& (CInternalNode<int> *)cInternalNode->GetPointer(i) - (CInternalNode<int> *)cInternalNode->GetPointer(i - 1) == 1);
			}
			Assert::IsFalse(cInternalNode->Insert(MAXNUM_KEY + 1, cInternalNode + MAXNUM_KEY + 2));
			cInternalNode->Delete(1);
			Assert::IsTrue(cInternalNode->GetCount() == 3 && cInternalNode->GetElement(1) == 2);
			delete cInternalNode;
		}*/
		/*TEST_METHOD(内部节点分裂节点操作)//该分裂过程只把节点对半分裂，并不插入节点
		{
			CInternalNode<int> * cInternalNode = new CInternalNode<int>();
			CInternalNode<int> * brotherNode = new CInternalNode<int>();
			for (int i = 1; i <= MAXNUM_KEY; i++){
				cInternalNode->SetElement(i, 2 * i);
				cInternalNode->SetPointer(i, cInternalNode + i);//这些指针是不能访问的，这里只做测试使用
				cInternalNode->InsertAddCount();
			}//通过这种方式，使内部节点变为 key值 2468| 指针 12345
			cInternalNode->SetPointer(MAXNUM_KEY + 1, cInternalNode + MAXNUM_KEY + 1);

			int key = cInternalNode->Split(brotherNode, 1);
			Assert::IsTrue(cInternalNode->GetCount() == 1 && brotherNode->GetCount() == 2
				&& cInternalNode->GetElement(1) == 2 && brotherNode->GetElement(1) == 6);
			delete cInternalNode;
		}*/






		//BPlus树类测试
		//BPlus树类测试
		//BPlus树类测试
		//BPlus树类测试

		TEST_METHOD(测试随机数生成)
		{
			IntRandNum * randNum = new IntRandNum();
			int a = 0;
			for (int i = 0; i < 1000; i++){
				a += randNum->getRandNum();
			}
			Assert::IsTrue(a == 500500);
		}

		TEST_METHOD(测试随机字符串生成)
		{
			StringRandNum * randString = new StringRandNum();

			Assert::IsTrue(randString->strArray.size() == 1000);
		}

		TEST_METHOD(测试插入操作)
		{
			BPlusTree<int> *tree = new BPlusTree<int>();
			IntRandNum * randNum = new IntRandNum();
			for (int i = 0; i < 1000; i++){
				int a = randNum->getRandNum();
				tree->Insert(a, a);
				if (!tree->CheckTree()){
					Assert::IsTrue(tree->CheckTree());
				}

			}
			CLeafNode<int> * start = tree->m_pLeafHead;
			int sum = 0;
			while (start){
				for (int i = 1; i <= start->GetCount(); i++){
					sum += start->GetElement(i);
				}
				start = start->m_pNextNode;
			}
			Assert::IsTrue(tree->getLeafCount() == 1000 && tree->CheckTree() && sum == 500500);
			delete tree;
			delete randNum;
		}

		TEST_METHOD(测试String插入操作)
		{
			BPlusTree<string> *tree = new BPlusTree<string>();
			StringRandNum * randNum = new StringRandNum();
			for (int i = 0; i < 1000; i++){
				string a = randNum->getRandNum();
				tree->Insert(a, i);
				if (!tree->CheckTree()){
					Assert::IsTrue(tree->CheckTree());
				}

			}
			delete tree;
			delete randNum;
		}

		TEST_METHOD(测试删除操作){
			BPlusTree<int> *tree = new BPlusTree<int>();
			int a = 1000;
			int b = 489;
			IntRandNum * insertRandNum = new IntRandNum(a);
			IntRandNum * deleteRandNum = new IntRandNum(a);
			for (int i = 0; i < a; i++){
				int a = insertRandNum->getRandNum();
				tree->Insert(a, a);
				if (!tree->CheckTree()){
					Assert::IsTrue(1);
				}
			}
			for (int i = 0; i < b; i++){
				int a = deleteRandNum->getRandNum();
				tree->Delete(a);
				if (!tree->CheckTree()){
					Assert::IsTrue(1);
				}
			}
			Assert::IsTrue(tree->getLeafCount() == a - b && tree->CheckTree());
			for (int i = 0; i < a - b; i++){
				int a = deleteRandNum->getRandNum();
				tree->Delete(a);
			}
			Assert::IsTrue(tree->getLeafCount() == 0 && tree->CheckTree());
		}
		TEST_METHOD(测试pos查询){
			BPlusTree<int> *tree = new BPlusTree<int>();
			int a = 1000;
			IntRandNum * insertRandNum = new IntRandNum(a);
			for (int i = 0; i < a; i++){
				int a = insertRandNum->getRandNum();
				tree->Insert(a, a);
			}

			Assert::IsTrue(tree->getLeafCount() == 1000 && tree->CheckTree());
			for (int i = 1; i <= 1000; i++){
				int a = tree->getOffsetFromPos(i);
				Assert::IsTrue(a == i);
			}
		}

		TEST_METHOD(测试range查询){
			BPlusTree<int> *tree = new BPlusTree<int>();
			int a = 1000;
			IntRandNum * insertRandNum = new IntRandNum(a);
			for (int i = 0; i < a; i++){
				int b = insertRandNum->getRandNum();
				tree->Insert(b, b);
			}
			Assert::IsTrue(tree->getLeafCount() == 1000 && tree->CheckTree());

			for (int i = 1; i <= 499; i++){
				int b = tree->records_in_range(i, 1000 - i);
				Assert::IsTrue(b == 1000 - 2 * i);
			}
		}

		/*TEST_METHOD(测试二分法查找){
			int a[] = { 1, 314, 578, 757, 845, 1024, 2065 };
			int positiona = 0;
			InsertKeyPosition(648, (int **)&a, 7, positiona);
			Assert::IsTrue(positiona == 3);
			int b[] = { 1 };
			int positionb = 0;
			InsertKeyPosition(648, (int **)&b, 1, positionb);
			Assert::IsTrue(positionb == 1);
			int c[] = { 5 };
			int positionc = 0;
			InsertKeyPosition(1, (int **)&c, 1, positionc);
			Assert::IsTrue(positionc == 0);
		}*/

	};


}