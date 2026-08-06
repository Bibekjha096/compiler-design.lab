/*
Author: Kushal Prasad Joshi
Purpose: Experiment 2 for lab 1 - Find the area of a circle
Date: May 4, 2026
*/

#include <stdio.h>

#define PI 3.14159

int main() {
    double radius, area;

    printf("Enter the radius of the circle: ");
    scanf("%lf", &radius);

    area = PI * radius * radius;

    printf("The area of the circle with radius %.2lf is: %.2lf\n", radius, area);

    return 0;
}