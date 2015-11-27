#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "openssl_hash.h"

struct demo
{
	int age;
	char name[256];
}demo[6] =
{
	{32, "xuyuu"},
	{65, "qincai"},
	{12, "suanmiao"},
	{22, "luobo"},
	{35, "fentiao"},
	{4, "xigua"}
};

int lh_strcmp(const void *a, const void *b)
{
	int h1, h2, len;
	const struct demo *p1 = a, *p2 = b;
	len = (h1 = strlen(p1->name)) > (h2 = strlen(p2->name)) ? h2 : h1;

	return ((!strncmp(p1->name, p2->name, len) && p1->age == p2->age && h1 == h2) ? 0 : -1);
}

int main(int argc, char **argv)
{
	int i;
	_LHASH *hash;
	hash = lh_new(lh_strhash, lh_strcmp);

	for(i = 0; i < sizeof(demo)/sizeof(struct demo); i++)
	{
		lh_insert(hash, &demo[i]);
	}

	/*find hash*/
	struct demo d = {62, "qincai"}, *r;
	r = lh_retrieve(hash, &d);
	if(r)
	{
		printf("find node in hashtable .\n");
		printf("age : %d   name : %s\n", r->age, r->name);
	}
	else
	{
		printf("don`t find node in hashtable .\n");	
	}

	return 0;
}


