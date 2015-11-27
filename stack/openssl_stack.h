#ifndef HEADER_STACK_H
#define HEADER_STACK_H

# define OBJ_BSEARCH_VALUE_ON_NOMATCH            0x01
# define OBJ_BSEARCH_FIRST_VALUE_ON_MATCH        0x02

# ifndef CHECKED_PTR_OF
#  define CHECKED_PTR_OF(type, p) ((void*) (1 ? p : (type*)0))
# endif

# define CHECKED_STACK_OF(type, p) \
    ((_STACK*) (1 ? p : (STACK_OF(type)*)0))
# define CHECKED_SK_COPY_FUNC(type, p) \
    ((void *(*)(void *)) ((1 ? p : (type *(*)(const type *))0)))
# define CHECKED_SK_FREE_FUNC(type, p) \
    ((void (*)(void *)) ((1 ? p : (void (*)(type *))0)))
# define CHECKED_SK_CMP_FUNC(type, p) \
    ((int (*)(const void *, const void *)) \
        ((1 ? p : (int (*)(const type * const *, const type * const *))0)))

# define STACK_OF(type) struct stack_st_##type
# define PREDECLARE_STACK_OF(type) STACK_OF(type);

# define DECLARE_STACK_OF(type) STACK_OF(type);
# define DECLARE_SPECIAL_STACK_OF(type, type2) STACK_OF(type);


# define SKM_sk_new(type, cmp) \
        ((STACK_OF(type) *)sk_new(cmp))
# define SKM_sk_new_null(type) \
        ((STACK_OF(type) *)sk_new_null())
# define SKM_sk_free(type, st) \
        sk_free(CHECKED_STACK_OF(type, st))
# define SKM_sk_num(type, st) \
        sk_num(CHECKED_STACK_OF(type, st))
# define SKM_sk_value(type, st,i) \
        ((type *)sk_value(CHECKED_STACK_OF(type, st), i))
# define SKM_sk_set(type, st,i,val) \
        sk_set(CHECKED_STACK_OF(type, st), i, CHECKED_PTR_OF(type, val))
# define SKM_sk_zero(type, st) \
        sk_zero(CHECKED_STACK_OF(type, st))
# define SKM_sk_push(type, st, val) \
        sk_push(CHECKED_STACK_OF(type, st), CHECKED_PTR_OF(type, val))
# define SKM_sk_unshift(type, st, val) \
        sk_unshift(CHECKED_STACK_OF(type, st), CHECKED_PTR_OF(type, val))
# define SKM_sk_find(type, st, val) \
        sk_find(CHECKED_STACK_OF(type, st), CHECKED_PTR_OF(type, val))
# define SKM_sk_find_ex(type, st, val) \
        sk_find_ex(CHECKED_STACK_OF(type, st), \
                   CHECKED_PTR_OF(type, val))
# define SKM_sk_delete(type, st, i) \
        (type *)sk_delete(CHECKED_STACK_OF(type, st), i)
# define SKM_sk_delete_ptr(type, st, ptr) \
        (type *)sk_delete_ptr(CHECKED_STACK_OF(type, st), CHECKED_PTR_OF(type, ptr))
# define SKM_sk_insert(type, st,val, i) \
        sk_insert(CHECKED_STACK_OF(type, st), CHECKED_PTR_OF(type, val), i)
# define SKM_sk_set_cmp_func(type, st, cmp) \
        ((int (*)(const type * const *,const type * const *)) \
        sk_set_cmp_func(CHECKED_STACK_OF(type, st), CHECKED_SK_CMP_FUNC(type, cmp)))
# define SKM_sk_dup(type, st) \
        (STACK_OF(type) *)sk_dup(CHECKED_STACK_OF(type, st))
# define SKM_sk_pop_free(type, st, free_func) \
        sk_pop_free(CHECKED_STACK_OF(type, st), CHECKED_SK_FREE_FUNC(type, free_func))
# define SKM_sk_deep_copy(type, st, copy_func, free_func) \
        (STACK_OF(type) *)sk_deep_copy(CHECKED_STACK_OF(type, st), CHECKED_SK_COPY_FUNC(type, copy_func), CHECKED_SK_FREE_FUNC(type, free_func))
# define SKM_sk_shift(type, st) \
        (type *)sk_shift(CHECKED_STACK_OF(type, st))
# define SKM_sk_pop(type, st) \
        (type *)sk_pop(CHECKED_STACK_OF(type, st))
# define SKM_sk_sort(type, st) \
        sk_sort(CHECKED_STACK_OF(type, st))
# define SKM_sk_is_sorted(type, st) \
        sk_is_sorted(CHECKED_STACK_OF(type, st))

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct stack_st _STACK; 

int sk_num(const _STACK *);
void *sk_value(const _STACK *, int);

void *sk_set(_STACK *, int, void *);

_STACK *sk_new(int (*cmp) (const void *, const void *));
_STACK *sk_new_null(void);
void sk_free(_STACK *);
void sk_pop_free(_STACK *st, void (*func) (void *));
_STACK *sk_deep_copy(_STACK *, void *(*)(void *), void (*)(void *));
int sk_insert(_STACK *sk, void *data, int where);
void *sk_delete(_STACK *st, int loc);
void *sk_delete_ptr(_STACK *st, void *p);
int sk_find(_STACK *st, void *data);
int sk_find_ex(_STACK *st, void *data);
int sk_push(_STACK *st, void *data);
int sk_unshift(_STACK *st, void *data);
void *sk_shift(_STACK *st);
void *sk_pop(_STACK *st);
void sk_zero(_STACK *st);
int (*sk_set_cmp_func(_STACK *sk, int (*c) (const void *, const void *)))
 (const void *, const void *);
_STACK *sk_dup(_STACK *st);
void sk_sort(_STACK *st);
int sk_is_sorted(const _STACK *st);

#ifdef  __cplusplus
}
#endif

#endif
