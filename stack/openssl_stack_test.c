#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "openssl_stack.h"

typedef struct DEMO
{
	int id;
	int mark;
}DEMO;

# define sk_DEMO_new(cmp) SKM_sk_new(DEMO, (cmp))
# define sk_DEMO_new_null() SKM_sk_new_null(DEMO)
# define sk_DEMO_free(st) SKM_sk_free(DEMO, (st))
# define sk_DEMO_num(st) SKM_sk_num(DEMO, (st))
# define sk_DEMO_value(st, i) SKM_sk_value(DEMO, (st), (i))
# define sk_DEMO_set(st, i, val) SKM_sk_set(DEMO, (st), (i), (val))
# define sk_DEMO_zero(st) SKM_sk_zero(DEMO, (st))
# define sk_DEMO_push(st, val) SKM_sk_push(DEMO, (st), (val))
# define sk_DEMO_unshift(st, val) SKM_sk_unshift(DEMO, (st), (val))
# define sk_DEMO_find(st, val) SKM_sk_find(DEMO, (st), (val))
# define sk_DEMO_find_ex(st, val) SKM_sk_find_ex(DEMO, (st), (val))
# define sk_DEMO_delete(st, i) SKM_sk_delete(DEMO, (st), (i))
# define sk_DEMO_delete_ptr(st, ptr) SKM_sk_delete_ptr(DEMO, (st), (ptr))
# define sk_DEMO_insert(st, val, i) SKM_sk_insert(DEMO, (st), (val), (i))
# define sk_DEMO_set_cmp_func(st, cmp) SKM_sk_set_cmp_func(DEMO, (st), (cmp))
# define sk_DEMO_dup(st) SKM_sk_dup(DEMO, st)
# define sk_DEMO_pop_free(st, free_func) SKM_sk_pop_free(DEMO, (st), (free_func))
# define sk_DEMO_deep_copy(st, copy_func, free_func) SKM_sk_deep_copy(DEMO, (st), (copy_func), (free_func))
# define sk_DEMO_shift(st) SKM_sk_shift(DEMO, (st))
# define sk_DEMO_pop(st) SKM_sk_pop(DEMO, (st))
# define sk_DEMO_sort(st) SKM_sk_sort(DEMO, (st))
# define sk_DEMO_is_sorted(st) SKM_sk_is_sorted(DEMO, (st))


int demo_cmp(const void * a, const void * b)
{
	int ret;
	struct DEMO *p1 = *(struct DEMO **)a;
	struct DEMO *p2 = *(struct DEMO **)b;


	if(p1->id > p2->id)
		ret = 1;
	else if(p1->id < p2->id)
		ret = -1;
	else
		ret = 0;

	return ret;
}

static void demo_free(struct DEMO *a)
{
	free(a);
}

int main(int argc, char **argv)
{
	int i, n;
	STACK_OF(DEMO) *demo = NULL, *sortdemo = NULL;
	struct DEMO *node = NULL, *tmp = NULL;
	
	demo = sk_DEMO_new_null();
	sortdemo = sk_DEMO_new(demo_cmp);

	/*not sort*/
	for(i = 0; i < 10; i++)
	{
		node = (struct DEMO *)malloc(sizeof(struct DEMO));
		if(!node)
			continue;
		node->id = (int)(rand() % 100);
		node->mark = i + 100;
		sk_DEMO_push(demo, node);
	}

	n = sk_DEMO_num(demo);
	for(i = 0; i < n; i++)
	{
		tmp = sk_DEMO_value(demo, i);	
		printf("id: %d  mark: %d\n", tmp->id, tmp->mark);
	}
	

	/*sort*/
	for(i = 0; i < 10; i++)
	{
		node = (struct DEMO *)malloc(sizeof(struct DEMO));
		if(!node)
			continue;
		node->id = rand() % 100;
		node->mark = i + 100;
		sk_DEMO_push(sortdemo, (void *)node);
	}

	//find  and have sorted
	i = sk_DEMO_find(sortdemo, node);
	printf("find the end node, id is: %d\n", i);

	/*
	sk_DEMO_sort(sortdemo);
	*/

	n = sk_DEMO_num(sortdemo);
	for(i = 0; i < n; i++)
	{
		tmp = sk_DEMO_value(sortdemo, i);	
		printf("id: %d  mark: %d\n", tmp->id, tmp->mark);
	}


	sk_DEMO_pop_free(demo, demo_free);
	sk_DEMO_pop_free(sortdemo, demo_free);

	return 0;
}
