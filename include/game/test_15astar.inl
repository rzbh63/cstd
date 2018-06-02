// A*算法：
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include<iostream.h>
#define N 4
typedef struct QNode {
  int data[N][N];
  int ancent; //标记方向左上右下分别为 1234 5为可以任意方向
  int x;
  int y;
  int gone; //是否遍历该节点，0未遍历，1遍历过
  int value; //和目标的状态差=不在位将牌距离和+深度
  int deep;
  struct QNode* father; //存放前一节点在"store"数组中的位置
  struct QNode* next;
} QNode, *QueuePtr;
typedef struct {
  QueuePtr head;
  QueuePtr rear;
} LinkQueue;
int A1[N][N] = {
  {1, 2, 3, 4},
  {5, 10, 6, 8},
  {0, 9, 7, 12},
  {13, 14, 11, 15}
};
int A[N][N] = {
  1, 2, 11, 7,
  5, 6, 0, 3,
  9, 10, 15, 4,
  13, 14, 12, 8,
};
int B[N][N] = {
  {1, 2, 3, 4},
  {5, 6, 7, 8},
  {9, 10, 11, 12},
  {13, 14, 15, 0}
};
int x, y;
QueuePtr min;//存放最小的结点
bool check()
{
  //判断是否有路径，根据初始态和目标态的秩序，若不同为奇数或同为偶数，则无路径
  int temp = A[x][y];
  int i, j, sum2 = 0, sum1 = 0;
  int a[N * N], b[N * N];
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i * N + j] = A[i][j];
    }
  }
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      b[i * N + j] = B[i][j];
    }
  }
  for (i = 0; i < N * N - 1; i++) {
    for (j = i + 1; j < N * N; j++) {
      if (a[i] != temp && a[j] != temp && a[i] > a[j]) {
        sum1++;
      }
    }
  }
  for (i = 0; i < N * N - 1; i++) {
    for (j = i + 1; j < N * N; j++) {
      if (b[i] != temp && b[j] != temp && b[i] > b[j]) {
        sum2++;
      }
    }
  }
  if ((sum1 % 2 == 0 && sum2 % 2 == 1) || (sum1 % 2 == 1 && sum2 % 2 == 0)) {
    return false;
  }
  return true;
}
bool begin_opint()
{
  int i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      if (A[i][j] == 0) {
        x = i;
        y = j;
        return true;
      }
    }
  }
  return false;
}
bool compare(int a[N][N])
{
  int i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      if (a[i][j] != B[i][j]) {
        return false;
      }
    }
  }
  return true;
}
bool moveleft(int a[N][N], QueuePtr* b, int x, int y)
{
  int k, i, j;
  if (y == 0) {
    return false;
  }
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      (*b)->data[i][j] = a[i][j];
    }
  }
  k = (*b)->data[x][y];
  (*b)->data[x][y] = (*b)->data[x][y - 1];
  (*b)->data[x][y - 1] = k;
  (*b)->x = x;
  (*b)->y = y - 1;
  return true;
}
bool moveup(int a[N][N], QueuePtr* b, int x, int y)
{
  int k, i, j;
  if (x == 0) {
    return false;
  }
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      (*b)->data[i][j] = a[i][j];
    }
  }
  k = (*b)->data[x][y];
  (*b)->data[x][y] = (*b)->data[x - 1][y];
  (*b)->data[x - 1][y] = k;
  (*b)->x = x - 1;
  (*b)->y = y;
  return true;
}
bool movedown(int a[N][N], QueuePtr* b, int x, int y)
{
  int k, i, j;
  if (x == N - 1) {
    return false;
  }
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      (*b)->data[i][j] = a[i][j];
    }
  }
  k = (*b)->data[x][y];
  (*b)->data[x][y] = (*b)->data[x + 1][y];
  (*b)->data[x + 1][y] = k;
  (*b)->x = x + 1;
  (*b)->y = y;
  return true;
}
bool moveright(int a[N][N], QueuePtr* b, int x, int y)
{
  int k, i, j;
  if (y == N - 1) {
    return false;
  }
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      (*b)->data[i][j] = a[i][j];
    }
  }
  k = (*b)->data[x][y];
  (*b)->data[x][y] = (*b)->data[x][y + 1];
  (*b)->data[x][y + 1] = k;
  (*b)->x = x;
  (*b)->y = y + 1;
  return true;
}
bool copy(QueuePtr* a)
{
  int i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      (*a)->data[i][j] = A[i][j];
    }
  }
  return true;
}
void output(QueuePtr* p)
{
  int i, j;
  long int n = 0;
  for (; (*p)->father != NULL; (*p) = (*p)->father, n++) {
    for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
        printf(" %d", (*p)->data[i][j]);
      }
      printf("\n");
    }
    printf("\n");
  }
  printf("step is %d\n", n - 1);
}
int getvalue(QueuePtr* p)
{
  int count = 0; //保存距离
  bool test = true; //若已找到一个位置的值则继续找下一个
  //计算不在位的距离和
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      test = true;
      for (int k = 0; k < N; k++) {
        for (int l = 0; l < N; l++) {
          if ((i != (*p)->x || j != (*p)->y) && (*p)->data[i][j] == B[k][l]) {
            count = count + abs(i - k) + abs(j - l);
            test = false;
          }
          if (test == false) {
            break;
          }
        }
        if (test == false) {
          break;
        }
      }
    }
  }
  count = count + (*p)->deep; //加上深度值
  return count;
}
void test_15astar()
{
  QueuePtr closed, p, q;
  LinkQueue open;
  if (!begin_opint()) {
    printf("no 0 opint!!\n"); //确定0点
    exit(0);
  }
  if (!check()) {
    printf("no answer!!\n"); //确定是否有解
    exit(0);
  }
  open.head = open.rear = (QueuePtr)malloc(sizeof(QNode)); //头结点
  open.head->father = NULL;
  open.rear->next = open.head->next = NULL;
  closed = (QueuePtr)malloc(sizeof(QNode)); //头结点
  closed->next = NULL;
  closed->father = NULL;
  p = (QueuePtr)malloc(sizeof(QNode)); //S0进open表
  copy(&p);
  p->x = x;
  p->y = y;
  p->ancent = 5;
  p->deep = 0; //s0的深度为0
  p->gone = 0;
  p->father = open.head;
  p->value = getvalue(&p); //
  p->next = open.head->next;
  open.head->next = p;
  open.rear = open.head;
  //min=p; //初始为s0结点
  if (compare(p->data)) {
    output(&p);
    exit(0);
  }
  while (open.head->next != NULL) {
    /*q=open.head->next; //open进closed
    open.head->next=q->next; //移除open表头结点
    q->next=closed->next; //插入closed表的表头
    closed->next=q;
    */
    //寻找最小状态
    for (min = q = open.head->next; q != NULL; q = q->next) {
      if (q->value <= min->value && q->gone == 0) {
        min = q;
        break;
      }
    }
    min->gone = 1; //改最小状态已遍历
    min->father->next = min->next; //在open表中删除找到的最小态
    min->next = closed->next; //插入closed表的表头
    //min->father=closed; //？
    closed->next = min;
    //空格向4个方向移动
    switch (closed->next->ancent) {
    case 1:
      p = (QueuePtr)malloc(sizeof(QNode)); //祖先结点从右来
      if (moveleft(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 1;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //比较输出结果
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (moveup(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 2;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //比较输出结果
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (movedown(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 3;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //比较输出结果
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      break;
    case 2:
      p = (QueuePtr)malloc(sizeof(QNode)); //祖先结点从下来
      if (moveleft(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 1;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //比较输出结果
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (moveup(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 2;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //比较输出结果
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (moveright(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 4;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //比较输出结果
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      break;
    case 3:
      p = (QueuePtr)malloc(sizeof(QNode)); //祖先结点从上来
      if (moveleft(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 1;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //比较输出结果
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (movedown(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 3;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //比较输出结果
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (moveright(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 4;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //比较输出结果
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      break;
    case 4:
      p = (QueuePtr)malloc(sizeof(QNode)); //祖先结点从左边来
      if (moveup(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 2;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //比较输出结果
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (movedown(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 3;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //比较输出结果
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (moveright(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 4;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //比较输出结果
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      break;
    default:
      p = (QueuePtr)malloc(sizeof(QNode)); //初始情况
      if (moveleft(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 1;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //比较输出结果
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (moveup(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 2;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //比较输出结果
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (movedown(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 3;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //比较输出结果
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (moveright(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 4;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //比较输出结果
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      break;
    }
  }
  printf("error: no answer!\n");
}

