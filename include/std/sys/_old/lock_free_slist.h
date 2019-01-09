#define CAS(a, b, c)   compare_and_swap(&a, &b, c)
BOOL sys_compare_and_swap(int *accum, int *dest, int newval)
{
  if ( *accum == *dest ) {
      *dest = newval;
      return TRUE;
  }
  return FALSE;
}

BOOL AtomicCAS(LONG volatile *dest, LONG newvalue, LONG oldvalue)
{
  LONG    ret;
  ret = InterlockedCompareExchange(dest, newvalue, oldvalue);
  if ( ret == oldvalue ) {
    return TRUE;
  } else {
    return FALSE;
  }
}

struct {
  struct lock_free_slist* next;
  struct lock_free_slist* prev;
} lock_free_slist;
void slist_push(lock_free_slist* p, lock_free_slist* q) //������
{
  //׼���¼���Ľ������
  q->next = NULL;
  q->prev = NULL;
  do {
  } while(CAS(p->next, NULL, q) != TRUE); //���û�аѽ�����ϣ�����
  CAS(tail, p, q); //��β���
}
