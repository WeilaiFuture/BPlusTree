/* BPlusTree.h

B+树定义文件，本程序实行一个简单的B+树

(1) 一个v阶的B+树由根结点、内部结点和叶子结点组成。
(2) 根结点可以是叶子结点，也可以是有两个或更多子树的内部结点。
(3) 每个内部结点包含v - 2v个键。如果一个内部结点包含k个键，则有且只有k+1个指向子树的指针。
(4) 叶子结点总是在树的同一层上。
(5) 如果叶子结点是主索引，它包含一组按键值排序的记录；如果叶子结点是从索引，它包含一组短记录，每个短记录包含一个键以及指向实际记录的指针。
(6) 内部结点的键值和叶子结点的数据值都是从小到大排序的。
(7) 在中间结点中，每个键的左子树中的所有的键都小于这个键，每个键的右子树中的所有的键都大于等于这个键。

*/


/* B+树的阶，即内部结点中键的最小数目v。也有些人把阶定义为内部结点中键的最大数目，即2v。
   一般而言，叶子结点中最大数据个数和内部结点中最大键个数是一样的，也是2v。
   (我想这样做的目的是为了把内部结点和叶子结点统一到同一个结构中吧)
*/
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define ORDER_V 2    /* 为简单起见，把v固定为2，实际的B+树v值应该是可配的 */
#define MAXNUM_KEY (ORDER_V * 2)    /* 内部结点中最多键个数，为2v */
#define MAXNUM_POINTER (MAXNUM_KEY + 1)    /* 内部结点中最多指向子树的指针个数，为2v */
#define MAXNUM_DATA (ORDER_V * 2)    /* 叶子结点中最多数据个数，为2v */
#define BPT_FLAG_LEFT 1
#define BPT_FLAG_RIGHT 2
#define BPT_OVERFLOW 0
#define BPT_SUCCESS 1
#define BPT_WRONG_POSITION 2
#define BPT_POSITION_REPEAT 3
/* 结点类型 */
enum NODE_TYPE
{
NODE_TYPE_ROOT     = 1,    // 根结点
NODE_TYPE_INTERNAL = 2,    // 内部结点
NODE_TYPE_LEAF     = 3,    // 叶子结点
};

template<typename KEY_TYPE>
int GetKeyPosition(KEY_TYPE key, KEY_TYPE** key_list, const int &length, int &position){//为0代表错误，获取该值的下标
	if (length == 0)
		return BPT_WRONG_POSITION;
	int low = 0, high = length - 1;
	while (low <= high){
		int middle = (low + high) / 2;
		if (key == *key_list[middle]){
			position = middle + 1;
			return BPT_SUCCESS;
		}
		else if (key < *key_list[middle])
			high = middle - 1;
		else
			low = middle + 1;
	}
	return BPT_WRONG_POSITION;
}

template<typename KEY_TYPE>
int InsertKeyPosition(KEY_TYPE key, KEY_TYPE** key_list, const int length, int &position){//为0代表错误，获取该值插入的下标
	if (length == 0)
		return BPT_WRONG_POSITION;
	int low = 0, high = length - 1;
	if (key < *key_list[low]){
		position = 0;
		return BPT_SUCCESS;
	}
	if (key > *key_list[high]){
		position = high + 1;
		return BPT_SUCCESS;
	}
		
	while (low < high){
		int middle = (low + high) / 2;
		if (key == *key_list[middle]){
			return BPT_POSITION_REPEAT;
		}
		else if (key > *key_list[middle]){
			low = middle;
			if (low == high - 1){
				if (key < *key_list[high]){
					position = high;
					return BPT_SUCCESS;
				}
				else{
					position = high + 1;
					return BPT_SUCCESS;
				}
			}
		}
		else{
			high = middle;
			if (low == high - 1){
				if (key < *key_list[high]){
					position = low + 1;
					return BPT_SUCCESS;
				}
				else{
					position = high + 1;
					return BPT_SUCCESS;
				}
			}
		}
	}
	return BPT_WRONG_POSITION;
}


/* 结点数据结构，为内部结点和叶子结点的父类 */
template<typename  KEY_TYPE>
class CNode
{
public:

	CNode(){
		m_Type = NODE_TYPE_LEAF;
		m_Count = 0;
		m_pFather = NULL;
	};
	virtual ~CNode(){
	};
   
    //获取和设置结点类型
	NODE_TYPE GetType() { return m_Type; }
	void SetType(KEY_TYPE type) { m_Type = type; }

    // 获取和设置有效数据个数
    int GetCount() { return m_Count;}
    void SetCount(int i) { m_Count = i; }
	int InsertAddCount(){ return ++m_Count; }

    // 获取和设置某个元素，对中间结点指键，对叶子结点指数据
	virtual KEY_TYPE GetElement(int i){ return NULL; };
	virtual void SetElement(int i, KEY_TYPE value){};
    // 获取和设置某个指针，对中间结点指指针，对叶子结点无意义
	virtual CNode<KEY_TYPE>* GetPointer(int i) { return NULL; }
    virtual void SetPointer(int i, CNode* pointer) { }

    // 获取和设置父结点
	CNode<KEY_TYPE>* GetFather() { return m_pFather; }
	void SetFather(CNode<KEY_TYPE>* father) { m_pFather = father; }

    // 获取一个最近的兄弟结点
	CNode<KEY_TYPE>* GetBrother(int& flag){
		CNode* pFather = GetFather();
		if (NULL == pFather)
		{
			return NULL;
		}

		CNode* pBrother = NULL;

		for (int i = 1; i <= pFather->GetCount() + 1; i++)
		{
			// 找到本结点的位置
			if (pFather->GetPointer(i) == this)
			{
				if (i == (pFather->GetCount() + 1))
				{
					pBrother = pFather->GetPointer(i - 1);    // 本身是最后一个指针，只能找前一个指针
					flag = BPT_FLAG_LEFT;
				}
				else if (pFather->GetPointer(i + 1)->GetCount()<ORDER_V && i != 1 && pFather->GetCount() >= ORDER_V)
				{
					pBrother = pFather->GetPointer(i - 1);    // 右兄弟数量太少，左兄弟存在且左兄弟数量足够
					flag = BPT_FLAG_LEFT;
				}
				else{
					pBrother = pFather->GetPointer(i + 1);    // 优先找后一个指针
					flag = BPT_FLAG_RIGHT;
				}
			}
		}
		return pBrother;
	};

protected:
   
    NODE_TYPE m_Type;    // 结点类型，取值为NODE_TYPE类型

    int m_Count;    // 有效数据个数，对中间结点指键个数，对叶子结点指数据个数

	CNode<KEY_TYPE>* m_pFather;     // 指向父结点的指针，标准B+树中并没有该指针，加上是为了更快地实现结点分裂和旋转等操作

};

/* 内部结点数据结构 */
template<typename  KEY_TYPE>
class CInternalNode : public CNode<KEY_TYPE>
{
public:

	CInternalNode(){
		m_Type = NODE_TYPE_INTERNAL;
		m_Keys = (KEY_TYPE **)malloc(sizeof(KEY_TYPE*)*MAXNUM_KEY);
		m_Pointers = (CNode<KEY_TYPE>**)malloc(sizeof(CNode<KEY_TYPE>*)*MAXNUM_POINTER);
		int i = 0;
		for (i = 0; i < MAXNUM_KEY; i++)
		{
			m_Keys[i] = NULL;
		}

		for (i = 0; i < MAXNUM_POINTER; i++)
		{
			m_Pointers[i] = NULL;
		}
	};
	virtual ~CInternalNode(){
		for (int i = 1; i <= GetCount(); i++)
		{
			if (this->GetType() != NODE_TYPE_LEAF)    // 叶子结点没有指针
			{
				CNode<KEY_TYPE> * pNode = GetPointer(i);
				delete pNode;
			}
		}
		for (int i = 0; i < m_Count; i++)
		{
			delete m_Keys[i];
		}
		delete m_Pointers;
	};

	void OnlyClearCInternalNode(){
		for (int i = 0; i < MAXNUM_KEY; i++)
		{
			delete m_Keys[i];
		}
		delete m_Pointers;
	}
    // 获取和设置键值，对用户来说，数字从1开始
    KEY_TYPE GetElement(int i)
    {
        if ((i > 0 ) && (i <= MAXNUM_KEY))
            return *m_Keys[i - 1];
        else
			return BPT_OVERFLOW;
    }

    void SetElement(int i, KEY_TYPE key)
    {
		if ((i > 0) && (i <= MAXNUM_KEY)){
			m_Keys[i - 1] = new KEY_TYPE();
			*(m_Keys[i - 1]) = key;
		}
    }

	void ClearElement(int i)
	{
		if ((i > 0) && (i <= MAXNUM_KEY)){
			delete m_Keys[i - 1];
		}
	}
	void ClearPointer(int i)
	{
		if ((i > 0) && (i <= MAXNUM_POINTER)){
			m_Pointers[i - 1] = NULL;
		}
	}
    // 获取和设置指针，对用户来说，数字从1开始
    CNode* GetPointer(int i)
    {
        if ((i > 0 ) && (i <= MAXNUM_POINTER))
        {
            return m_Pointers[i - 1];
        }
        else
        {
			return BPT_OVERFLOW;
        }
    }

	void SetPointer(int i, CNode<KEY_TYPE>* pointer)
    {
        if ((i > 0 ) && (i <= MAXNUM_POINTER))
            m_Pointers[i - 1] = pointer;
    }

    // 插入键
	bool Insert(KEY_TYPE value, CNode<KEY_TYPE>* pNode){
		// 如果中间结点已满，直接返回失败
		if (GetCount() >= MAXNUM_KEY)
			return false;
		int position = 0;
		InsertKeyPosition(value, m_Keys, m_Count, position);

		// 当前位置及其后面的键依次后移，空出当前位置
		for (int j = m_Count; j > position; j--)
		{
			m_Keys[j] = m_Keys[j - 1];
		}

		// 当前位置及其后面的指针依次后移
		for (int j = m_Count + 1; j > position + 1; j--)
		{
			m_Pointers[j] = m_Pointers[j - 1];
		}

		// 把键和指针存入当前位置
		m_Keys[position] = new KEY_TYPE();
		*m_Keys[position] = value;
		m_Pointers[position + 1] = pNode;    // 注意是第i+1个指针而不是第i个指针
		pNode->SetFather(this);      // 非常重要

		m_Count++;

		// 返回成功
		return true;
	};
    // 删除键
	bool Delete(KEY_TYPE value){
		int position = 0;
		GetKeyPosition(value, m_Keys, m_Count, position);
		delete(*(m_Keys + (position - 1)));
		for (int i = position - 1; i < m_Count - 1; i++)
			m_Keys[i] = m_Keys[i + 1];
		memset((void*)(&m_Keys[m_Count - 1]), 0, sizeof(KEY_TYPE*));

		//delete(*(m_Pointers + position));
		for (int i = position; i < m_Count; i++){
			m_Pointers[i] = m_Pointers[i + 1];
		}
		memset((void*)(&m_Pointers[m_Count]), 0, sizeof(KEY_TYPE*));
		this->m_Count--;
		return true;
	}

    // 分裂结点
	KEY_TYPE Split(CInternalNode<KEY_TYPE>* pNode, KEY_TYPE key){
		int i = 0, j = 0;

		// 如果要插入的键值在第V和V+1个键值中间，需要翻转一下，因此先处理此情况
		if ((key > this->GetElement(ORDER_V)) && (key < this->GetElement(ORDER_V + 1)))
		{
			// 把第V+1 -- 2V个键移到指定的结点中

			for (i = ORDER_V + 1; i <= MAXNUM_KEY; i++)
			{
				j++;
				pNode->SetElement(j, this->GetElement(i));
				this->ClearElement(i);
			}

			// 把第V+2 -- 2V+1个指针移到指定的结点中
			j = 0;
			for (i = ORDER_V + 2; i <= MAXNUM_POINTER; i++)
			{
				j++;
				this->GetPointer(i)->SetFather(pNode);    // 重新设置子结点的父亲
				pNode->SetPointer(j, this->GetPointer(i));
				this->ClearPointer(i);
			}

			// 设置好Count个数
			this->SetCount(ORDER_V);
			pNode->SetCount(ORDER_V);

			// 把原键值返回
			return key;
		}

		// 以下处理key小于第V个键值或key大于第V+1个键值的情况

		// 判断是提取第V还是V+1个键
		int position = 0;
		if (key < this->GetElement(ORDER_V))
		{
			position = ORDER_V;
		}
		else
		{
			position = ORDER_V + 1;
		}

		// 把第position个键提出来，作为新的键值返回
		KEY_TYPE RetKey = this->GetElement(position);

		// 把第position+1 -- 2V个键移到指定的结点中
		j = 0;
		for (i = position + 1; i <= MAXNUM_KEY; i++)
		{
			j++;
			pNode->SetElement(j, this->GetElement(i));
			this->ClearElement(i);
		}

		// 把第position+1 -- 2V+1个指针移到指定的结点中(注意指针比键多一个)
		j = 0;
		for (i = position + 1; i <= MAXNUM_POINTER; i++)
		{
			j++;
			this->GetPointer(i)->SetFather(pNode);    // 重新设置子结点的父亲
			pNode->SetPointer(j, this->GetPointer(i));
			this->ClearPointer(i);
		}

		// 清除提取出的位置
		this->ClearElement(position);

		// 设置好Count个数
		this->SetCount(position - 1);
		pNode->SetCount(MAXNUM_KEY - position);


		return RetKey;
	};
    // 结合结点
	bool Combine(CNode* pNode){
		// 参数检查
		if (this->GetCount() + pNode->GetCount() + 1> MAXNUM_DATA)    // 预留一个新键的位置
		{
			return false;
		}

		// 取待合并结点的第一个孩子的第一个元素作为新键值
			
		KEY_TYPE NewKey = NULL;

		CNode<KEY_TYPE> * pFather = pNode->GetFather();
		for (int i = 1; i <= pFather->GetCount() + 1; i++){
			if (pFather->GetPointer(i) == this){
				NewKey = pFather->GetElement(i);
			}
		}

		m_Keys[m_Count] = new KEY_TYPE();
		*m_Keys[m_Count] = NewKey;
		m_Count++;
		m_Pointers[m_Count] = pNode->GetPointer(1);
		m_Pointers[m_Count]->SetFather(this);
		for (int i = 1; i <= pNode->GetCount(); i++)
		{
			m_Keys[m_Count] = new KEY_TYPE();
			*m_Keys[m_Count] = pNode->GetElement(i);
			m_Count++;
			m_Pointers[m_Count] = pNode->GetPointer(i + 1);
			m_Pointers[m_Count]->SetFather(this);
		}

		return true;
	};
    // 从另一结点移一个元素到本结点
	bool MoveOneElement(CNode* pNode){
		// 参数检查
		if (this->GetCount() >= MAXNUM_DATA)
			return false;


		// 兄弟结点在本结点左边
		if (pNode->GetElement(1) < this->GetElement(1))
		{
			// 先腾出位置
			for (int i = m_Count; i > 0; i--)
				m_Keys[i] = m_Keys[i - 1];
			for (int j = m_Count + 1; j > 0; j--)
				m_Pointers[j] = m_Pointers[j - 1];

			// 赋值
			// 第一个键值不是兄弟结点的最后一个键值，而是本结点最左子孙结点的第一个元素的值，也就是父节点中该指针对应的值
			//m_Keys[0] = GetPointer(1)->GetElement(1);
			CNode<KEY_TYPE> *pFather = this->GetFather();
			for (int i = 1; i <= pFather->GetCount() + 1; i++){
				if (pFather->GetPointer(i) == pNode){
					m_Keys[0] = new KEY_TYPE();
					*m_Keys[0] = pFather->GetElement(i);
					pFather->SetElement(i, pNode->GetElement(pNode->GetCount()));
				}
			}
			// 第一个子结点为兄弟结点的最后一个子结点
			m_Pointers[0] = pNode->GetPointer(pNode->GetCount() + 1);
			pNode->GetPointer(pNode->GetCount() + 1)->SetFather(this);


			// 修改兄弟结点
			pNode->SetElement(pNode->GetCount(), NULL);
			pNode->SetPointer(pNode->GetCount() + 1, NULL);
		}
		else    // 兄弟结点在本结点右边
		{
			m_Keys[m_Count] = new KEY_TYPE();
			*m_Keys[m_Count] = pNode->GetPointer(1)->GetElement(1);
			CNode<KEY_TYPE> *pFather = this->GetFather();
			for (int i = 1; i <= pFather->GetCount() + 1; i++){
				if (pFather->GetPointer(i) == this){
					*m_Keys[m_Count] = pFather->GetElement(i);
					pFather->SetElement(i, pNode->GetElement(1));
				}
			}
			m_Pointers[m_Count + 1] = pNode->GetPointer(1);
			m_Pointers[m_Count + 1]->SetFather(this);
			// 修改兄弟结点
			for (int i = 1; i <= pNode->GetCount() - 1; i++)
			{
				pNode->SetElement(i, pNode->GetElement(i + 1));
			}
			pNode->SetElement(pNode->GetCount(), NULL);
			for (int j = 1; j <= pNode->GetCount(); j++)
			{
				pNode->SetPointer(j, pNode->GetPointer(j + 1));
			}
			pNode->SetPointer(pNode->GetCount() + 1, NULL);
		}

		// 设置数目
		this->SetCount(this->GetCount() + 1);
		pNode->SetCount(pNode->GetCount() - 1);

		return true;
	};

protected:

    KEY_TYPE **m_Keys;           // 键数组
	CNode<KEY_TYPE>** m_Pointers;     // 指针数组

};
/* 叶子结点数据结构 */
template<typename  KEY_TYPE>
class CLeafNode : public CNode<KEY_TYPE>
{
public:

	CLeafNode(){
		m_Type = NODE_TYPE_LEAF;
		m_Datas = (KEY_TYPE **)malloc(sizeof(KEY_TYPE*)*MAXNUM_DATA);
		m_Offset = (int *)malloc(sizeof(int)*MAXNUM_DATA);
		for (int i = 0; i < MAXNUM_DATA; i++)
		{
			m_Datas[i] = NULL;
			m_Offset[i] = NULL;
		}

		m_pPrevNode = NULL;
		m_pNextNode = NULL;
	};
	virtual ~CLeafNode(){
		for (int i = 0; i < m_Count; i++)
		{
			delete m_Datas[i];
		}

		delete m_Offset;
	};


    // 获取和设置数据

	int GetElementOffset(int i)
	{
		if ((i > 0) && (i <= MAXNUM_DATA))
		{
			return m_Offset[i - 1];
		}
		else
		{
			return NULL;
		}
	}

	void SetElement(int i, KEY_TYPE data, int offset)
    {
        if ((i > 0 ) && (i <= MAXNUM_DATA))
        {
			m_Datas[i - 1] = new KEY_TYPE();
            *m_Datas[i - 1] = data;
			m_Offset[i - 1] = offset;
        }
    }

	void ClearElement(int i)
	{
		if ((i > 0) && (i <= MAXNUM_DATA))
		{
			delete m_Datas[i - 1];
		}
	}

	KEY_TYPE GetElement(int i)
	{
		return *m_Datas[i - 1];
	}

    // 获取和设置指针，对叶子结点无意义，只是实行父类的虚函数
	CNode<KEY_TYPE>* GetPointer(int i)
    {
        return NULL;
    }
   
    // 插入数据
	bool Insert(KEY_TYPE value, int offset){
		int i = 0;
		// 如果叶子结点已满，直接返回失败
		if (GetCount() >= MAXNUM_DATA)
			return false;

		// 找到要插入数据的位置
		for (i = 0; (i < m_Count) && (value > *m_Datas[i]); i++)
		{
		}

		// 当前位置及其后面的数据依次后移，空出当前位置
		for (int j = m_Count; j > i; j--){
			m_Datas[j] = m_Datas[j - 1];
			m_Offset[j] = m_Offset[j - 1];
		}
		// 把数据存入当前位置
		m_Datas[i] = new KEY_TYPE();
		*m_Datas[i] = value;
		m_Offset[i] = offset;
		m_Count++;

		// 返回成功
		return true;
	};
    // 删除数据
	bool Delete(KEY_TYPE value){
		bool found = false;
		int i;
		int j;
		for (i = 0; i < m_Count; i++)
		{
			if (value == *m_Datas[i])
			{
				found = true;
				break;
			}
		}
		// 如果没有找到，返回失败
		if (false == found)
		{
			return false;
		}
		// 后面的数据依次前移
		for (j = i; j < m_Count - 1; j++)
		{
			*m_Datas[j] = *m_Datas[j + 1];
			m_Offset[j] = m_Offset[j + 1];
		}
		m_Datas[j] = NULL;
		m_Offset[j] = NULL;
		m_Count--;

		// 返回成功
		return true;

	};

    // 分裂结点
	KEY_TYPE Split(CLeafNode* pNode){
		// 把本叶子结点的后一半数据移到指定的结点中
		int j = 0;
		for (int i = ORDER_V + 1; i <= MAXNUM_DATA; i++)
		{
			j++;
			pNode->SetElement(j, this->GetElement(i), this->GetElementOffset(i));
			this->ClearElement(i);
		}
		// 设置好Count个数
		this->SetCount(this->GetCount() - j);
		pNode->SetCount(pNode->GetCount() + j);

		// 返回新结点的第一个元素作为键
		return pNode->GetElement(1);
	};
    // 结合结点
	bool Combine(CLeafNode* pNode){
		// 参数检查
		if (this->GetCount() + pNode->GetCount() > MAXNUM_DATA)
		{
			return false;
		}

		for (int i = 1; i <= pNode->GetCount(); i++)
		{
			this->Insert(pNode->GetElement(i), pNode->GetElementOffset(i));
		}

		return true;
	};

public:
    // 以下两个变量用于实现双向链表
    CLeafNode* m_pPrevNode;                 // 前一个结点
    CLeafNode* m_pNextNode;                 // 后一个结点
   
protected:

    KEY_TYPE **m_Datas;    // 数据数组
	int * m_Offset;		//偏移量数组

};

/* B+树数据结构 */
template<typename  KEY_TYPE>
class BPlusTree
{
public:
  
	BPlusTree(){
		m_Depth = 0;
		leafCount = 0;
		m_Root = NULL;
		m_pLeafHead = NULL;
		m_pLeafTail = NULL;
	};
	virtual ~BPlusTree(){
		ClearTree();
	};

    // 查找指定的数据的路径
	bool Search(KEY_TYPE data, char* sPath){
		int offset = 0;
		int pos = 0;
		CNode<KEY_TYPE> * pNode = GetRoot();
		// 循环查找对应的叶子结点
		while (NULL != pNode)
		{
			// 结点为叶子结点，循环结束
			if (pNode->GetType() == NODE_TYPE_LEAF)
			{
				break;
			}

			// 找到第一个键值大于等于key的位置
			for (pos = 1; (pos <= pNode->GetCount()) && (data >= pNode->GetElement(pos)); pos++)
			{
			}
			pNode = pNode->GetPointer(pos);
		}

		// 没找到
		if (NULL == pNode)
		{
			return false;
		}
		// 在叶子结点中继续找
		bool found = false;
		for (int i = 1; (i <= pNode->GetCount()); i++)
		{
			if (data == pNode->GetElement(i))
			{
				found = true;
			}
		}

		return found;
	};
	//查找叶子节点与key对应的值offset
	bool KeyToValue(KEY_TYPE data, int &offset){
		int i = 0;

		CNode<KEY_TYPE> * pNode = GetRoot();
		// 循环查找对应的叶子结点
		while (NULL != pNode)
		{
			// 结点为叶子结点，循环结束
			if (NODE_TYPE_LEAF == pNode->GetType())
			{
				break;
			}

			// 找到第一个键值大于等于key的位置
			for (i = 1; (i <= pNode->GetCount()) && (data >= pNode->GetElement(i)); i++)
			{
			}
			pNode = pNode->GetPointer(i);
		}

		// 没找到
		if (NULL == pNode)
		{
			return false;
		}

		// 在叶子结点中继续找
		bool found = false;
		for (i = 1; (i <= pNode->GetCount()); i++)
		{
			if (data == pNode->GetElement(i))
			{
				offset = ((CLeafNode<KEY_TYPE> *)pNode)->GetElementOffset(i);
				found = true;
				break;
			}
		}

		return found;
	};
    // 插入指定的数据
	bool Insert(KEY_TYPE data, int offset){
		// 检查是否重复插入
		bool found = Search(data, NULL);
		if (true == found)
		{
			return false;
		}
		// 查找理想的叶子结点
		CLeafNode<KEY_TYPE>* pOldNode = SearchLeafNode(data);
		// 如果没有找到，说明整个树是空的，生成根结点
		if (NULL == pOldNode)
		{
			pOldNode = new CLeafNode<KEY_TYPE>();
			m_pLeafHead = pOldNode;
			m_pLeafTail = pOldNode;
			SetRoot(pOldNode);
		}

		// 叶子结点未满，对应情况1，直接插入
		if (pOldNode->GetCount() < MAXNUM_DATA)
		{
			if (pOldNode->Insert(data, offset)){
				this->IncLeafCount();
				return true;
			}
			else
				return false;
		}

		// 原叶子结点已满，新建叶子结点，并把原结点后一半数据剪切到新结点
		CLeafNode<KEY_TYPE>* pNewNode = new CLeafNode<KEY_TYPE>;
		KEY_TYPE key;
		key = pOldNode->Split(pNewNode);

		// 在双向链表中插入结点
		CLeafNode<KEY_TYPE>* pOldNext = pOldNode->m_pNextNode;
		pOldNode->m_pNextNode = pNewNode;
		pNewNode->m_pNextNode = pOldNext;
		pNewNode->m_pPrevNode = pOldNode;
		if (NULL == pOldNext)
		{
			m_pLeafTail = pNewNode;
		}
		else
		{
			pOldNext->m_pPrevNode = pNewNode;
		}


		// 判断是插入到原结点还是新结点中，确保是按数据值排序的
		if (data < key)
		{
			pOldNode->Insert(data, offset);    // 插入原结点
		}
		else
		{
			pNewNode->Insert(data, offset);    // 插入新结点
		}

		// 父结点
		CInternalNode<KEY_TYPE>* pFather = (CInternalNode<KEY_TYPE>*)(pOldNode->GetFather());

		// 如果原结点是根节点，对应情况2
		if (NULL == pFather)
		{
			CNode<KEY_TYPE>* pNode1 = new CInternalNode<KEY_TYPE>;
			pNode1->SetPointer(1, pOldNode);                           // 指针1指向原结点
			pNode1->SetElement(1, key);                                // 设置键
			pNode1->SetPointer(2, pNewNode);                           // 指针2指向新结点
			pOldNode->SetFather(pNode1);                               // 指定父结点
			pNewNode->SetFather(pNode1);                               // 指定父结点
			pNode1->SetCount(1);

			SetRoot(pNode1);                                           // 指定新的根结点
			this->IncLeafCount();
			return true;
		}

		// 情况3和情况4在这里实现
		bool ret = InsertInternalNode(pFather, key, pNewNode);
		if (ret){
			this->IncLeafCount();
			return true;
		}
		else
			return false;

	};
    // 删除指定的数据
	bool Delete(KEY_TYPE data){
		// 查找理想的叶子结点
		CLeafNode<KEY_TYPE>* pOldNode = SearchLeafNode(data);
		// 如果没有找到，返回失败
		if (NULL == pOldNode)
		{
			return false;
		}

		// 删除数据，如果失败一定是没有找到，直接返回失败
		bool isFirstNode = (pOldNode->GetElement(1) == data);
		bool success = pOldNode->Delete(data);

		if (false == success)
		{
			return false;
		}

		// 获取父结点
		CInternalNode<KEY_TYPE>* pFather = (CInternalNode<KEY_TYPE>*)(pOldNode->GetFather());
		if (NULL == pFather)
		{
			// 如果一个数据都没有了，删除根结点(只有根节点可能出现此情况)
			if (0 == pOldNode->GetCount())
			{
				delete pOldNode;
				m_pLeafHead = NULL;
				m_pLeafTail = NULL;
				SetRoot(NULL);
			}
			this->DecLeafCount();
			return true;
		}


		CInternalNode<KEY_TYPE>* reFather = (CInternalNode<KEY_TYPE>*)(pOldNode->GetFather());
		// 如果删除的是父结点的键值，需要更改该键
		bool breakFlag = true;
		while (reFather && breakFlag && isFirstNode)
		{
			if (data >= reFather->GetElement(1)){//如果大于父节点的第一个值，则该值一定不会出现在父节点层以上
				for (int i = 1; (i <= reFather->GetCount()) && (data >= reFather->GetElement(i)); i++)
				{

					if (reFather->GetElement(i) == data)
					{
						reFather->SetElement(i, pOldNode->GetElement(1));    // 更改为叶子结点新的第一个元素
						breakFlag = false;
					}
				}
			}
			else{//删除节点小于父节点值，该值可能出现在祖父节点中
				reFather = (CInternalNode<KEY_TYPE>*)reFather->GetFather();
			}
		}
		// 删除后叶子结点填充度仍>=50%，对应情况1
		if (pOldNode->GetCount() >= ORDER_V)
		{
			this->DecLeafCount();
			return true;
		}
		// 找到一个最近的兄弟结点(在本节点不是最后节点的情况下，选择右兄弟节点，根据B+树的定义，除了叶子结点，总是能找到的)
		int flag = BPT_FLAG_LEFT;
		CLeafNode<KEY_TYPE>* pBrother = (CLeafNode<KEY_TYPE>*)(pOldNode->GetBrother(flag));

		// 兄弟结点填充度>50%，对应情况2A
		KEY_TYPE NewData = NULL;
		int NewDataOffset = NULL;
		if (pBrother->GetCount() > ORDER_V)
		{
			if (BPT_FLAG_LEFT == flag)    // 兄弟在左边，移最后一个数据过来
			{
				NewData = pBrother->GetElement(pBrother->GetCount());
				NewDataOffset = pBrother->GetElementOffset(pBrother->GetCount());
			}
			else    // 兄弟在右边，移第一个数据过来
			{
				NewData = pBrother->GetElement(1);
				NewDataOffset = pBrother->GetElementOffset(1);
			}

			pOldNode->Insert(NewData, NewDataOffset);
			pBrother->Delete(NewData);

			// 修改父结点的键值
			if (BPT_FLAG_LEFT == flag)
			{
				for (int i = 1; i <= pFather->GetCount() + 1; i++)
				{
					if (pFather->GetPointer(i) == pOldNode && i > 1)
					{
						pFather->SetElement(i - 1, pOldNode->GetElement(1));    // 更改本结点对应的键
					}
				}
			}
			else
			{
				for (int i = 1; i <= pFather->GetCount() + 1; i++)
				{
					if (pFather->GetPointer(i) == pOldNode && i > 1)
					{
						pFather->SetElement(i - 1, pOldNode->GetElement(1));    // 更改本结点对应的键
					}
					if (pFather->GetPointer(i) == pBrother && i > 1)
					{
						pFather->SetElement(i - 1, pBrother->GetElement(1));    // 更改兄弟结点对应的键
					}
				}
			}

			this->DecLeafCount();
			return true;
		}

		// 情况2B

		// 父结点中要删除的键
		KEY_TYPE NewKey = NULL;

		// 把本结点与兄弟结点合并，无论如何合并到数据较小的结点，这样父结点就无需修改指针

		if (BPT_FLAG_LEFT == flag)
		{
			(void)pBrother->Combine(pOldNode);
			NewKey = pOldNode->GetElement(1);

			CLeafNode<KEY_TYPE>* pOldNext = pOldNode->m_pNextNode;
			pBrother->m_pNextNode = pOldNext;
			// 在双向链表中删除结点
			if (NULL == pOldNext)
			{
				m_pLeafTail = pBrother;
			}
			else
			{
				pOldNext->m_pPrevNode = pBrother;
			}
			// 删除本结点
			delete pOldNode;
		}
		else
		{
			(void)pOldNode->Combine(pBrother);
			NewKey = pBrother->GetElement(1);

			CLeafNode<KEY_TYPE>* pOldNext = pBrother->m_pNextNode;
			pOldNode->m_pNextNode = pOldNext;
			// 在双向链表中删除结点
			if (NULL == pOldNext)
			{
				m_pLeafTail = pOldNode;
			}
			else
			{
				pOldNext->m_pPrevNode = pOldNode;
			}
			// 删除本结点
			delete pBrother;
		}

		if (DeleteInternalNode(pFather, NewKey)){
			this->DecLeafCount();
			return true;
		}
		else
			return false;
	};

    // 清除树
	void ClearTree(){
		CNode<KEY_TYPE>* pNode = GetRoot();
		if (NULL != pNode)
		{
			delete pNode;
		}
		clearLeafCount();
		m_pLeafHead = NULL;
		m_pLeafTail = NULL;
		SetRoot(NULL);
	};

    // 打印树
	void PrintTree(){
		CNode<KEY_TYPE>* pRoot = GetRoot();
		if (NULL == pRoot) return;

		CNode<KEY_TYPE>* p1, *p2, *p3;
		int i, j, k;
		int total = 0;

		printf("\n第一层\n | ");
		PrintNode(pRoot);
		total = 0;
		printf("\n第二层\n | ");
		for (i = 1; i <= MAXNUM_POINTER; i++)
		{
			p1 = pRoot->GetPointer(i);
			if (NULL == p1) continue;
			PrintNode(p1);
			total++;
			if (total % 4 == 0) printf("\n | ");
		}
		total = 0;
		printf("\n第三层\n | ");
		for (i = 1; i <= MAXNUM_POINTER; i++)
		{
			p1 = pRoot->GetPointer(i);
			if (NULL == p1) continue;
			for (j = 1; j <= MAXNUM_POINTER; j++)
			{
				p2 = p1->GetPointer(j);
				if (NULL == p2) continue;
				PrintNode(p2);
				total++;
				if (total % 4 == 0) printf("\n | ");
			}
		}
		total = 0;
		printf("\n第四层\n | ");
		for (i = 1; i <= MAXNUM_POINTER; i++)
		{
			p1 = pRoot->GetPointer(i);
			if (NULL == p1) continue;
			for (j = 1; j <= MAXNUM_POINTER; j++)
			{
				p2 = p1->GetPointer(j);
				if (NULL == p2) continue;
				for (k = 1; k <= MAXNUM_POINTER; k++)
				{
					p3 = p2->GetPointer(k);
					if (NULL == p3) continue;
					PrintNode(p3);
					total++;
					if (total % 4 == 0) printf("\n | ");
				}
			}
		}
	};


    // 检查树是否满足B+树的定义
	bool CheckTree(){
		CLeafNode<KEY_TYPE> * pThisNode = m_pLeafHead;
		CLeafNode<KEY_TYPE> * pNextNode = NULL;
		int leafCount = 0;
		if (pThisNode == NULL&&this->getLeafCount() == 0)
			return true;
		while (NULL != pThisNode)
		{
			pNextNode = pThisNode->m_pNextNode;
			leafCount += pThisNode->GetCount();
			for (int i = 1; i<pThisNode->GetCount(); i++){//每个节点内的值按从小到大排序
				if (pThisNode->GetElement(i) > pThisNode->GetElement(i + 1))
					return false;
			}
			if (NULL != pNextNode)
			{
				if (pThisNode->GetElement(pThisNode->GetCount()) > pNextNode->GetElement(1))
					return false;
			}
			pThisNode = pNextNode;
		}
		if (leafCount != this->leafCount)
			return false;
		else if (m_Root->GetFather() != NULL)
			return false;
		else
			return CheckNode(GetRoot());
	};

	void PrintNode(CNode<KEY_TYPE>* pNode){
		if (NULL == pNode)
		{
			return;
		}

		for (int i = 1; i <= MAXNUM_KEY; i++)
		{
			if (pNode->GetType() != 3)
				std::cout << pNode->GetElement(i)<<" ";
			else
				std::cout << pNode->GetElement(i) << "(" << ((CLeafNode<KEY_TYPE> *)pNode)->GetElementOffset(i) << ")  ";
			if (i >= MAXNUM_KEY)
			{
				printf(" | ");
			}
		}
	};

    // 递归检查结点及其子树是否满足B+树的定义
	bool CheckNode(CNode<KEY_TYPE>* pNode){
		if (NULL == pNode)
		{
			return true;
		}

		int i = 0;
		bool ret = false;

		// 检查是否满足50%的填充度
		if ((pNode->GetCount() < ORDER_V) && (pNode->GetCount() > 0) && (pNode != GetRoot()))
		{
			return false;
		}

		// 检查键或数据是否按大小排序
		for (i = 1; i < pNode->GetCount(); i++)
		{
			if (pNode->GetElement(i) > pNode->GetElement(i + 1))
			{
				return false;
			}
		}

		//监测所有节点的父节点，父节点出错会造成严重后果
		if (pNode != GetRoot()){
			CNode<KEY_TYPE> *pFatherNode = new CNode<KEY_TYPE>();
			pFatherNode = pNode->GetFather();
			bool success = false;
			for (int i = 1; i <= pFatherNode->GetCount() + 1; i++){
				if (pFatherNode->GetPointer(i) == pNode)
					success = true;
			}
			if (success == false)
				return false;
		}
		//如果是叶子节点，监测完毕，返回正确值
		if (pNode->GetType() == NODE_TYPE_LEAF)
		{
			return true;
		}

		//查看中间节点中所有值是否在叶子节点出现，如果没有出现，说明插入失败
		for (int i = 1; i <= pNode->GetCount(); i++){
			KEY_TYPE internalVal = pNode->GetElement(i);
			CNode<KEY_TYPE> * node = pNode->GetPointer(i + 1);
			while (node->GetType() != NODE_TYPE_LEAF){
				node = node->GetPointer(1);
			}
			if (internalVal != node->GetElement(1))
				return false;
		}

		//查看中间节点中的值与指针是不是n=m+1的关系
		for (int i = 1; i <= MAXNUM_KEY; i++){
			if ((i <= pNode->GetCount() + 1) && pNode->GetPointer(i) == NULL)
				return false;
			if ((i > pNode->GetCount() + 1) && pNode->GetPointer(i) != NULL)
				return false;
		}



		// 对中间结点，递归检查子树
		for (i = 1; i <= pNode->GetCount() + 1; i++)
		{
			ret = CheckNode(pNode->GetPointer(i));
			// 只要有一个不合法就返回不合法
			if (false == ret)
			{
				return false;
			}
		}

		return true;

	};

    // 获取和设置根结点
	CNode<KEY_TYPE>* GetRoot()
    {
        return m_Root;
    }

	void SetRoot(CNode<KEY_TYPE>* root)
    {
        m_Root = root;
    }

	int getOffsetFromPos(int pos)
	{
		CLeafNode<KEY_TYPE> *node = m_pLeafHead;
		while (pos > node->GetCount()){
			pos = pos - node->GetCount();
			node = node->m_pNextNode;
		}
		return node->GetElementOffset(pos);
	}

    // 获取和设置深度
    int GetDepth()
    {
        return m_Depth;
    }

    void SetDepth(int depth)
    {
        m_Depth = depth;
    }
   
    // 深度加一
    void IncDepth()
    {
        m_Depth = m_Depth + 1;
    }

    // 深度减一
    void DecDepth()
    {
        if (m_Depth > 0)
        {
            m_Depth = m_Depth - 1;
        }
    }

	void IncLeafCount(){
		leafCount++;
	}

	void DecLeafCount(){
		if (leafCount>0)
			leafCount--;
	}

	void clearLeafCount(){
		leafCount = 0;
	}

	int getLeafCount(){
		return leafCount;
	}

	int records_in_range(KEY_TYPE min, KEY_TYPE max){
		int MinPos = 0, MaxPos = 0, Count = 0;
		CLeafNode<KEY_TYPE>* minNode = GetCleafNodeFromKey(min, MinPos);
		CLeafNode<KEY_TYPE>* maxNode = GetCleafNodeFromKey(max, MaxPos);
		Count += minNode->GetCount() - MinPos;
		while (minNode->m_pNextNode != maxNode){
			minNode = minNode->m_pNextNode;
			Count += minNode->GetCount();
		}
		Count += MaxPos;//左开右闭空间
		return Count;
	}


public:
    // 以下两个变量用于实现双向链表
	CLeafNode<KEY_TYPE> *m_pLeafHead;                 // 头结点
	CLeafNode<KEY_TYPE>* m_pLeafTail;                   // 尾结点

protected:

	CLeafNode<KEY_TYPE>* GetCleafNodeFromKey(KEY_TYPE data, int &pos){
		int i = 0;
		CNode<KEY_TYPE> * pNode = GetRoot();
		// 循环查找对应的叶子结点
		while (NULL != pNode)
		{
			// 结点为叶子结点，循环结束
			if (NODE_TYPE_LEAF == pNode->GetType())
			{
				break;
			}

			// 找到第一个键值大于等于key的位置
			for (i = 1; (i <= pNode->GetCount()) && (data >= pNode->GetElement(i)); i++)
			{
			}
			pNode = pNode->GetPointer(i);
		}
		bool found = false;
		for (i = 1; (i <= pNode->GetCount()); i++)
		{
			if (data == pNode->GetElement(i))
			{
				found = true;
				break;
			}
		}
		pos = i;
		return (CLeafNode<KEY_TYPE>*)pNode;
	}

    // 为插入而查找叶子结点
	CLeafNode<KEY_TYPE>* SearchLeafNode(KEY_TYPE data){
		int i = 0;

		CNode<KEY_TYPE> * pNode = GetRoot();
		// 循环查找对应的叶子结点
		while (NULL != pNode)
		{
			// 结点为叶子结点，循环结束
			if (NODE_TYPE_LEAF == pNode->GetType())
			{
				break;
			}

			// 找到第一个键值大于等于key的位置
			for (i = 1; i <= pNode->GetCount(); i++)
			{
				if (data < pNode->GetElement(i))
				{
					break;
				}
			}

			pNode = pNode->GetPointer(i);
		}

		return (CLeafNode<KEY_TYPE>*)pNode;
	}
	;
    //插入键到中间结点
	bool InsertInternalNode(CInternalNode<KEY_TYPE>* pNode, KEY_TYPE key, CNode<KEY_TYPE>* pRightSon){
		if (NULL == pNode || NODE_TYPE_LEAF == pNode->GetType())
		{
			return false;
		}

		// 结点未满，直接插入
		if (pNode->GetCount() < MAXNUM_KEY)
		{
			return pNode->Insert(key, pRightSon);
		}

		CInternalNode<KEY_TYPE>* pBrother = new CInternalNode<KEY_TYPE>();
		KEY_TYPE NewKey;
		// 分裂本中间结点
		NewKey = pNode->Split(pBrother, key);

		if (pNode->GetCount() < pBrother->GetCount())
		{
			pNode->Insert(key, pRightSon);
		}
		else if (pNode->GetCount() > pBrother->GetCount())
		{
			pBrother->Insert(key, pRightSon);
		}
		else    // 两者相等，即键值在第V和V+1个键值中间的情况，把字节点挂到新结点的第一个指针上
		{
			for (int i = pBrother->GetCount(); i >= 1; i--){
				pBrother->SetPointer(i + 1, pBrother->GetPointer(i));
			}
			pBrother->SetPointer(1, pRightSon);
			pRightSon->SetFather(pBrother);
		}

		CInternalNode<KEY_TYPE>* pFather = (CInternalNode<KEY_TYPE>*)(pNode->GetFather());
		// 直到根结点都满了，新生成根结点
		if (NULL == pFather)
		{
			pFather = new CInternalNode<KEY_TYPE>;
			pFather->SetPointer(1, pNode);                           // 指针1指向原结点
			pFather->SetElement(1, NewKey);                          // 设置键
			pFather->SetPointer(2, pBrother);                        // 指针2指向新结点
			pNode->SetFather(pFather);                               // 指定父结点
			pBrother->SetFather(pFather);                            // 指定父结点
			pFather->SetCount(1);
			SetRoot(pFather);                                        // 指定新的根结点
			IncDepth();
			return true;
		}

		// 递归
		return InsertInternalNode(pFather, NewKey, pBrother);
	};
    // 在中间结点中删除键
	bool DeleteInternalNode(CInternalNode<KEY_TYPE>* pNode, KEY_TYPE key){
		// 删除键，如果失败一定是没有找到，直接返回失败
		bool success = pNode->Delete(key);
		if (false == success)
		{
			return false;
		}

		// 获取父结点
		CInternalNode<KEY_TYPE>* pFather = (CInternalNode<KEY_TYPE>*)(pNode->GetFather());
		if (NULL == pFather)
		{
			// 如果一个数据都没有了，把根结点的第一个结点作为根结点
			if (0 == pNode->GetCount())
			{
				SetRoot(pNode->GetPointer(1));
				pNode->GetPointer(1)->SetFather(NULL);
				delete pNode;
			}

			return true;
		}

		// 删除后结点填充度仍>=50%
		if (pNode->GetCount() >= ORDER_V)
		{
			return true;
		}

		// 找到一个最近的兄弟结点(根据B+树的定义，除了根结点，总是能找到的)
		int flag = BPT_FLAG_LEFT;
		CInternalNode<KEY_TYPE>* pBrother = (CInternalNode<KEY_TYPE>*)(pNode->GetBrother(flag));

		// 兄弟结点填充度>50%
		KEY_TYPE NewData = NULL;
		if (pBrother->GetCount() > ORDER_V)
		{
			pNode->MoveOneElement(pBrother);
			return true;
		}

		// 父结点中要删除的键
		KEY_TYPE NewKey = NULL;

		// 把本结点与兄弟结点合并，无论如何合并到数据较小的结点，这样父结点就无需修改指针
		if (BPT_FLAG_LEFT == flag)
		{
			(void)pBrother->Combine(pNode);
			for (int i = 1; i <= pFather->GetCount() + 1; i++){
				if (pFather->GetPointer(i) == pBrother){
					NewKey = pFather->GetElement(i);
				}
			}
			pNode->OnlyClearCInternalNode();
		}
		else
		{
			(void)pNode->Combine(pBrother);
			for (int i = 1; i <= pFather->GetCount() + 1; i++){
				if (pFather->GetPointer(i) == pNode){
					NewKey = pFather->GetElement(i);
				}
			}
			pBrother->OnlyClearCInternalNode();
		}

		// 递归
		return DeleteInternalNode(pFather, NewKey);
	};
   
	CNode<KEY_TYPE>* m_Root;    // 根结点
    int m_Depth;      // 树的深度
	int leafCount;	 //树中叶子节点的总数
};

/* end of BPlusTree.h */