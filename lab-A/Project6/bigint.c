#define _CRT_SECURE_NO_WARNINGS
#include "bigint.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static Node* CreateNode(int digit) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->digit = digit;
    n->next = NULL;
    return n;
}

BigInt* BigInt_Create(void) {
    BigInt* b = (BigInt*)malloc(sizeof(BigInt));
    b->head = NULL;
    return b;
}

void BigInt_Free(BigInt* b) {
    if (!b) return;
    Node* cur = b->head;
    while (cur) {
        Node* tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    free(b);
}

BigInt* BigInt_Clone(const BigInt* b) {
    if (!b) return NULL;
    BigInt* res = BigInt_Create();
    Node* src = b->head;
    Node** dst = &res->head;
    while (src) {
        *dst = CreateNode(src->digit);
        dst = &(*dst)->next;
        src = src->next;
    }
    return res;
}

BigInt* BigInt_FromString(const char* str) {
    BigInt* b = BigInt_Create();
    if (!str || str[0] == '\0') {
        b->head = CreateNode(0);
        return b;
    }

    size_t len = strlen(str);
    size_t start = 0;
    while (start < len - 1 && str[start] == '0') start++; // пропуск ведущих нулей

    for (int i = (int)len - 1; i >= (int)start; i--) {
        if (str[i] < '0' || str[i] > '9') continue; // игнорируем нецифровые символы
        Node* n = CreateNode(str[i] - '0');
        n->next = b->head;
        b->head = n;
    }

    if (b->head == NULL) b->head = CreateNode(0); // случай "0" или пустая строка
    return b;
}

char* BigInt_ToString(const BigInt* b) {
    if (!b || !b->head) {
        char* s = (char*)malloc(2);
        strcpy(s, "0");
        return s;
    }
    int len = 0;
    for (Node* cur = b->head; cur; cur = cur->next) len++;
    char* s = (char*)malloc(len + 1);
    int i = len - 1;
    for (Node* cur = b->head; cur; cur = cur->next) {
        s[i--] = cur->digit + '0';
    }
    s[len] = '\0';
    return s;
}

BigInt* BigInt_Add(const BigInt* a, const BigInt* b) {
    BigInt* res = BigInt_Create();
    Node* curA = a ? a->head : NULL;
    Node* curB = b ? b->head : NULL;
    Node** curRes = &res->head;
    int carry = 0;

    while (curA || curB || carry) {
        int sum = carry;
        if (curA) { sum += curA->digit; curA = curA->next; }
        if (curB) { sum += curB->digit; curB = curB->next; }
        *curRes = CreateNode(sum % 10);
        curRes = &(*curRes)->next;
        carry = sum / 10;
    }
    return res;
}

static BigInt* BigInt_MulDigit(const BigInt* a, int d) {
    if (d == 0) return BigInt_FromString("0");
    BigInt* res = BigInt_Create();
    Node* cur = a->head;
    Node** dst = &res->head;
    int carry = 0;

    while (cur || carry) {
        int prod = carry;
        if (cur) { prod += cur->digit * d; cur = cur->next; }
        *dst = CreateNode(prod % 10);
        dst = &(*dst)->next;
        carry = prod / 10;
    }
    return res;
}

BigInt* BigInt_Mul(const BigInt* a, const BigInt* b) {
    // Инициализируем результат нулём
    BigInt* res = BigInt_FromString("0");
    int shift = 0;

    for (Node* curB = b->head; curB; curB = curB->next, shift++) {
        if (curB->digit == 0) continue;

        BigInt* partial = BigInt_MulDigit(a, curB->digit);

        // Сдвиг влево (умножение на 10^shift) достигается добавлением нулей в начало списка (LSD)
        Node* shiftHead = partial->head;
        for (int i = 0; i < shift; i++) {
            Node* z = CreateNode(0);
            z->next = shiftHead;
            shiftHead = z;
        }
        partial->head = shiftHead;

        BigInt* newRes = BigInt_Add(res, partial);
        BigInt_Free(res);
        BigInt_Free(partial);
        res = newRes;
    }
    return res;
}

BigInt* BigInt_Pow(const BigInt* base, unsigned int exp) {
    BigInt* res = BigInt_FromString("1");
    if (exp == 0) return res;

    BigInt* curr = BigInt_Clone(base);
    while (exp > 0) {
        if (exp & 1) {
            BigInt* tmp = BigInt_Mul(res, curr);
            BigInt_Free(res);
            res = tmp;
        }
        exp >>= 1;
        if (exp > 0) {
            BigInt* tmp = BigInt_Mul(curr, curr);
            BigInt_Free(curr);
            curr = tmp;
        }
    }
    BigInt_Free(curr);
    return res;
}