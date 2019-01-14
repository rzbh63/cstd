// APR�ڴ��
#include <windows.h>
#define MEM_ALIGN(size,boundary) (((size) + ((boundary)-1)) & ~((boundary)-1))
#define MEM_ALIGN_DEFAULT(size) MEM_ALIGN(size,8)
#define BOUNDARY_INDEX  12
#define MAX_INDEX       20

typedef struct _MEMNODE
{
    struct _MEMNODE         *next;
    struct _MEMNODE         **ref;
    unsigned int            index;
    unsigned int            free_index;
    char                    *first_avail;
    char                    *endp;
} MEMNODE;


#define Allocator_CS_Get(a)   (a)->pcs
#define Allocator_CS_Set(a, b)   (a)->pcs = (b)
#define SIZEOF_ALLOCATOR sizeof(ALLOCATOR)
#define Allocator_owner_Get(a)  (a)->owner
#define Allocator_owner_Set(a, b)  (a)->owner = (b)
typedef struct _ALLOCATOR
{
    unsigned int            max_index;
    unsigned int            max_free_index;
    unsigned int            current_free_index;
    CRITICAL_SECTION        *pcs;
    struct _MEMPOOL         *owner;
    struct _MEMNODE         *pfree[MAX_INDEX];
} ALLOCATOR;

const unsigned int BOUNDARY_SIZE    = 1 << BOUNDARY_INDEX;  //4096
const unsigned int MIN_ALLOC        = 2 * BOUNDARY_SIZE;    //8192
const unsigned int SIZEOF_MEMNODE   = MEM_ALIGN_DEFAULT(sizeof(MEMNODE));//MEMNODE��С


MEMNODE *Allocator_alloc(ALLOCATOR *allocator, size_t in_size)
{
    MEMNODE *node, **ref;
    unsigned int max_index;
    size_t size, i, index;

    //4096�ı���������8192�������8192
    size = MEM_ALIGN(in_size + SIZEOF_MEMNODE, BOUNDARY_SIZE);
    if (size < in_size) return NULL;
    if (size < MIN_ALLOC)   size = MIN_ALLOC;

    //����СתΪ�±�
    index = (size >> BOUNDARY_INDEX) - 1;

    //С�ڵ�ǰ���ڵ�
    if (index <= allocator->max_index) 
    {
        if (allocator->pcs)
            EnterCriticalSection(allocator->pcs);

        //�ҵ���һ�����õĲ�Ϊ�սڵ�
        max_index = allocator->max_index;
        ref = &allocator->pfree[index];
        i = index;
        while (*ref == NULL && i < max_index) 
        {
           ref++;
           i++;
        }

        //�ҵ�һ���ǿյ�����
        if ( (node = *ref) ) 
        {
            if ((*ref = node->next) == NULL && i >= max_index) 
            {//��ǰ����û�к���node���������ߵ�node���ڵ�ǰ��������ڵ㣬�������������ڵ�
                do 
                {
                    ref--;
                    max_index--;
                }
                while (*ref == NULL && max_index > 0);

                allocator->max_index = max_index;
            }

            allocator->current_free_index += node->index + 1;
            if (allocator->current_free_index > allocator->max_free_index)
                allocator->current_free_index = allocator->max_free_index;

            if (allocator->pcs)
                LeaveCriticalSection(allocator->pcs);

            node->next = NULL;
            node->first_avail = (char *)node + SIZEOF_MEMNODE;

            return node;
        }

        if (allocator->pcs)
            LeaveCriticalSection(allocator->pcs);
    }
    //���ڵ�ǰ���ڵ�,����0���� �������Ϊ��
    else if (allocator->pfree[0]) 
    {
        if (allocator->pcs)
            EnterCriticalSection(allocator->pcs);

        //�ҵ���һ�������С�Ĳ�Ϊ�սڵ�
        ref = &allocator->pfree[0];
        while (  (node = *ref)  && index > node->index)
            ref = &node->next;

        if (node) 
        {
            *ref = node->next;

            allocator->current_free_index += node->index + 1;
            if (allocator->current_free_index > allocator->max_free_index)
                allocator->current_free_index = allocator->max_free_index;

            if (allocator->pcs)
                LeaveCriticalSection(allocator->pcs);


            node->next = NULL;
            node->first_avail = (char *)node + SIZEOF_MEMNODE;

            return node;
        }

        if (allocator->pcs)
            LeaveCriticalSection(allocator->pcs);

    }

    //ʲô��û�ҵ���ֱ������
    if ((node = (MEMNODE*)malloc(size)) == NULL)
        return NULL;

    node->next = NULL;
    node->index = index;
    node->first_avail = (char *)node + SIZEOF_MEMNODE;
    node->endp = (char *)node + size;

    return node;
}


void Allocator_free(ALLOCATOR *allocator, MEMNODE *node)
{
    MEMNODE *next, *freelist = NULL;
    unsigned int index, max_index;
    unsigned int max_free_index, current_free_index;

    if (allocator->pcs)
        EnterCriticalSection(allocator->pcs);

    max_index = allocator->max_index;
    max_free_index = allocator->max_free_index;
    current_free_index = allocator->current_free_index;

    do 
    {
        next = node->next;
        index = node->index;

        if (max_free_index != 0
            && index + 1 > current_free_index) 
        {//�Ѿ���������������黹��ϵͳ
            node->next = freelist;
            freelist = node;
        }
        else if (index < MAX_INDEX) 
        {//С��80K�����ڹ���ڵ�(1-19) 
            if ((node->next = allocator->pfree[index]) == NULL
                && index > max_index) 
            {//���ڵ�ǰ���ڵ㣬����max_index��
                max_index = index;
            }
            allocator->pfree[index] = node;
            if (current_free_index >= (index + 1))
                current_free_index -= (index + 1);
            else
                current_free_index = 0;
        }
        else 
        {//����80K,�ҵ�0������
            node->next = allocator->pfree[0];
            allocator->pfree[0] = node;
            if (current_free_index >= (index + 1))
                current_free_index -= (index + 1);
            else
                current_free_index = 0;
        }
    } while ( (node = next)  );

    allocator->max_index = max_index;
    allocator->current_free_index = current_free_index;

    if (allocator->pcs)
        LeaveCriticalSection(allocator->pcs);


    while (freelist != NULL) 
    {
        node = freelist;
        freelist = node->next;
        free(node);
    }
}


typedef int (*CLEANUP_FUNC)(void* p);

struct _CLEANUP {
  const void *data;
  CLEANUP_FUNC cleanup_func;
  struct _CLEANUP* next;
};


typedef struct _MEMPOOL 
{
    struct _MEMPOOL         *parent;
    struct _MEMPOOL         *child;
    struct _MEMPOOL         *sibling;
    struct _MEMPOOL         **ref;

    struct _CLEANUP         *cleanups;
    struct _CLEANUP         *free_cleanups;

    struct _ALLOCATOR       *allocator;
    struct _MEMNODE         *active;
} MEMPOOL;


#define node_free_space(node_) ((size_t)(node_->endp - node_->first_avail))

#define list_remove(node) do {      \
    *node->ref = node->next;        \
    node->next->ref = node->ref;    \
} while (0)

#define list_insert(node, point) do {           \
    node->ref = point->ref;                     \
    *node->ref = node;                          \
    node->next = point;                         \
    point->ref = &node->next;                   \
} while (0)


const unsigned int SIZEOF_MEMPOOL   = MEM_ALIGN_DEFAULT(sizeof(MEMPOOL));//MEMPOOL��С
static int          pools_initialized   = 0;
static ALLOCATOR    *global_allocator   = NULL;
static MEMPOOL      *global_pool        = NULL;


BOOL Pool_Create(MEMPOOL **newpool,MEMPOOL *parent,ALLOCATOR *allocator /* = NULL */)
{
    MEMPOOL *pool;
    MEMNODE *node;

    if (newpool==NULL)
        return FALSE;

    if (parent == NULL)
        parent = global_pool;

    if (allocator == NULL)
        allocator = parent->allocator;

    if ((node = Allocator_alloc(allocator,MIN_ALLOC - SIZEOF_MEMNODE)) == NULL) 
        return FALSE;

    node->next = node;
    node->ref = &node->next;

    //���粽��1����˵����MEMPOOL����MEMNODE֮��
    pool = (MEMPOOL *)node->first_avail;
    node->first_avail = (char *)pool + SIZEOF_MEMPOOL;

    pool->allocator     = allocator;
    pool->active        = node;
    pool->child         = NULL;
    pool->cleanups      = NULL;
    pool->free_cleanups = NULL;

    //���´��������ڹ���MEMPOOL֮������ͽṹ
    if (  (pool->parent = parent) ) 
    {
        CRITICAL_SECTION *pcs;

        if ( (pcs = Allocator_CS_Get(parent->allocator)) )
            EnterCriticalSection(pcs);


        if ( (pool->sibling = parent->child) )
            pool->sibling->ref = &pool->sibling;

        parent->child = pool;
        pool->ref = &parent->child;


        if (pcs)
            LeaveCriticalSection(pcs);

    }
    else 
    {
        pool->sibling = NULL;
        pool->ref = NULL;
    }

    *newpool = pool;

    return TRUE;
}

//�����Ǵ������ĵ�ʵ���ڴ�����ĺ�����

void * Mem_Alloc(MEMPOOL *pool, size_t in_size)
{
    MEMNODE *active, *node;
    void *mem;
    size_t size, free_index;

    size = MEM_ALIGN_DEFAULT(in_size);      //��ӽ��Ĳ�С��8�ı���
    if (size < in_size) return NULL;

    active = pool->active;

    //�����ʣ��ռ䣬ֱ��ʹ��
    if (size <= node_free_space(active)) 
    {
        mem = active->first_avail;
        active->first_avail += size;

        memset(mem,0,in_size);
        return mem;
    }

    node = active->next;//�鿴��һ��ʣ��ռ��Ƿ���
    if (size <= node_free_space(node)) 
    {
        list_remove(node);
    }
    else 
    {//���ǲ����������Ŀ϶�Ҳ�����ã���Ϊʣ��ռ��С���Ź����
        if ((node = Allocator_alloc(pool->allocator, size)) == NULL) 
            return NULL;
    }

    node->free_index = 0;   

    mem = node->first_avail;
    node->first_avail += size;

    list_insert(node, active);

    pool->active = node;

    free_index = (MEM_ALIGN(active->endp - active->first_avail + 1,
        BOUNDARY_SIZE) - BOUNDARY_SIZE) >> BOUNDARY_INDEX;  //�����С��4096�ı���

    active->free_index = free_index;
    node = active->next;
    if (free_index >= node->free_index)
    {
        memset(mem,0,in_size);
        return mem;
    }

    //��ʣ��ռ�Ӵ�С����
    do 
    {
        node = node->next;
    }
    while (free_index < node->free_index);

    list_remove(active);
    list_insert(active, node);

    memset(mem,0,in_size);
    return mem;
}


BOOL Allocator_Create(ALLOCATOR **allocator)
{
    ALLOCATOR *new_allocator;

    if (allocator == NULL)
        return FALSE;

    if ((new_allocator = (ALLOCATOR*)malloc(SIZEOF_ALLOCATOR)) == NULL)
        return FALSE;

    memset(new_allocator, 0, SIZEOF_ALLOCATOR);

    *allocator = new_allocator;

    return TRUE;
}
void Allocator_Destroy(ALLOCATOR *allocator)
{
    unsigned int index;
    MEMNODE *node, **ref;

    for (index = 0; index < MAX_INDEX; index++) 
    {
        ref = &allocator->pfree[index];
        while ( (node = *ref) ) 
        {
            *ref = node->next;
            free(node);
        }
    }

    free(allocator);
}

void Allocator_max_free_Set(ALLOCATOR *allocator,size_t in_size)
{
    unsigned int max_free_index;
    unsigned int size = in_size;

    CRITICAL_SECTION *pcs;

    pcs = Allocator_CS_Get(allocator);
    if (pcs)
        EnterCriticalSection(pcs);


    max_free_index = MEM_ALIGN(size, BOUNDARY_SIZE) >> BOUNDARY_INDEX;
    allocator->current_free_index += max_free_index;
    allocator->current_free_index -= allocator->max_free_index;
    allocator->max_free_index = max_free_index;
    if (allocator->current_free_index > max_free_index)
        allocator->current_free_index = max_free_index;


    if (pcs)
        LeaveCriticalSection(pcs);
}

static void Run_Cleanups(_CLEANUP **cref)
{
    _CLEANUP *c = *cref;

    while (c) 
    {
        *cref = c->next;
        c->cleanup_func((void*)c->data);
        c = *cref;
    }
}

static int thread_mutex_cleanup(void *data)
{
  CRITICAL_SECTION *pcs = (CRITICAL_SECTION *)data;
  
  DeleteCriticalSection(pcs);
  
  return true;
}

void Pool_Cleanup_Register(MEMPOOL *p, const void *data, CLEANUP_FUNC cleanup_fnc)
{
  _CLEANUP *c;
  
  if ( p ) 
  {
    c =(_CLEANUP *) Mem_Alloc(p, sizeof(_CLEANUP));
    
    c->data = data;
    c->cleanup_func = cleanup_fnc;
    
    c->next = p->cleanups;
    p->cleanups = c;
  }
}
BOOL CriticalSection_Create(CRITICAL_SECTION **ppCS,MEMPOOL *pool)
{
    (*ppCS) = (CRITICAL_SECTION *)Mem_Alloc(pool, sizeof(**ppCS));

    InitializeCriticalSection(*ppCS);

    Pool_Cleanup_Register(pool, (*ppCS), thread_mutex_cleanup);
    return TRUE;
}

void Pool_Destroy(MEMPOOL *pool)
{
    MEMNODE *active;
    ALLOCATOR *allocator;

    while (pool->child)
        Pool_Destroy(pool->child);

    Run_Cleanups(&pool->cleanups);

    if (pool->parent) 
    {

        CRITICAL_SECTION *pcs;

        if ( (pcs = Allocator_CS_Get(pool->parent->allocator)) )
            EnterCriticalSection(pcs);


        if ( (*pool->ref = pool->sibling) )
            pool->sibling->ref = pool->ref;

        if (pcs)
            LeaveCriticalSection(pcs);

    }


    allocator = pool->allocator;
    active = pool->active;
    *active->ref = NULL;


    if (Allocator_owner_Get(allocator) == pool)
    { 
        Allocator_CS_Set(allocator, NULL);
    }

    Allocator_free(allocator, active);


    if (Allocator_owner_Get(allocator) == pool) 
    {
        Allocator_Destroy(allocator);
    }
}


BOOL Pool_Initialize(void)
{
    if (pools_initialized++)
        return TRUE;

    if (Allocator_Create(&global_allocator) == FALSE) 
    {
        pools_initialized = 0;
        return FALSE;
    }

    if ( Pool_Create(&global_pool, NULL,global_allocator) == FALSE) 
    {
        Allocator_Destroy(global_allocator);
        global_allocator = NULL;
        pools_initialized = 0;
        return FALSE;
    }

    CRITICAL_SECTION* pcs;

    if ( CriticalSection_Create(&pcs,global_pool) == FALSE) 
    {
        return FALSE;
    }

    Allocator_CS_Set(global_allocator, pcs);

    Allocator_owner_Set(global_allocator, global_pool);

    return TRUE;
}

void Pool_Terminate(void)
{
    if (pools_initialized==0)
        return;

    if (--pools_initialized)
        return;

    Pool_Destroy(global_pool); /* This will also destroy the mutex */
    global_pool = NULL;
    global_allocator = NULL;
}


int test_mempool()
{
const int MAXLENGTH = 100000;

#if 0
  boost::pool<> p(sizeof(int));
  int* vec1[MAXLENGTH];
  int* vec2[MAXLENGTH];
  
  clock_t clock_begin = clock();
  for (int i = 0; i < MAXLENGTH; ++i)
  {
    vec1[i] = static_cast<int*>(p.malloc());
  }
  for (int i = 0; i < MAXLENGTH; ++i)
  {
    p.free(vec1[i]);
    vec1[i] = NULL;
  }
  
  clock_t clock_end = clock();
  cout<<"���������� "<<clock_end-clock_begin<<" ��ϵͳʱ��"<<endl;
  
  clock_begin = clock();
  for (int i = 0; i < MAXLENGTH; ++i)
  {
    vec2[i] = new int;
  }
  for (int i = 0; i < MAXLENGTH; ++i)
  {
    delete vec2[i];
    vec2[i] = NULL;
  }
  
  clock_end = clock();
  cout<<"���������� "<<clock_end-clock_begin<<" ��ϵͳʱ��"<<endl;
#endif 
  return 0;
}