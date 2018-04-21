/* BPlusTree.h

B+�������ļ���������ʵ��һ���򵥵�B+��

(1) һ��v�׵�B+���ɸ���㡢�ڲ�����Ҷ�ӽ����ɡ�
(2) ����������Ҷ�ӽ�㣬Ҳ������������������������ڲ���㡣
(3) ÿ���ڲ�������v - 2v���������һ���ڲ�������k������������ֻ��k+1��ָ��������ָ�롣
(4) Ҷ�ӽ������������ͬһ���ϡ�
(5) ���Ҷ�ӽ������������������һ�鰴��ֵ����ļ�¼�����Ҷ�ӽ���Ǵ�������������һ��̼�¼��ÿ���̼�¼����һ�����Լ�ָ��ʵ�ʼ�¼��ָ�롣
(6) �ڲ����ļ�ֵ��Ҷ�ӽ�������ֵ���Ǵ�С��������ġ�
(7) ���м����У�ÿ�������������е����еļ���С���������ÿ�������������е����еļ������ڵ����������

*/


/* B+���Ľף����ڲ�����м�����С��Ŀv��Ҳ��Щ�˰ѽ׶���Ϊ�ڲ�����м��������Ŀ����2v��
   һ����ԣ�Ҷ�ӽ����������ݸ������ڲ����������������һ���ģ�Ҳ��2v��
   (������������Ŀ����Ϊ�˰��ڲ�����Ҷ�ӽ��ͳһ��ͬһ���ṹ�а�)
*/
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define ORDER_V 2    /* Ϊ���������v�̶�Ϊ2��ʵ�ʵ�B+��vֵӦ���ǿ���� */
#define MAXNUM_KEY (ORDER_V * 2)    /* �ڲ����������������Ϊ2v */
#define MAXNUM_POINTER (MAXNUM_KEY + 1)    /* �ڲ���������ָ��������ָ�������Ϊ2v */
#define MAXNUM_DATA (ORDER_V * 2)    /* Ҷ�ӽ����������ݸ�����Ϊ2v */
#define BPT_FLAG_LEFT 1
#define BPT_FLAG_RIGHT 2
#define BPT_OVERFLOW 0
#define BPT_SUCCESS 1
#define BPT_WRONG_POSITION 2
#define BPT_POSITION_REPEAT 3
/* ������� */
enum NODE_TYPE
{
NODE_TYPE_ROOT     = 1,    // �����
NODE_TYPE_INTERNAL = 2,    // �ڲ����
NODE_TYPE_LEAF     = 3,    // Ҷ�ӽ��
};

template<typename KEY_TYPE>
int GetKeyPosition(KEY_TYPE key, KEY_TYPE** key_list, const int &length, int &position){//Ϊ0������󣬻�ȡ��ֵ���±�
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
int InsertKeyPosition(KEY_TYPE key, KEY_TYPE** key_list, const int length, int &position){//Ϊ0������󣬻�ȡ��ֵ������±�
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


/* ������ݽṹ��Ϊ�ڲ�����Ҷ�ӽ��ĸ��� */
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
   
    //��ȡ�����ý������
	NODE_TYPE GetType() { return m_Type; }
	void SetType(KEY_TYPE type) { m_Type = type; }

    // ��ȡ��������Ч���ݸ���
    int GetCount() { return m_Count;}
    void SetCount(int i) { m_Count = i; }
	int InsertAddCount(){ return ++m_Count; }

    // ��ȡ������ĳ��Ԫ�أ����м���ָ������Ҷ�ӽ��ָ����
	virtual KEY_TYPE GetElement(int i){ return NULL; };
	virtual void SetElement(int i, KEY_TYPE value){};
    // ��ȡ������ĳ��ָ�룬���м���ָָ�룬��Ҷ�ӽ��������
	virtual CNode<KEY_TYPE>* GetPointer(int i) { return NULL; }
    virtual void SetPointer(int i, CNode* pointer) { }

    // ��ȡ�����ø����
	CNode<KEY_TYPE>* GetFather() { return m_pFather; }
	void SetFather(CNode<KEY_TYPE>* father) { m_pFather = father; }

    // ��ȡһ��������ֵܽ��
	CNode<KEY_TYPE>* GetBrother(int& flag){
		CNode* pFather = GetFather();
		if (NULL == pFather)
		{
			return NULL;
		}

		CNode* pBrother = NULL;

		for (int i = 1; i <= pFather->GetCount() + 1; i++)
		{
			// �ҵ�������λ��
			if (pFather->GetPointer(i) == this)
			{
				if (i == (pFather->GetCount() + 1))
				{
					pBrother = pFather->GetPointer(i - 1);    // ���������һ��ָ�룬ֻ����ǰһ��ָ��
					flag = BPT_FLAG_LEFT;
				}
				else if (pFather->GetPointer(i + 1)->GetCount()<ORDER_V && i != 1 && pFather->GetCount() >= ORDER_V)
				{
					pBrother = pFather->GetPointer(i - 1);    // ���ֵ�����̫�٣����ֵܴ��������ֵ������㹻
					flag = BPT_FLAG_LEFT;
				}
				else{
					pBrother = pFather->GetPointer(i + 1);    // �����Һ�һ��ָ��
					flag = BPT_FLAG_RIGHT;
				}
			}
		}
		return pBrother;
	};

protected:
   
    NODE_TYPE m_Type;    // ������ͣ�ȡֵΪNODE_TYPE����

    int m_Count;    // ��Ч���ݸ��������м���ָ����������Ҷ�ӽ��ָ���ݸ���

	CNode<KEY_TYPE>* m_pFather;     // ָ�򸸽���ָ�룬��׼B+���в�û�и�ָ�룬������Ϊ�˸����ʵ�ֽ����Ѻ���ת�Ȳ���

};

/* �ڲ�������ݽṹ */
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
			if (this->GetType() != NODE_TYPE_LEAF)    // Ҷ�ӽ��û��ָ��
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
    // ��ȡ�����ü�ֵ�����û���˵�����ִ�1��ʼ
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
    // ��ȡ������ָ�룬���û���˵�����ִ�1��ʼ
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

    // �����
	bool Insert(KEY_TYPE value, CNode<KEY_TYPE>* pNode){
		// ����м���������ֱ�ӷ���ʧ��
		if (GetCount() >= MAXNUM_KEY)
			return false;
		int position = 0;
		InsertKeyPosition(value, m_Keys, m_Count, position);

		// ��ǰλ�ü������ļ����κ��ƣ��ճ���ǰλ��
		for (int j = m_Count; j > position; j--)
		{
			m_Keys[j] = m_Keys[j - 1];
		}

		// ��ǰλ�ü�������ָ�����κ���
		for (int j = m_Count + 1; j > position + 1; j--)
		{
			m_Pointers[j] = m_Pointers[j - 1];
		}

		// �Ѽ���ָ����뵱ǰλ��
		m_Keys[position] = new KEY_TYPE();
		*m_Keys[position] = value;
		m_Pointers[position + 1] = pNode;    // ע���ǵ�i+1��ָ������ǵ�i��ָ��
		pNode->SetFather(this);      // �ǳ���Ҫ

		m_Count++;

		// ���سɹ�
		return true;
	};
    // ɾ����
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

    // ���ѽ��
	KEY_TYPE Split(CInternalNode<KEY_TYPE>* pNode, KEY_TYPE key){
		int i = 0, j = 0;

		// ���Ҫ����ļ�ֵ�ڵ�V��V+1����ֵ�м䣬��Ҫ��תһ�£�����ȴ�������
		if ((key > this->GetElement(ORDER_V)) && (key < this->GetElement(ORDER_V + 1)))
		{
			// �ѵ�V+1 -- 2V�����Ƶ�ָ���Ľ����

			for (i = ORDER_V + 1; i <= MAXNUM_KEY; i++)
			{
				j++;
				pNode->SetElement(j, this->GetElement(i));
				this->ClearElement(i);
			}

			// �ѵ�V+2 -- 2V+1��ָ���Ƶ�ָ���Ľ����
			j = 0;
			for (i = ORDER_V + 2; i <= MAXNUM_POINTER; i++)
			{
				j++;
				this->GetPointer(i)->SetFather(pNode);    // ���������ӽ��ĸ���
				pNode->SetPointer(j, this->GetPointer(i));
				this->ClearPointer(i);
			}

			// ���ú�Count����
			this->SetCount(ORDER_V);
			pNode->SetCount(ORDER_V);

			// ��ԭ��ֵ����
			return key;
		}

		// ���´���keyС�ڵ�V����ֵ��key���ڵ�V+1����ֵ�����

		// �ж�����ȡ��V����V+1����
		int position = 0;
		if (key < this->GetElement(ORDER_V))
		{
			position = ORDER_V;
		}
		else
		{
			position = ORDER_V + 1;
		}

		// �ѵ�position�������������Ϊ�µļ�ֵ����
		KEY_TYPE RetKey = this->GetElement(position);

		// �ѵ�position+1 -- 2V�����Ƶ�ָ���Ľ����
		j = 0;
		for (i = position + 1; i <= MAXNUM_KEY; i++)
		{
			j++;
			pNode->SetElement(j, this->GetElement(i));
			this->ClearElement(i);
		}

		// �ѵ�position+1 -- 2V+1��ָ���Ƶ�ָ���Ľ����(ע��ָ��ȼ���һ��)
		j = 0;
		for (i = position + 1; i <= MAXNUM_POINTER; i++)
		{
			j++;
			this->GetPointer(i)->SetFather(pNode);    // ���������ӽ��ĸ���
			pNode->SetPointer(j, this->GetPointer(i));
			this->ClearPointer(i);
		}

		// �����ȡ����λ��
		this->ClearElement(position);

		// ���ú�Count����
		this->SetCount(position - 1);
		pNode->SetCount(MAXNUM_KEY - position);


		return RetKey;
	};
    // ��Ͻ��
	bool Combine(CNode* pNode){
		// �������
		if (this->GetCount() + pNode->GetCount() + 1> MAXNUM_DATA)    // Ԥ��һ���¼���λ��
		{
			return false;
		}

		// ȡ���ϲ����ĵ�һ�����ӵĵ�һ��Ԫ����Ϊ�¼�ֵ
			
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
    // ����һ�����һ��Ԫ�ص������
	bool MoveOneElement(CNode* pNode){
		// �������
		if (this->GetCount() >= MAXNUM_DATA)
			return false;


		// �ֵܽ���ڱ�������
		if (pNode->GetElement(1) < this->GetElement(1))
		{
			// ���ڳ�λ��
			for (int i = m_Count; i > 0; i--)
				m_Keys[i] = m_Keys[i - 1];
			for (int j = m_Count + 1; j > 0; j--)
				m_Pointers[j] = m_Pointers[j - 1];

			// ��ֵ
			// ��һ����ֵ�����ֵܽ������һ����ֵ�����Ǳ��������������ĵ�һ��Ԫ�ص�ֵ��Ҳ���Ǹ��ڵ��и�ָ���Ӧ��ֵ
			//m_Keys[0] = GetPointer(1)->GetElement(1);
			CNode<KEY_TYPE> *pFather = this->GetFather();
			for (int i = 1; i <= pFather->GetCount() + 1; i++){
				if (pFather->GetPointer(i) == pNode){
					m_Keys[0] = new KEY_TYPE();
					*m_Keys[0] = pFather->GetElement(i);
					pFather->SetElement(i, pNode->GetElement(pNode->GetCount()));
				}
			}
			// ��һ���ӽ��Ϊ�ֵܽ������һ���ӽ��
			m_Pointers[0] = pNode->GetPointer(pNode->GetCount() + 1);
			pNode->GetPointer(pNode->GetCount() + 1)->SetFather(this);


			// �޸��ֵܽ��
			pNode->SetElement(pNode->GetCount(), NULL);
			pNode->SetPointer(pNode->GetCount() + 1, NULL);
		}
		else    // �ֵܽ���ڱ�����ұ�
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
			// �޸��ֵܽ��
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

		// ������Ŀ
		this->SetCount(this->GetCount() + 1);
		pNode->SetCount(pNode->GetCount() - 1);

		return true;
	};

protected:

    KEY_TYPE **m_Keys;           // ������
	CNode<KEY_TYPE>** m_Pointers;     // ָ������

};
/* Ҷ�ӽ�����ݽṹ */
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


    // ��ȡ����������

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

    // ��ȡ������ָ�룬��Ҷ�ӽ�������壬ֻ��ʵ�и�����麯��
	CNode<KEY_TYPE>* GetPointer(int i)
    {
        return NULL;
    }
   
    // ��������
	bool Insert(KEY_TYPE value, int offset){
		int i = 0;
		// ���Ҷ�ӽ��������ֱ�ӷ���ʧ��
		if (GetCount() >= MAXNUM_DATA)
			return false;

		// �ҵ�Ҫ�������ݵ�λ��
		for (i = 0; (i < m_Count) && (value > *m_Datas[i]); i++)
		{
		}

		// ��ǰλ�ü��������������κ��ƣ��ճ���ǰλ��
		for (int j = m_Count; j > i; j--){
			m_Datas[j] = m_Datas[j - 1];
			m_Offset[j] = m_Offset[j - 1];
		}
		// �����ݴ��뵱ǰλ��
		m_Datas[i] = new KEY_TYPE();
		*m_Datas[i] = value;
		m_Offset[i] = offset;
		m_Count++;

		// ���سɹ�
		return true;
	};
    // ɾ������
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
		// ���û���ҵ�������ʧ��
		if (false == found)
		{
			return false;
		}
		// �������������ǰ��
		for (j = i; j < m_Count - 1; j++)
		{
			*m_Datas[j] = *m_Datas[j + 1];
			m_Offset[j] = m_Offset[j + 1];
		}
		m_Datas[j] = NULL;
		m_Offset[j] = NULL;
		m_Count--;

		// ���سɹ�
		return true;

	};

    // ���ѽ��
	KEY_TYPE Split(CLeafNode* pNode){
		// �ѱ�Ҷ�ӽ��ĺ�һ�������Ƶ�ָ���Ľ����
		int j = 0;
		for (int i = ORDER_V + 1; i <= MAXNUM_DATA; i++)
		{
			j++;
			pNode->SetElement(j, this->GetElement(i), this->GetElementOffset(i));
			this->ClearElement(i);
		}
		// ���ú�Count����
		this->SetCount(this->GetCount() - j);
		pNode->SetCount(pNode->GetCount() + j);

		// �����½��ĵ�һ��Ԫ����Ϊ��
		return pNode->GetElement(1);
	};
    // ��Ͻ��
	bool Combine(CLeafNode* pNode){
		// �������
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
    // ����������������ʵ��˫������
    CLeafNode* m_pPrevNode;                 // ǰһ�����
    CLeafNode* m_pNextNode;                 // ��һ�����
   
protected:

    KEY_TYPE **m_Datas;    // ��������
	int * m_Offset;		//ƫ��������

};

/* B+�����ݽṹ */
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

    // ����ָ�������ݵ�·��
	bool Search(KEY_TYPE data, char* sPath){
		int offset = 0;
		int pos = 0;
		CNode<KEY_TYPE> * pNode = GetRoot();
		// ѭ�����Ҷ�Ӧ��Ҷ�ӽ��
		while (NULL != pNode)
		{
			// ���ΪҶ�ӽ�㣬ѭ������
			if (pNode->GetType() == NODE_TYPE_LEAF)
			{
				break;
			}

			// �ҵ���һ����ֵ���ڵ���key��λ��
			for (pos = 1; (pos <= pNode->GetCount()) && (data >= pNode->GetElement(pos)); pos++)
			{
			}
			pNode = pNode->GetPointer(pos);
		}

		// û�ҵ�
		if (NULL == pNode)
		{
			return false;
		}
		// ��Ҷ�ӽ���м�����
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
	//����Ҷ�ӽڵ���key��Ӧ��ֵoffset
	bool KeyToValue(KEY_TYPE data, int &offset){
		int i = 0;

		CNode<KEY_TYPE> * pNode = GetRoot();
		// ѭ�����Ҷ�Ӧ��Ҷ�ӽ��
		while (NULL != pNode)
		{
			// ���ΪҶ�ӽ�㣬ѭ������
			if (NODE_TYPE_LEAF == pNode->GetType())
			{
				break;
			}

			// �ҵ���һ����ֵ���ڵ���key��λ��
			for (i = 1; (i <= pNode->GetCount()) && (data >= pNode->GetElement(i)); i++)
			{
			}
			pNode = pNode->GetPointer(i);
		}

		// û�ҵ�
		if (NULL == pNode)
		{
			return false;
		}

		// ��Ҷ�ӽ���м�����
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
    // ����ָ��������
	bool Insert(KEY_TYPE data, int offset){
		// ����Ƿ��ظ�����
		bool found = Search(data, NULL);
		if (true == found)
		{
			return false;
		}
		// ���������Ҷ�ӽ��
		CLeafNode<KEY_TYPE>* pOldNode = SearchLeafNode(data);
		// ���û���ҵ���˵���������ǿյģ����ɸ����
		if (NULL == pOldNode)
		{
			pOldNode = new CLeafNode<KEY_TYPE>();
			m_pLeafHead = pOldNode;
			m_pLeafTail = pOldNode;
			SetRoot(pOldNode);
		}

		// Ҷ�ӽ��δ������Ӧ���1��ֱ�Ӳ���
		if (pOldNode->GetCount() < MAXNUM_DATA)
		{
			if (pOldNode->Insert(data, offset)){
				this->IncLeafCount();
				return true;
			}
			else
				return false;
		}

		// ԭҶ�ӽ���������½�Ҷ�ӽ�㣬����ԭ����һ�����ݼ��е��½��
		CLeafNode<KEY_TYPE>* pNewNode = new CLeafNode<KEY_TYPE>;
		KEY_TYPE key;
		key = pOldNode->Split(pNewNode);

		// ��˫�������в�����
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


		// �ж��ǲ��뵽ԭ��㻹���½���У�ȷ���ǰ�����ֵ�����
		if (data < key)
		{
			pOldNode->Insert(data, offset);    // ����ԭ���
		}
		else
		{
			pNewNode->Insert(data, offset);    // �����½��
		}

		// �����
		CInternalNode<KEY_TYPE>* pFather = (CInternalNode<KEY_TYPE>*)(pOldNode->GetFather());

		// ���ԭ����Ǹ��ڵ㣬��Ӧ���2
		if (NULL == pFather)
		{
			CNode<KEY_TYPE>* pNode1 = new CInternalNode<KEY_TYPE>;
			pNode1->SetPointer(1, pOldNode);                           // ָ��1ָ��ԭ���
			pNode1->SetElement(1, key);                                // ���ü�
			pNode1->SetPointer(2, pNewNode);                           // ָ��2ָ���½��
			pOldNode->SetFather(pNode1);                               // ָ�������
			pNewNode->SetFather(pNode1);                               // ָ�������
			pNode1->SetCount(1);

			SetRoot(pNode1);                                           // ָ���µĸ����
			this->IncLeafCount();
			return true;
		}

		// ���3�����4������ʵ��
		bool ret = InsertInternalNode(pFather, key, pNewNode);
		if (ret){
			this->IncLeafCount();
			return true;
		}
		else
			return false;

	};
    // ɾ��ָ��������
	bool Delete(KEY_TYPE data){
		// ���������Ҷ�ӽ��
		CLeafNode<KEY_TYPE>* pOldNode = SearchLeafNode(data);
		// ���û���ҵ�������ʧ��
		if (NULL == pOldNode)
		{
			return false;
		}

		// ɾ�����ݣ����ʧ��һ����û���ҵ���ֱ�ӷ���ʧ��
		bool isFirstNode = (pOldNode->GetElement(1) == data);
		bool success = pOldNode->Delete(data);

		if (false == success)
		{
			return false;
		}

		// ��ȡ�����
		CInternalNode<KEY_TYPE>* pFather = (CInternalNode<KEY_TYPE>*)(pOldNode->GetFather());
		if (NULL == pFather)
		{
			// ���һ�����ݶ�û���ˣ�ɾ�������(ֻ�и��ڵ���ܳ��ִ����)
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
		// ���ɾ�����Ǹ����ļ�ֵ����Ҫ���ĸü�
		bool breakFlag = true;
		while (reFather && breakFlag && isFirstNode)
		{
			if (data >= reFather->GetElement(1)){//������ڸ��ڵ�ĵ�һ��ֵ�����ֵһ����������ڸ��ڵ������
				for (int i = 1; (i <= reFather->GetCount()) && (data >= reFather->GetElement(i)); i++)
				{

					if (reFather->GetElement(i) == data)
					{
						reFather->SetElement(i, pOldNode->GetElement(1));    // ����ΪҶ�ӽ���µĵ�һ��Ԫ��
						breakFlag = false;
					}
				}
			}
			else{//ɾ���ڵ�С�ڸ��ڵ�ֵ����ֵ���ܳ������游�ڵ���
				reFather = (CInternalNode<KEY_TYPE>*)reFather->GetFather();
			}
		}
		// ɾ����Ҷ�ӽ��������>=50%����Ӧ���1
		if (pOldNode->GetCount() >= ORDER_V)
		{
			this->DecLeafCount();
			return true;
		}
		// �ҵ�һ��������ֵܽ��(�ڱ��ڵ㲻�����ڵ������£�ѡ�����ֵܽڵ㣬����B+���Ķ��壬����Ҷ�ӽ�㣬�������ҵ���)
		int flag = BPT_FLAG_LEFT;
		CLeafNode<KEY_TYPE>* pBrother = (CLeafNode<KEY_TYPE>*)(pOldNode->GetBrother(flag));

		// �ֵܽ������>50%����Ӧ���2A
		KEY_TYPE NewData = NULL;
		int NewDataOffset = NULL;
		if (pBrother->GetCount() > ORDER_V)
		{
			if (BPT_FLAG_LEFT == flag)    // �ֵ�����ߣ������һ�����ݹ���
			{
				NewData = pBrother->GetElement(pBrother->GetCount());
				NewDataOffset = pBrother->GetElementOffset(pBrother->GetCount());
			}
			else    // �ֵ����ұߣ��Ƶ�һ�����ݹ���
			{
				NewData = pBrother->GetElement(1);
				NewDataOffset = pBrother->GetElementOffset(1);
			}

			pOldNode->Insert(NewData, NewDataOffset);
			pBrother->Delete(NewData);

			// �޸ĸ����ļ�ֵ
			if (BPT_FLAG_LEFT == flag)
			{
				for (int i = 1; i <= pFather->GetCount() + 1; i++)
				{
					if (pFather->GetPointer(i) == pOldNode && i > 1)
					{
						pFather->SetElement(i - 1, pOldNode->GetElement(1));    // ���ı�����Ӧ�ļ�
					}
				}
			}
			else
			{
				for (int i = 1; i <= pFather->GetCount() + 1; i++)
				{
					if (pFather->GetPointer(i) == pOldNode && i > 1)
					{
						pFather->SetElement(i - 1, pOldNode->GetElement(1));    // ���ı�����Ӧ�ļ�
					}
					if (pFather->GetPointer(i) == pBrother && i > 1)
					{
						pFather->SetElement(i - 1, pBrother->GetElement(1));    // �����ֵܽ���Ӧ�ļ�
					}
				}
			}

			this->DecLeafCount();
			return true;
		}

		// ���2B

		// �������Ҫɾ���ļ�
		KEY_TYPE NewKey = NULL;

		// �ѱ�������ֵܽ��ϲ���������κϲ������ݽ�С�Ľ�㣬���������������޸�ָ��

		if (BPT_FLAG_LEFT == flag)
		{
			(void)pBrother->Combine(pOldNode);
			NewKey = pOldNode->GetElement(1);

			CLeafNode<KEY_TYPE>* pOldNext = pOldNode->m_pNextNode;
			pBrother->m_pNextNode = pOldNext;
			// ��˫��������ɾ�����
			if (NULL == pOldNext)
			{
				m_pLeafTail = pBrother;
			}
			else
			{
				pOldNext->m_pPrevNode = pBrother;
			}
			// ɾ�������
			delete pOldNode;
		}
		else
		{
			(void)pOldNode->Combine(pBrother);
			NewKey = pBrother->GetElement(1);

			CLeafNode<KEY_TYPE>* pOldNext = pBrother->m_pNextNode;
			pOldNode->m_pNextNode = pOldNext;
			// ��˫��������ɾ�����
			if (NULL == pOldNext)
			{
				m_pLeafTail = pOldNode;
			}
			else
			{
				pOldNext->m_pPrevNode = pOldNode;
			}
			// ɾ�������
			delete pBrother;
		}

		if (DeleteInternalNode(pFather, NewKey)){
			this->DecLeafCount();
			return true;
		}
		else
			return false;
	};

    // �����
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

    // ��ӡ��
	void PrintTree(){
		CNode<KEY_TYPE>* pRoot = GetRoot();
		if (NULL == pRoot) return;

		CNode<KEY_TYPE>* p1, *p2, *p3;
		int i, j, k;
		int total = 0;

		printf("\n��һ��\n | ");
		PrintNode(pRoot);
		total = 0;
		printf("\n�ڶ���\n | ");
		for (i = 1; i <= MAXNUM_POINTER; i++)
		{
			p1 = pRoot->GetPointer(i);
			if (NULL == p1) continue;
			PrintNode(p1);
			total++;
			if (total % 4 == 0) printf("\n | ");
		}
		total = 0;
		printf("\n������\n | ");
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
		printf("\n���Ĳ�\n | ");
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


    // ������Ƿ�����B+���Ķ���
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
			for (int i = 1; i<pThisNode->GetCount(); i++){//ÿ���ڵ��ڵ�ֵ����С��������
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

    // �ݹ����㼰�������Ƿ�����B+���Ķ���
	bool CheckNode(CNode<KEY_TYPE>* pNode){
		if (NULL == pNode)
		{
			return true;
		}

		int i = 0;
		bool ret = false;

		// ����Ƿ�����50%������
		if ((pNode->GetCount() < ORDER_V) && (pNode->GetCount() > 0) && (pNode != GetRoot()))
		{
			return false;
		}

		// �����������Ƿ񰴴�С����
		for (i = 1; i < pNode->GetCount(); i++)
		{
			if (pNode->GetElement(i) > pNode->GetElement(i + 1))
			{
				return false;
			}
		}

		//������нڵ�ĸ��ڵ㣬���ڵ�����������غ��
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
		//�����Ҷ�ӽڵ㣬�����ϣ�������ȷֵ
		if (pNode->GetType() == NODE_TYPE_LEAF)
		{
			return true;
		}

		//�鿴�м�ڵ�������ֵ�Ƿ���Ҷ�ӽڵ���֣����û�г��֣�˵������ʧ��
		for (int i = 1; i <= pNode->GetCount(); i++){
			KEY_TYPE internalVal = pNode->GetElement(i);
			CNode<KEY_TYPE> * node = pNode->GetPointer(i + 1);
			while (node->GetType() != NODE_TYPE_LEAF){
				node = node->GetPointer(1);
			}
			if (internalVal != node->GetElement(1))
				return false;
		}

		//�鿴�м�ڵ��е�ֵ��ָ���ǲ���n=m+1�Ĺ�ϵ
		for (int i = 1; i <= MAXNUM_KEY; i++){
			if ((i <= pNode->GetCount() + 1) && pNode->GetPointer(i) == NULL)
				return false;
			if ((i > pNode->GetCount() + 1) && pNode->GetPointer(i) != NULL)
				return false;
		}



		// ���м��㣬�ݹ�������
		for (i = 1; i <= pNode->GetCount() + 1; i++)
		{
			ret = CheckNode(pNode->GetPointer(i));
			// ֻҪ��һ�����Ϸ��ͷ��ز��Ϸ�
			if (false == ret)
			{
				return false;
			}
		}

		return true;

	};

    // ��ȡ�����ø����
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

    // ��ȡ���������
    int GetDepth()
    {
        return m_Depth;
    }

    void SetDepth(int depth)
    {
        m_Depth = depth;
    }
   
    // ��ȼ�һ
    void IncDepth()
    {
        m_Depth = m_Depth + 1;
    }

    // ��ȼ�һ
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
		Count += MaxPos;//���ұտռ�
		return Count;
	}


public:
    // ����������������ʵ��˫������
	CLeafNode<KEY_TYPE> *m_pLeafHead;                 // ͷ���
	CLeafNode<KEY_TYPE>* m_pLeafTail;                   // β���

protected:

	CLeafNode<KEY_TYPE>* GetCleafNodeFromKey(KEY_TYPE data, int &pos){
		int i = 0;
		CNode<KEY_TYPE> * pNode = GetRoot();
		// ѭ�����Ҷ�Ӧ��Ҷ�ӽ��
		while (NULL != pNode)
		{
			// ���ΪҶ�ӽ�㣬ѭ������
			if (NODE_TYPE_LEAF == pNode->GetType())
			{
				break;
			}

			// �ҵ���һ����ֵ���ڵ���key��λ��
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

    // Ϊ���������Ҷ�ӽ��
	CLeafNode<KEY_TYPE>* SearchLeafNode(KEY_TYPE data){
		int i = 0;

		CNode<KEY_TYPE> * pNode = GetRoot();
		// ѭ�����Ҷ�Ӧ��Ҷ�ӽ��
		while (NULL != pNode)
		{
			// ���ΪҶ�ӽ�㣬ѭ������
			if (NODE_TYPE_LEAF == pNode->GetType())
			{
				break;
			}

			// �ҵ���һ����ֵ���ڵ���key��λ��
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
    //��������м���
	bool InsertInternalNode(CInternalNode<KEY_TYPE>* pNode, KEY_TYPE key, CNode<KEY_TYPE>* pRightSon){
		if (NULL == pNode || NODE_TYPE_LEAF == pNode->GetType())
		{
			return false;
		}

		// ���δ����ֱ�Ӳ���
		if (pNode->GetCount() < MAXNUM_KEY)
		{
			return pNode->Insert(key, pRightSon);
		}

		CInternalNode<KEY_TYPE>* pBrother = new CInternalNode<KEY_TYPE>();
		KEY_TYPE NewKey;
		// ���ѱ��м���
		NewKey = pNode->Split(pBrother, key);

		if (pNode->GetCount() < pBrother->GetCount())
		{
			pNode->Insert(key, pRightSon);
		}
		else if (pNode->GetCount() > pBrother->GetCount())
		{
			pBrother->Insert(key, pRightSon);
		}
		else    // ������ȣ�����ֵ�ڵ�V��V+1����ֵ�м����������ֽڵ�ҵ��½��ĵ�һ��ָ����
		{
			for (int i = pBrother->GetCount(); i >= 1; i--){
				pBrother->SetPointer(i + 1, pBrother->GetPointer(i));
			}
			pBrother->SetPointer(1, pRightSon);
			pRightSon->SetFather(pBrother);
		}

		CInternalNode<KEY_TYPE>* pFather = (CInternalNode<KEY_TYPE>*)(pNode->GetFather());
		// ֱ������㶼���ˣ������ɸ����
		if (NULL == pFather)
		{
			pFather = new CInternalNode<KEY_TYPE>;
			pFather->SetPointer(1, pNode);                           // ָ��1ָ��ԭ���
			pFather->SetElement(1, NewKey);                          // ���ü�
			pFather->SetPointer(2, pBrother);                        // ָ��2ָ���½��
			pNode->SetFather(pFather);                               // ָ�������
			pBrother->SetFather(pFather);                            // ָ�������
			pFather->SetCount(1);
			SetRoot(pFather);                                        // ָ���µĸ����
			IncDepth();
			return true;
		}

		// �ݹ�
		return InsertInternalNode(pFather, NewKey, pBrother);
	};
    // ���м�����ɾ����
	bool DeleteInternalNode(CInternalNode<KEY_TYPE>* pNode, KEY_TYPE key){
		// ɾ���������ʧ��һ����û���ҵ���ֱ�ӷ���ʧ��
		bool success = pNode->Delete(key);
		if (false == success)
		{
			return false;
		}

		// ��ȡ�����
		CInternalNode<KEY_TYPE>* pFather = (CInternalNode<KEY_TYPE>*)(pNode->GetFather());
		if (NULL == pFather)
		{
			// ���һ�����ݶ�û���ˣ��Ѹ����ĵ�һ�������Ϊ�����
			if (0 == pNode->GetCount())
			{
				SetRoot(pNode->GetPointer(1));
				pNode->GetPointer(1)->SetFather(NULL);
				delete pNode;
			}

			return true;
		}

		// ɾ������������>=50%
		if (pNode->GetCount() >= ORDER_V)
		{
			return true;
		}

		// �ҵ�һ��������ֵܽ��(����B+���Ķ��壬���˸���㣬�������ҵ���)
		int flag = BPT_FLAG_LEFT;
		CInternalNode<KEY_TYPE>* pBrother = (CInternalNode<KEY_TYPE>*)(pNode->GetBrother(flag));

		// �ֵܽ������>50%
		KEY_TYPE NewData = NULL;
		if (pBrother->GetCount() > ORDER_V)
		{
			pNode->MoveOneElement(pBrother);
			return true;
		}

		// �������Ҫɾ���ļ�
		KEY_TYPE NewKey = NULL;

		// �ѱ�������ֵܽ��ϲ���������κϲ������ݽ�С�Ľ�㣬���������������޸�ָ��
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

		// �ݹ�
		return DeleteInternalNode(pFather, NewKey);
	};
   
	CNode<KEY_TYPE>* m_Root;    // �����
    int m_Depth;      // �������
	int leafCount;	 //����Ҷ�ӽڵ������
};

/* end of BPlusTree.h */