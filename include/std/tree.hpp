/*************************************************************************
    > File Name: tree_print.cpp
    > Author:
    > Mail:
    > Created Time: 2016��04��05�� ���ڶ� 09ʱ10��02��
 ************************************************************************/

#include <iostream>
using namespace std;

#define TYPE char

//���Ľڵ�
struct TreeNode {
  TYPE element;//�ýڵ��Ԫ��
  TreeNode *firstChild;//ָ��ýڵ�ĵ�һ������
  TreeNode *next;//ָ��ýڵ���ֵܽڵ�

};

class Tree {
public:
  Tree(TreeNode * r = NULL) :root(r) {}
  Tree(int node_num);
  ~Tree();

  void addNode(int i, int j);
  void preOrder();//ǰ�����
  void print();//��ӡ
private:
  void print(TreeNode* node, int num);
  void addBrotherNode(TreeNode* bro, TreeNode* node);
  void preOrder(TreeNode* parent);//ǰ�����
private:
  TreeNode * root;//�����ĸ�
};

//��ӡ������״
void Tree::print()
{
  print(root, 0);
}

void printSpace(int num)
{
  int i = 0;
  for (i = 0; i < num - 3; i++)
    cout << " ";

  for (; i < num - 2; ++i)
    cout << "|";
  for (; i < num; ++i)
    cout << "_";
}


void Tree::print(TreeNode* node, int num)
{
  if (node != NULL) {
    printSpace(num);
    cout << node->element << endl;
    print(node->firstChild, num + 4);
    print(node->next, num);
  }
}

//ǰ�����
void Tree::preOrder()
{
  cout << "ǰ�����: ";
  preOrder(root);
  cout << endl;
}

void Tree::preOrder(TreeNode* parent)
{
  if (parent != NULL) {
    cout << parent->element << " ";
    preOrder(parent->firstChild);
    preOrder(parent->next);
  }
}

//���䲢��ʼ�����е������
Tree::Tree(int node_num)
{
  root = new TreeNode[node_num];

  char ch = 'A';

  for (int i = 0; i < node_num; ++i) {
    root[i].element = ch + i;
    root[i].firstChild = NULL;
    root[i].next = NULL;
  }

}

//�ͷ����нڵ���ڴ�ռ�
Tree::~Tree()
{
  if (root != NULL)
    delete[] root;
}

//addNode�����ӽ�����
//������ڵ��firstChild==NULL, ��firstChild = node;
//������ڵ��firstChild != NULL, ��
void Tree::addNode(int i, int j)
{
  TreeNode* parent = &root[i];
  TreeNode* node = &root[j];

  if (parent->firstChild == NULL)
    parent->firstChild = node;
  else
    addBrotherNode(parent->firstChild, node);
}

//���ڵ���뵽�ֵܽڵ�
void Tree::addBrotherNode(TreeNode* bro, TreeNode* node)
{
  if (bro->next == NULL)
    bro->next = node;
  else
    addBrotherNode(bro->next, node);
}

//������
int test_tree()
{
  Tree tree(16);//����ʮ�����ڵ�

  tree.addNode(0, 1);
  tree.addNode(0, 2);
  tree.addNode(0, 3);
  tree.addNode(0, 4);
  tree.addNode(0, 5);
  tree.addNode(0, 6);
  tree.addNode(3, 7);
  tree.addNode(4, 8);
  tree.addNode(4, 9);
  tree.addNode(5, 10);
  tree.addNode(5, 11);
  tree.addNode(5, 12);
  tree.addNode(6, 13);
  tree.addNode(9, 14);
  tree.addNode(9, 15);

  cout << "Tree1: " << endl;
  tree.preOrder();
  tree.print();

  Tree tree2(9);
  tree2.addNode(0, 1);
  tree2.addNode(0, 2);
  tree2.addNode(1, 3);
  tree2.addNode(1, 4);
  tree2.addNode(2, 5);
  tree2.addNode(3, 6);
  tree2.addNode(5, 7);
  tree2.addNode(5, 8);

  cout << "Tree2: " << endl;
  tree2.preOrder();
  tree2.print();

  return 0;
}
