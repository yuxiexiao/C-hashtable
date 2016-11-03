/*
 * CS 11, C Track, lab 7
 *
 * FILE: hash_table.c
 *
 *       Implementation of the hash table functionality.
 *
 */

/*
 * Include the declaration of the hash table data structures
 * and the function prototypes.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "memcheck.h"


/*** Hash function. ***/

int hash(char *s)
{
	int i = 0;
	int sum = 0;

	while (s[i] != NULL && *s != '\0')
	{
		sum+= (int)s[i];
		i++;
	}

	return (sum % NSLOTS);

}


/*** Linked list utilities. ***/

/* Create a single node. */
node *create_node(char *key, int value)
{
	node *result = (node *)malloc(sizeof(node));

    if (result == NULL)
    {
        fprintf(stderr, "Fatal error: out of memory. "
                "Terminating program.\n");
        exit(1);
    }

    result->key = key;  /* Fill in the new node with the given key. */
    result->value = value;
    result->next = NULL;

    return result;

}


/* Free all the nodes of a linked list. */
void free_list(node *list)
{
	node *n;     /* a single node of the list */

    while (list != NULL)
    {
        n = list;
        list = list->next;

        /*
         * 'n' now points to the first element of the list, while
         * 'list' now points to everything but the first element.
         * Since nothing points to 'n', it can be freed.
         */
        free(n->key);
        free(n);

    }


}

/*** Hash table utilities. ***/

/* Create a new hash table. */
hash_table *create_hash_table()
{
	int i= 0;
	/* allocate memory for the hashtable */
	hash_table *result = malloc(sizeof(hash_table));

	if (result == NULL)
    {
        fprintf(stderr, "Fatal error: out of memory. "
                "Terminating program.\n");
        exit(1);
    }
    /* allocate memory for the slot array */
    result->slot = (node *)malloc(sizeof(node) * NSLOTS);

    if (result->slot == NULL)
    {
        fprintf(stderr, "Fatal error: out of memory. "
                "Terminating program.\n");
        exit(1);
    }
    /* initialize all the linked lists in slot with a NULL node */
    for (i = 0; i < NSLOTS; i++)
    {
    	result->slot[i] = NULL;
    }

    return result;
}


/* Free a hash table. */
void free_hash_table(hash_table *ht)
{
	int i;
	node *list;

	for (i = 0; i < NSLOTS; i++)
	{
		list = ht->slot[i];
		free_list(list);
	}

	free(ht->slot);
	free(ht);

}


/*
 * Look for a key in the hash table.  Return 0 if not found.
 * If it is found return the associated value.
 */
int get_value(hash_table *ht, char *key)
{
	int hashvalue = hash(key);
	node *list;

	list = ht->slot[hashvalue];
	while (list != NULL)
	{
		if (strcmp(key, list->key) == 0) /* if they are equal */
		{
			return (list->value);
		}

		list = list->next;
	}
	return 0;

}


/*
 * Set the value stored at a key.  If the key is not in the table,
 * create a new node and set the value to 'value'.  Note that this
 * function alters the hash table that was passed to it.
 */
void set_value(hash_table *ht, char *key, int value)
{
	int hashvalue = hash(key);
	node *list, *newlist;
	list = ht->slot[hashvalue];

	/* key is not in hashtable */
	if (get_value(ht, key) == 0)
	{
		newlist = create_node(key, value);
		newlist->next = ht->slot[hashvalue];
		ht->slot[hashvalue] = newlist;

	}

	/* key is in hashtable */
	else
	{
		while (list != NULL)
		{
			if (strcmp(key, list->key) == 0)
			{
				list->value = value;
				free(key);
			}

			list = list->next;

		}
	}
}


/* Print out the contents of the hash table as key/value pairs. */
void print_hash_table(hash_table *ht)
{
	int i;
	node *list;
	node *item;

	for(i = 0; i < NSLOTS; i++)
	{
		list = ht->slot[i];
	    for (item = list; item != NULL; item = item->next)
	    {
	        printf("%s %d\n", item->key, item->value);
	    }
	}

}


