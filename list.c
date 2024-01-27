#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int maxvals;
    int numvals;
    int *entries;
} intarray_t;

int pointer_clear(int **ptr)
{
    if (*ptr != NULL)
    {
        free(*ptr);
        *ptr = NULL;
    }
}

intarray_t *create_intarray(int maxsize)
{
    intarray_t *ptr = NULL;

    ptr = (intarray_t *)malloc(sizeof(intarray_t));

    ptr->entries = (int *)malloc(sizeof(int) * maxsize);
    ptr->maxvals = maxsize;
    ptr->numvals = 0;

    return ptr;
}

int destroy_intarray(intarray_t *array)
{
    if (array)
    {
        free(array->entries);
        array->maxvals = 0;
        array->numvals = 0;
        return 0;
    }
    else
    {
        return -1;
    }
}

void resize_array(intarray_t *array, int newsize)
{
    if (array->maxvals + newsize > array->numvals)
    {
        int *newentries = NULL;
        int i;

        newentries = (int *)malloc(sizeof(int) * (array->maxvals + newsize));

        for (i = 0; i < array->numvals; i++)
        {
            newentries[i] = array->entries[i];
        }

        pointer_clear(&array->entries);
        array->entries = newentries;
        array->maxvals = array->maxvals + newsize;
    }
    else
    {
        printf("Warning: new size will cause data loss from array\n");
    }
}

void append_intarray(intarray_t *array, int val)
{
    if (array->numvals == array->maxvals)
    {
        resize_array(array, 1);
    }
    if (array->numvals < array->maxvals)
    {
        array->entries[array->numvals] = val;
        ++array->numvals;
    }
}

int write_intarray(intarray_t *array, int val, int index)
{
    if (index + 1 < array->maxvals)
    {
        array->entries[index] = val;
        if (index + 1 > array->numvals)
        {
            array->numvals = index + 1;
        }
        return 0;
    }
    else
    {
        return -1;
    }
}

int len_intarray(intarray_t *array)
{
    return array->numvals;
}

int size_intarray(intarray_t *array)
{
    return array->maxvals;
}

void print_intarray(intarray_t *array)
{
    int i;

    printf("array:{");

    for (i = 0; i < array->numvals - 1; i++)
    {
        printf("%i, ", array->entries[i]);
    }

    printf(
        "%i}\nlength:%i  capacity:%i\n",
        array->entries[array->numvals - 1],
        len_intarray(array),
        size_intarray(array));
}

int main(void)
{
    int i = 10;
    intarray_t *x = NULL;

    x = create_intarray(10);

    while (i)
    {
        append_intarray(x, i);
        i--;
    }
    write_intarray(x, 1337, 0);
    print_intarray(x);

    write_intarray(x, 69, 5);
    print_intarray(x);

    append_intarray(x, 420);
    print_intarray(x);

    return 0;
}