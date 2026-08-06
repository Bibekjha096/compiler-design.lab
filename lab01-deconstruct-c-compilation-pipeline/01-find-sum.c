/*
Author: Kushal Prasad Joshi
Purpose: Experiment 1 for lab 1 - Find the sum of two integers 
Date: May 4, 2026
*/

#include <stdio.h>

int main() {
    int a, b, sum;

    printf("Enter two numbers: ");
    scanf("%d %d", &a, &b);

    sum = a + b;

    printf("The sum of %d and %d is: %d\n", a, b, sum);

    return 0;
}