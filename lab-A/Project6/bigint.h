#ifndef BIGINT_H
#define BIGINT_H

#include <stddef.h>

typedef struct Node {
    int digit;
    struct Node* next;
} Node;

// Число хранится в виде списка, где голова (head) – младший разряд (LSD).
// Например, число 123 хранится как: 3 -> 2 -> 1 -> NULL
typedef struct {
    Node* head;
} BigInt;

// Управление памятью и создание
BigInt* BigInt_Create(void);
BigInt* BigInt_FromString(const char* str);
char* BigInt_ToString(const BigInt* b);
void    BigInt_Free(BigInt* b);
BigInt* BigInt_Clone(const BigInt* b);

// Арифметика
BigInt* BigInt_Add(const BigInt* a, const BigInt* b);
BigInt* BigInt_Mul(const BigInt* a, const BigInt* b);
BigInt* BigInt_Pow(const BigInt* base, unsigned int exp);

#endif 
