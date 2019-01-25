/*************************************************************************
> File Name: tree_print.cpp
> Author:
> Mail:
> Created Time: 2016��04��05�� ���ڶ� 09ʱ10��02��
************************************************************************/

#include <stdio.h>

#define TYPE char

//���Ľڵ� 
//�������ֵܡ���ʾ��
struct TreeNode {
  TYPE element;//�ýڵ��Ԫ��
  TreeNode *firstChild;//ָ��ýڵ�ĵ�һ������
  TreeNode *next;//ָ��ýڵ���ֵܽڵ�
};
void printSpace(int level)
{
  int i = 0;
  for (i = 0; i < level - 3; i++)
    printf(" ");

  for (; i < level - 2; ++i)
    printf("|");
  for (; i < level; ++i)
    printf("_");
}
typedef void (*printNode_t)(TreeNode* node);
void print(TreeNode* node, printNode_t printNode, int level)
{
  if (node != NULL) {
    printSpace(level);
    printNode(node);
    printf("\n");
    print(node->firstChild, printNode, level + 4);
    print(node->next, printNode, level);
  }
}

//ǰ�����
void preOrder(TreeNode* parent, printNode_t printNode)
{
  if (parent != NULL) {
    printNode(parent);
    preOrder(parent->firstChild, printNode);
    preOrder(parent->next, printNode);
  }
}

typedef void(*freeTree_t)(TreeNode* root);
//�ͷ����нڵ���ڴ�ռ�
void freeTree(TreeNode* root, freeTree_t free_)
{
  if (root != NULL) {
    free_(root);
    freeTree(root->firstChild, free_);
    freeTree(root->next, free_);
    root = NULL;
  }
}

//���ڵ���뵽�ֵܽڵ�
void addLastBrotherNode(TreeNode* bro, TreeNode* node)
{
  if (bro->next == NULL) {
    bro->next = node;
  }
  else {
    addLastBrotherNode(bro->next, node);
  }
}

//addNode�����ӽ�����
//������ڵ��firstChild==NULL, ��firstChild = node;
//������ڵ��firstChild != NULL, ��
void addLastChildNode(TreeNode* parent, TreeNode* node)
{
  if (parent->firstChild == NULL) {
    parent->firstChild = node;
  }
  else {
    addLastBrotherNode(parent->firstChild, node);
  }
}

void addFirstChildNode(TreeNode* parent, TreeNode* node)
{
  if (parent->firstChild) {
    node->next = parent->firstChild->next;
  }
  parent->firstChild = node;
}
struct testTreeNode {
  char data;
  TreeNode node[1];
};
//���䲢��ʼ�����е������
void initTree(testTreeNode* root, int node_num)
{
  char ch = 'A';

  for (int i = 0; i < node_num; ++i) {
    root[i].data = ch + i;
    root[i].node->firstChild = NULL;
    root[i].node->next = NULL;
  }
}

void printNode(TreeNode* p) {
  testTreeNode* b = container_of(p, testTreeNode, node);
  printf("%c", b->data);
}

//������
int test_tree_c()
{
  testTreeNode tree[16] = { 0 };//����ʮ�����ڵ�
  initTree(tree, countof(tree));
  testTreeNode* root = NULL;
  root = tree;
#define addNode(i, j)  addLastChildNode(root[i].node, root[j].node)
  addNode(0, 1);
  addNode(0, 2);
  addNode(0, 3);
  addNode(0, 4);
  addNode(0, 5);
  addNode(0, 6);
  addNode(3, 7);
  addNode(4, 8);
  addNode(4, 9);
  addNode(5, 10);
  addNode(5, 11);
  addNode(5, 12);
  addNode(6, 13);
  addNode(9, 14);
  addNode(9, 15);

  printf("Tree1: \n");
  preOrder(root->node, printNode);
  print(root->node, printNode, 0);

  testTreeNode tree2[9] = { 0 };//����ʮ�����ڵ�
  initTree(tree2, countof(tree2));
  root = tree2;
  addNode(0, 1);
  addNode(0, 2);
  addNode(1, 3);
  addNode(1, 4);
  addNode(2, 5);
  addNode(3, 6);
  addNode(5, 7);
  addNode(5, 8);

  printf("Tree2: \n");
  preOrder(root->node, printNode);
  print(root->node, printNode, 0);
#undef addNode
  return 0;
}
