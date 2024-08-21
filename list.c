#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  Vector* ptr = malloc(sizeof(Vector));

  ptr->maxvals = size;
  ptr->numvals = 0;
  ptr->type = type;
  ptr->entries = malloc(get_effective_size(type) * size);

  return ptr;
}

int destroy_vector(Vector** vector) {
  Vector* v = *vector;
  if (v) {
    free(v->entries);
    v->maxvals = 0;
    v->numvals = 0;
    v->type = 0;
    *vector = NULL;
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
    printf("Vec Error: new size will cause data loss from vector\n");
    exit(-1);
  }
}

void* vector_get(Vector* vector, void* ptr, int index) {
  size_t size = get_effective_size(vector->type);
  return memcpy(ptr, vector->entries + index * size, size);
}

int vector_pop(Vector* vector, void* ptr, int index) {
  int i;
  size_t size = get_effective_size(vector->type);
  if (vector_get(vector, ptr, index) < 0) {
    perror("Vec Error:");
    return -1;
  };
  vector->numvals--;
  for (i = index; i < vector->numvals; i++) {
    void* lower = vector->entries + i * size;
    void* upper = vector->entries + (i + 1) * size;
    memcpy(lower, upper, size);
  }
  return 0;
}

int vector_push(Vector* vector, void* val) {
  if (vector->numvals == vector->maxvals) {
    vector_resize(vector, 1);
  }
  if (vector->numvals < vector->maxvals) {
    vector_assign_value(vector, val, vector->numvals);
    ++vector->numvals;
    return 0;
  } else {
    printf("Vec Error: cant extend vector corrupted vector");
    exit(-1);
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
    printf("Vec Error: write is out of range");
    exit(-1);
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
    printf("%i, ", ((int*)vector->entries)[i]);
  }
  printf("%i}\nlength:%i  capacity:%i\n",
         ((int*)vector->entries)[vector->numvals - 1], vector_len(vector),
         vector_size(vector));
}

int main(void) {
  int i = 9;
  Vector* x = NULL;
  Vector* y = NULL;
  y = create_vector(INT, 4);
  int g = 2;
  vector_push(y, &g);
  vector_push(y, &g);
  vector_push(y, &g);
  vector_push(y, &i);

  x = create_vector(INT, 10);
  while (i) {
    i--;
    vector_push(x, &i);
  }

  int out = 0;
  vector_get(x, &out, 2);

  printf("%i\n", out);
  print_vector(x);

  int popped = 0;
  vector_pop(x, &popped, 1);

  printf("%i\n", popped);
  print_vector(x);

  destroy_vector(&x);
  destroy_vector(&y);
  return 0;
}
