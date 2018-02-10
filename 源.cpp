#include <fstream>
#include <stdio.h>
#include <iostream>

typedef struct char_count * count_node;
struct char_count {
	char ch;  // 字符
	long count;  // 出现的次数
	short bit;  // 分配的比特
};
count_node weight[126];  // 用来记录各个字符的权重和在哈夫曼树中这个节点被分配到的比特

typedef struct HNodeType * HNode;
struct HNodeType {
	count_node node; // 节点
	HNode parent; // 父节点
	HNode lchild;  // 左孩子
	HNode rchild;  // 右孩子
};
HNode p;  // 用来记录哈夫曼树的树根，也是个工作指针

typedef struct charCode * code;
struct charCode {
	char ch;  // 字符
	short codeNum[1000];  // 编码
	int codeLen;  // 编码的长度
};
code char_code[125];  // 用来记录各字符编码的信息

int n, num;  // 输入不同字符的个数，num会改变，所以n用来保存num的值
int sumStrLen = 0;  // 编码前字符串的长度
char str[100000];  // 编码前的字符串
short codeStr[100000000];  // 编码后的比特

using namespace std;

/**
* 译码
*/
void tranCode(int len) {
	HNode q;
	q = p;
	cout << "\n译码结果：" << endl;
	for (int i = 0; i < len; i++) {
		if (codeStr[i] == 0) {
			q = q->lchild;  // 进入左子树
			if (q->node->ch) {
				cout << q->node->ch;
				q = p;
			}
		}
		if (codeStr[i] == 1) {
			q = q->rchild;  // 进入右子树
			if (q->node->ch) {
				cout << q->node->ch;
				q = p;
			}
		}
	}
	cout << endl;
}

/**
* 计算压缩效率
*/
void efficiency(int len) {
	int sumCodeLen = 0;
	for (int i = 0; i <= len; i++) {
		sumCodeLen += char_code[i]->codeLen;
	}
	cout << "\n压缩效率：" << sumCodeLen << "/" << sumStrLen << "=" << sumCodeLen * 1.0 / sumStrLen << endl;
}

/**
* 把字符串编码，返回编码后数组的长度
*/
int finishCode() {
	int m = 0;
	cout << "\n编码结果：" << endl;
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
* 获取各个字符的编码
*/
void getCode() {
	// 初始化
	for (int i = 0; i < 125; i++) {
		char_code[i] = new charCode();
		for (int j = 0; j < 1000; j++) {
			char_code[i]->codeNum[j] = -1;
		}
	}

	int cache[1000] = { 0 }, i = 0, j = 0;
	HNode q1, q2;
	q1 = p;
	// 所有左叶子
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
	// 右叶子
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
	cout << "\n得出各字符的编码：" << endl;
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

	// 把字符串进行编码
	int codeLen = finishCode();

	// 统计压缩效率
	efficiency(i);

	// 译码
	tranCode(codeLen);
}

/**
* 分配比特
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
* 前序遍历哈夫曼树
*/
void outputHafftree(HNode p) {
	if (p) {
		// root
		if (p->node->ch != NULL) {
			cout << "字符: " << p->node->ch << " 权重：" << p->node->count << endl;
		}
		else if (p->node->count) {
			cout << "[新增节点] 权重：" << p->node->count << endl;
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
* 构建哈夫曼树
*/
void HaffmanTree() {
	HNode hNl, hNr;
	hNl = new HNodeType();
	hNr = new HNodeType();
	hNl->lchild = hNl->rchild = NULL;  // 该节点（将成为左孩子）的左右子树置为NULL
	hNl->node = weight[1];  // 该节点（将成为左孩子）的核心节点置为weight中未使用的最小count的节点

	hNr->lchild = hNr->rchild = NULL;  // 该节点（将成为左孩子）的左右子树置为NULL

	if (n == 1) {  // 只有一个不同字符的情况
		hNr->node = NULL;  // 没有右孩子，右孩子置NULL

		p = hNl->parent = new HNodeType();  // 左孩子和右孩子的父亲置为一个新的节点，并使工作指针p指向它

		p->lchild = hNl;  // 新节点的左孩子置为旧的这个左孩子
		p->rchild = NULL;  // 新节点的右孩子置为NULL
		p->node = new char_count();  // 给新父亲分配空间
		p->node->ch = NULL;  // 新增的父亲的核心节点的ch置NULL
		p->node->count = p->lchild->node->count;  // 父亲权重为两个孩子权重的和
		p->parent = NULL;  // 置空哈夫曼树根节点中的父亲节点
	}
	else {
		hNr->node = weight[2];  // 该节点（将成为右孩子）的核心节点置为weight中未使用的最小count的节点

		for (int i = 3; i <= n; i++) {
			p = hNl->parent = hNr->parent = new HNodeType();  // 左孩子和右孩子的父亲置为一个新的节点，并使工作指针p指向它

			p->lchild = hNl;  // 新节点的左孩子置为旧的这个左孩子
			p->rchild = hNr;  // 新节点的右孩子置为旧的这个右孩子
			p->node = new char_count();  // 给新父亲分配空间
			p->node->ch = NULL;  // 新增的父亲的核心节点的ch置NULL
			p->node->count = p->lchild->node->count + p->rchild->node->count;  // 父亲权重为两个孩子权重的和

			hNl = new HNodeType();  // 给旧的左孩子（将再次成为一个左孩子）重新分配空间
			hNl->node = weight[i];  // 旧的左孩子（将再次成为一个左孩子）的核心节点置为weight中未使用的最小count的节点
			hNl->lchild = hNl->rchild = NULL;  // 旧的左孩子（将再次成为一个左孩子）的左右子树置为NULL

			hNr = p;  // 旧的右孩子成为父亲
		}
		// 生成哈夫曼树的根节点
		p = hNl->parent = hNr->parent = new HNodeType();  // 左孩子和右孩子的父亲置为一个新的节点，并使工作指针p指向它

		p->lchild = hNl;  // 新节点的左孩子置为旧的这个左孩子
		p->rchild = hNr;  // 新节点的右孩子置为旧的这个右孩子
		p->node = new char_count();  // 给新父亲分配空间
		p->node->ch = NULL;  // 新增的父亲的核心节点的ch置NULL
		p->node->count = p->lchild->node->count + p->rchild->node->count;  // 父亲权重为两个孩子权重的和
		p->parent = NULL;  // 置空哈夫曼树根节点中的父亲节点
	}
	// 打印
	cout << "\n前序遍历哈夫曼树:" << endl;
	outputHafftree(p);

	//分配比特
	coder(p);
}

/**
* 交换堆中两个元素
*
* @param {int} x 需要交换位置的元素之一的下标
* @param {int} y 需要交换位置的元素之一的下标
*/
void swap(int x, int y) {
	count_node t;
	t = weight[x];
	weight[x] = weight[y];
	weight[y] = t;
}

/**
* 向下调整
*
* @param {int} i 需要向下调整的元素的下标
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
* 建立堆
*/
void creat() {
	for (int i = num / 2; i >= 1; i--) siftdown(i);
}

/**
* 堆排序
*/
void heapsort() {
	while (num > 1) {
		swap(1, num);
		num--;
		siftdown(1);
	}
}

/**
* 构造权重低的先出的优先队列（最小堆）
*/
void priority_queue() {
	creat();
	heapsort();
	cout << "\n建立最小堆（权重小的先出）：" << endl;
	for (int i = 1; i <= n; i++) {
		cout << weight[i]->ch << " 权重：" << weight[i]->count << endl;
	}
}

/**
* 接收用户输入的字符串，并统计每个字符分别出现的次数
*/
 void get_str_weight() {
	 long count[126] = { 0 };
	 char c;
	 getchar();
	 cout << "请输入字符串进行编码/译码：" << endl;
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
			 cout << weight[j]->ch << " : 出现 " << weight[j]->count << " 次" << endl;
			 j++;
			 num++;
		 }
	 }
	 n = num;
}

 /**
 * 统计从文件当中获取的字符串中每个字符出现的次数
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
			 cout << weight[j]->ch << " : 出现 " << weight[j]->count << " 次" << endl;
			 j++;
			 num++;
		 }
	 }
	 n = num;
 }

 /**
 * 获取文件中的所有字符
 */
 void get_file() {
	 char path[1000];
	 cout << "请输入文件路径：";
	 cin >> path;
	 // 以读模式打开文件
	 ifstream fin(path);
	 fin >> str;
 }

 /**
 * [1]的执行任务
 */
 void task1() {
	 get_str_weight();
	 priority_queue();
	 HaffmanTree();
	 getCode();
 }

 /**
 * [2]的执行任务
 */
 void task2() {
	 get_file();
	 get_str_weight2();
	 priority_queue();
	 HaffmanTree();
	 getCode();
 }

int main() {
	// 初始化
	for (int i = 0; i < 126; i++) {
		weight[i] = new char_count();
	}
	cout << "[1] 控制台输入字符串进行哈夫曼编码/译码\n[2] 选择一个文件进行哈夫曼编码/译码" << endl;
	char choose = getchar();
	switch (choose) {
		case '1': task1();
		break;
		case '2': task2();
		break;
	}
	return 0;
}
