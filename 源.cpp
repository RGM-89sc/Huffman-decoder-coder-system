#include <fstream>
#include <stdio.h>
#include <iostream>

typedef struct char_count * count_node;
struct char_count {
	char ch;  // �ַ�
	long count;  // ���ֵĴ���
	short bit;  // ����ı���
};
count_node weight[126];  // ������¼�����ַ���Ȩ�غ��ڹ�������������ڵ㱻���䵽�ı���

typedef struct HNodeType * HNode;
struct HNodeType {
	count_node node; // �ڵ�
	HNode parent; // ���ڵ�
	HNode lchild;  // ����
	HNode rchild;  // �Һ���
};
HNode p;  // ������¼����������������Ҳ�Ǹ�����ָ��

typedef struct charCode * code;
struct charCode {
	char ch;  // �ַ�
	short codeNum[1000];  // ����
	int codeLen;  // ����ĳ���
};
code char_code[125];  // ������¼���ַ��������Ϣ

int n, num;  // ���벻ͬ�ַ��ĸ�����num��ı䣬����n��������num��ֵ
int sumStrLen = 0;  // ����ǰ�ַ����ĳ���
char str[100000];  // ����ǰ���ַ���
short codeStr[100000000];  // �����ı���

using namespace std;

/**
* ����
*/
void tranCode(int len) {
	HNode q;
	q = p;
	cout << "\n��������" << endl;
	for (int i = 0; i < len; i++) {
		if (codeStr[i] == 0) {
			q = q->lchild;  // ����������
			if (q->node->ch) {
				cout << q->node->ch;
				q = p;
			}
		}
		if (codeStr[i] == 1) {
			q = q->rchild;  // ����������
			if (q->node->ch) {
				cout << q->node->ch;
				q = p;
			}
		}
	}
	cout << endl;
}

/**
* ����ѹ��Ч��
*/
void efficiency(int len) {
	int sumCodeLen = 0;
	for (int i = 0; i <= len; i++) {
		sumCodeLen += char_code[i]->codeLen;
	}
	cout << "\nѹ��Ч�ʣ�" << sumCodeLen << "/" << sumStrLen << "=" << sumCodeLen * 1.0 / sumStrLen << endl;
}

/**
* ���ַ������룬���ر��������ĳ���
*/
int finishCode() {
	int m = 0;
	cout << "\n��������" << endl;
	for (int i = 0; i < sumStrLen; i++) {
		for (int j = 0; j < sumStrLen; j++) {
			if (char_code[j]->ch == str[i]) {
				for (int k = 0; k < char_code[j]->codeLen; k++) {
					codeStr[m] = char_code[j]->codeNum[k];
					cout << codeStr[m];
					m++;
				}
				break;
			}
		}
	}
	cout << endl;
	return m;
}

/**
* ��ȡ�����ַ��ı���
*/
void getCode() {
	// ��ʼ��
	for (int i = 0; i < 125; i++) {
		char_code[i] = new charCode();
		for (int j = 0; j < 1000; j++) {
			char_code[i]->codeNum[j] = -1;
		}
	}

	int cache[1000] = { 0 }, i = 0, j = 0;
	HNode q1, q2;
	q1 = p;
	// ������Ҷ��
	while (q1->lchild) {
		char_code[i]->ch = q1->lchild->node->ch;
		j = 0;
		cache[j] = q1->lchild->node->bit;
		j++;
		q2 = q1;
		while (q2->node->bit) {
			cache[j] = q2->node->bit;
			j++;
			q2 = q2->parent;
		}
		int m = 0;
		for (int k = j - 1; k >= 0; k--) {
			char_code[i]->codeNum[m] = cache[k];
			m++;
		}
		char_code[i]->codeLen = m;
		i++;
		if (q1->rchild) {
			q1 = q1->rchild;
		}
		else {
			break;
		}
	}
	// ��Ҷ��
	q1 = p;
	if (q1->rchild) {
		j = 0;
		int cache2[1000] = { 0 };
		while (q1->rchild) {
			cache[j] = q1->rchild->node->bit;
			j++;
			if (q1->rchild->node->ch) {
				char_code[i]->ch = q1->rchild->node->ch;
			}
			q1 = q1->rchild;
		}
		int m = 0;
		for (int k = j - 1; k >= 0; k--) {
			char_code[i]->codeNum[m] = cache[k];
			m++;
		}
		char_code[i]->codeLen = m;
	}

	// output
	cout << "\n�ó����ַ��ı��룺" << endl;
	for (int k = 0; k <= i; k++) {
		if (char_code[k]->ch) {
			cout << "char: " << char_code[k]->ch << " code: ";
			int k2 = 0;
			while (char_code[k]->codeNum[k2] != -1) {
				cout << char_code[k]->codeNum[k2];
				k2++;
			}
		}
		cout << endl;
	}

	// ���ַ������б���
	int codeLen = finishCode();

	// ͳ��ѹ��Ч��
	efficiency(i);

	// ����
	tranCode(codeLen);
}

/**
* �������
*/
void coder(HNode p) {
	if (p) {
		if (p->lchild) {
			p->lchild->node->bit = 0;
		}
		if (p->rchild) {
			p->rchild->node->bit = 1;
		}
		if (p->lchild) {
			coder(p->lchild);
		}
		if (p->rchild) {
			coder(p->rchild);
		}
	}
}

/**
* ǰ�������������
*/
void outputHafftree(HNode p) {
	if (p) {
		// root
		if (p->node->ch != NULL) {
			cout << "�ַ�: " << p->node->ch << " Ȩ�أ�" << p->node->count << endl;
		}
		else if (p->node->count) {
			cout << "[�����ڵ�] Ȩ�أ�" << p->node->count << endl;
		}
		// left
		if (p->lchild) {
			outputHafftree(p->lchild);
		}
		// right
		if (p->rchild) {
			outputHafftree(p->rchild);
		}
	}
}

/**
* ������������
*/
void HaffmanTree() {
	HNode hNl, hNr;
	hNl = new HNodeType();
	hNr = new HNodeType();
	hNl->lchild = hNl->rchild = NULL;  // �ýڵ㣨����Ϊ���ӣ�������������ΪNULL
	hNl->node = weight[1];  // �ýڵ㣨����Ϊ���ӣ��ĺ��Ľڵ���Ϊweight��δʹ�õ���Сcount�Ľڵ�

	hNr->lchild = hNr->rchild = NULL;  // �ýڵ㣨����Ϊ���ӣ�������������ΪNULL

	if (n == 1) {  // ֻ��һ����ͬ�ַ������
		hNr->node = NULL;  // û���Һ��ӣ��Һ�����NULL

		p = hNl->parent = new HNodeType();  // ���Ӻ��Һ��ӵĸ�����Ϊһ���µĽڵ㣬��ʹ����ָ��pָ����

		p->lchild = hNl;  // �½ڵ��������Ϊ�ɵ��������
		p->rchild = NULL;  // �½ڵ���Һ�����ΪNULL
		p->node = new char_count();  // ���¸��׷���ռ�
		p->node->ch = NULL;  // �����ĸ��׵ĺ��Ľڵ��ch��NULL
		p->node->count = p->lchild->node->count;  // ����Ȩ��Ϊ��������Ȩ�صĺ�
		p->parent = NULL;  // �ÿչ����������ڵ��еĸ��׽ڵ�
	}
	else {
		hNr->node = weight[2];  // �ýڵ㣨����Ϊ�Һ��ӣ��ĺ��Ľڵ���Ϊweight��δʹ�õ���Сcount�Ľڵ�

		for (int i = 3; i <= n; i++) {
			p = hNl->parent = hNr->parent = new HNodeType();  // ���Ӻ��Һ��ӵĸ�����Ϊһ���µĽڵ㣬��ʹ����ָ��pָ����

			p->lchild = hNl;  // �½ڵ��������Ϊ�ɵ��������
			p->rchild = hNr;  // �½ڵ���Һ�����Ϊ�ɵ�����Һ���
			p->node = new char_count();  // ���¸��׷���ռ�
			p->node->ch = NULL;  // �����ĸ��׵ĺ��Ľڵ��ch��NULL
			p->node->count = p->lchild->node->count + p->rchild->node->count;  // ����Ȩ��Ϊ��������Ȩ�صĺ�

			hNl = new HNodeType();  // ���ɵ����ӣ����ٴγ�Ϊһ�����ӣ����·���ռ�
			hNl->node = weight[i];  // �ɵ����ӣ����ٴγ�Ϊһ�����ӣ��ĺ��Ľڵ���Ϊweight��δʹ�õ���Сcount�Ľڵ�
			hNl->lchild = hNl->rchild = NULL;  // �ɵ����ӣ����ٴγ�Ϊһ�����ӣ�������������ΪNULL

			hNr = p;  // �ɵ��Һ��ӳ�Ϊ����
		}
		// ���ɹ��������ĸ��ڵ�
		p = hNl->parent = hNr->parent = new HNodeType();  // ���Ӻ��Һ��ӵĸ�����Ϊһ���µĽڵ㣬��ʹ����ָ��pָ����

		p->lchild = hNl;  // �½ڵ��������Ϊ�ɵ��������
		p->rchild = hNr;  // �½ڵ���Һ�����Ϊ�ɵ�����Һ���
		p->node = new char_count();  // ���¸��׷���ռ�
		p->node->ch = NULL;  // �����ĸ��׵ĺ��Ľڵ��ch��NULL
		p->node->count = p->lchild->node->count + p->rchild->node->count;  // ����Ȩ��Ϊ��������Ȩ�صĺ�
		p->parent = NULL;  // �ÿչ����������ڵ��еĸ��׽ڵ�
	}
	// ��ӡ
	cout << "\nǰ�������������:" << endl;
	outputHafftree(p);

	//�������
	coder(p);
}

/**
* ������������Ԫ��
*
* @param {int} x ��Ҫ����λ�õ�Ԫ��֮һ���±�
* @param {int} y ��Ҫ����λ�õ�Ԫ��֮һ���±�
*/
void swap(int x, int y) {
	count_node t;
	t = weight[x];
	weight[x] = weight[y];
	weight[y] = t;
}

/**
* ���µ���
*
* @param {int} i ��Ҫ���µ�����Ԫ�ص��±�
*/
void siftdown(int i) {
	int t, flag = 0;
	while (i * 2 <= num && flag == 0) {
		if (weight[i]->count < weight[i * 2]->count) t = i * 2;
		else t = i;
		if (i * 2 + 1 <= num) 
			if (weight[t]->count < weight[i * 2 + 1]->count) t = i * 2 + 1;
		if (t != i) {
			swap(t, i);
			i = t;
		}
		else flag = 1;
	}
}

/**
* ������
*/
void creat() {
	for (int i = num / 2; i >= 1; i--) siftdown(i);
}

/**
* ������
*/
void heapsort() {
	while (num > 1) {
		swap(1, num);
		num--;
		siftdown(1);
	}
}

/**
* ����Ȩ�ص͵��ȳ������ȶ��У���С�ѣ�
*/
void priority_queue() {
	creat();
	heapsort();
	cout << "\n������С�ѣ�Ȩ��С���ȳ�����" << endl;
	for (int i = 1; i <= n; i++) {
		cout << weight[i]->ch << " Ȩ�أ�" << weight[i]->count << endl;
	}
}

/**
* �����û�������ַ�������ͳ��ÿ���ַ��ֱ���ֵĴ���
*/
 void get_str_weight() {
	 long count[126] = { 0 };
	 char c;
	 getchar();
	 cout << "�������ַ������б���/���룺" << endl;
	 while ((c = getchar()) != '\n') {
		 count[c - 32]++;
		 str[sumStrLen] = c;
		 sumStrLen++;
	 }
	 sumStrLen *= 8;
	 int j = 1;
	 for (int i = 1; i < 126; i++) {
		 if (count[i] > 0) {
			 weight[j]->ch = i + 32;
			 weight[j]->count = count[i];
			 cout << weight[j]->ch << " : ���� " << weight[j]->count << " ��" << endl;
			 j++;
			 num++;
		 }
	 }
	 n = num;
}

 /**
 * ͳ�ƴ��ļ����л�ȡ���ַ�����ÿ���ַ����ֵĴ���
 */
 void get_str_weight2() {
	 getchar();
	 int i = 0;
	 long count[126] = { 0 };
	 while (str[i] != '\0') {
		 count[str[i] - 32]++;
		 sumStrLen++;
		 i++;
	 }
	 sumStrLen *= 8;
	 int j = 1;
	 for (int i = 1; i < 126; i++) {
		 if (count[i] > 0) {
			 weight[j]->ch = i + 32;
			 weight[j]->count = count[i];
			 cout << weight[j]->ch << " : ���� " << weight[j]->count << " ��" << endl;
			 j++;
			 num++;
		 }
	 }
	 n = num;
 }

 /**
 * ��ȡ�ļ��е������ַ�
 */
 void get_file() {
	 char path[1000];
	 cout << "�������ļ�·����";
	 cin >> path;
	 // �Զ�ģʽ���ļ�
	 ifstream fin(path);
	 fin >> str;
 }

 /**
 * [1]��ִ������
 */
 void task1() {
	 get_str_weight();
	 priority_queue();
	 HaffmanTree();
	 getCode();
 }

 /**
 * [2]��ִ������
 */
 void task2() {
	 get_file();
	 get_str_weight2();
	 priority_queue();
	 HaffmanTree();
	 getCode();
 }

int main() {
	// ��ʼ��
	for (int i = 0; i < 126; i++) {
		weight[i] = new char_count();
	}
	cout << "[1] ����̨�����ַ������й���������/����\n[2] ѡ��һ���ļ����й���������/����" << endl;
	char choose = getchar();
	switch (choose) {
		case '1': task1();
		break;
		case '2': task2();
		break;
	}
	return 0;
}
