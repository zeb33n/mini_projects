#include <stdio.h>
#include <stdlib.h>

// use generic to get type?
// give vector a type arg.
// use union to assign values
typedef enum {
  INT,
  UINT,
  CHAR,
  LONG,
} Type;

// #define gettype(x) _Generic((x), int : Int, unsigned int : UInt, char : Char)

typedef struct {
  int maxvals;
  int numvals;
  Type type;
  void* entries;
} Vector;

int pointer_clear(int** ptr) {
  if (*ptr != NULL) {
    free(*ptr);
    *ptr = NULL;
    return 0;
  } else {
    return -1;
  }
}

size_t get_effective_size(Type type) {
  switch (type) {
    case CHAR:
      return sizeof(char);
    case INT:
      return sizeof(int);
    case UINT:
      return sizeof(unsigned int);
    case LONG:
      return sizeof(long);
  }
}

void vector_assign_value(Vector* vector, void* val, int index) {
  switch (vector->type) {
    case CHAR:
      ((char*)vector->entries)[index] = *((char*)val);
      break;
    case INT:
      ((int*)vector->entries)[index] = *((int*)val);
      break;
    case UINT:
      ((unsigned int*)vector->entries)[index] = *((unsigned int*)val);
      break;
    case LONG:
      ((long*)vector->entries)[index] = *((long*)val);
      break;
  }
}

Vector* create_vector(Type type, int size) {
  Vector* ptr = (Vector*)malloc(sizeof(Vector));

  ptr->entries = malloc(get_effective_size(type) * size);
  ptr->maxvals = size;
  ptr->numvals = size;
  ptr->type = type;

  return ptr;
}

int destroy_vector(Vector* vector) {
  if (vector) {
    free(vector->entries);
    vector->maxvals = 0;
    vector->numvals = 0;
    return 0;
  } else {
    return -1;
  }
}

int vector_resize(Vector* vector, int newsize) {
  if (vector->maxvals + newsize > vector->numvals) {
    vector->entries =
        realloc(vector->entries,
                get_effective_size(vector->type) * (vector->maxvals + newsize));
    vector->maxvals = vector->maxvals + newsize;
    return 0;
  } else {
    printf("Error: new size will cause data loss from vector\n");
    return -1;
  }
}

int vector_append(Vector* vector, void* val) {
  if (vector->numvals == vector->maxvals) {
    vector_resize(vector, 1);
  }
  if (vector->numvals < vector->maxvals) {
    vector_assign_value(vector, val, vector->numvals);
    ++vector->numvals;
    return 0;
  } else {
    printf("Error: cant extend vector corrupted vector");
    return -1;
  }
}

int vector_write(Vector* vector, void* val, int index) {
  if (index + 1 < vector->maxvals) {
    vector_assign_value(vector, val, index);
    if (index + 1 > vector->numvals) {
      vector->numvals = index + 1;
    }
    return 0;
  } else {
    return -1;
  }
}

int vector_len(Vector* vector) {
  return vector->numvals;
}

int vector_size(Vector* vector) {
  return vector->maxvals;
}

void print_vector(Vector* vector) {
  int i;
  printf("vector:{");
  for (i = 0; i < vector->numvals - 1; i++) {
    printf("%i, ", ((char*)vector->entries)[i]);
  }
  printf("%i}\nlength:%i  capacity:%i\n",
         ((char*)vector->entries)[vector->numvals - 1], vector_len(vector),
         vector_size(vector));
}

int main(void) {
  int i = 9;
  Vector* x = NULL;

  x = create_vector(CHAR, 10);
  while (i) {
    printf("do we make it\n");
    i--;
    vector_append(x, &i);
  }

  print_vector(x);

  return 0;
}
