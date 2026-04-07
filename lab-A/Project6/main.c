#include <stdio.h>
#include <stdlib.h>
#include "bigint.h"
#include <locale.h>
static void print_op(const char* op, const BigInt* a, const BigInt* b, const BigInt* res) {
    char* sa = BigInt_ToString(a);
    char* sb = BigInt_ToString(b);
    char* sr = BigInt_ToString(res);
    printf("%s %s %s = %s\n", sa, op, sb, sr);
    free(sa); free(sb); free(sr);
}

int main(void) {
    setlocale(LC_CTYPE, "russian");
    printf("=== Тестирование арифметики длинных чисел ===\n\n");

    // 1. Сложение
    BigInt* a1 = BigInt_FromString("999999999999");
    BigInt* b1 = BigInt_FromString("1");
    BigInt* s1 = BigInt_Add(a1, b1);
    print_op("+", a1, b1, s1);

    // 2. Умножение
    BigInt* a2 = BigInt_FromString("123456789");
    BigInt* b2 = BigInt_FromString("987654321");
    BigInt* s2 = BigInt_Mul(a2, b2);
    print_op("*", a2, b2, s2);

    // 3. Возведение в степень
    BigInt* a3 = BigInt_FromString("2");
    BigInt* s3 = BigInt_Pow(a3, 64);
    char* sa3 = BigInt_ToString(a3);
    char* ss3 = BigInt_ToString(s3);
    printf("%s ^ 64 = %s\n", sa3, ss3);
    free(sa3); free(ss3);

    // 4. Крайние случаи
    printf("\n--- Крайние случаи ---\n");
    BigInt* zero = BigInt_FromString("0");
    BigInt* big = BigInt_FromString("555");
    BigInt* res_zero = BigInt_Mul(big, zero);
    print_op("*", big, zero, res_zero);

    BigInt* res_pow0 = BigInt_Pow(big, 0);
    char* sb = BigInt_ToString(big);
    char* sr = BigInt_ToString(res_pow0);
    printf("%s ^ 0 = %s\n", sb, sr);
    free(sb); free(sr);

    // Очистка памяти
    BigInt_Free(a1); BigInt_Free(b1); BigInt_Free(s1);
    BigInt_Free(a2); BigInt_Free(b2); BigInt_Free(s2);
    BigInt_Free(a3); BigInt_Free(s3);
    BigInt_Free(zero); BigInt_Free(big); BigInt_Free(res_zero); BigInt_Free(res_pow0);

    printf("\n✅ Все операции выполнены. Память освобождена.\n");
    return 0;
}