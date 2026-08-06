#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_RULES 20
#define MAX_PROD 10
#define MAX_SYMBOLS 10
// Structure to represent a grammar production rule
typedef struct
{
    char head;
    char body[MAX_PROD][MAX_SYMBOLS];
    int prod_count;
} GrammarRule;
GrammarRule grammar[MAX_RULES];
int rule_count = 0;
// Arrays to store FIRST and FOLLOW sets
char first_sets[26][MAX_SYMBOLS * 2];
char follow_sets[26][MAX_SYMBOLS * 2];
// Helper to check if a symbol is a Non-Terminal (Uppercase letters) 
bool is_non_terminal(char symbol)
{
    return (symbol >= 'A' && symbol <= 'Z');
}

// Helper to add a terminal to a set uniquely
bool add_to_set(char set[], char val)
{
    int len = strlen(set);
    for (int i = 0; i < len; i++)
    {
        if (set[i] == val)
            return false;
    }
    set[len] = val;
    set[len + 1] = '\0';
    return true;
}

// Helper to merge set2 into set1 (ignoring epsilon 'e')
bool merge_sets(char set1[], char set2[], bool ignore_epsilon)
{
    bool changed = false;
    int len2 = strlen(set2);
    for (int i = 0; i < len2; i++)
    {
        if (ignore_epsilon && set2[i] == 'e')
            continue;
        if (add_to_set(set1, set2[i]))
        {
            changed = true;
        }
    }
    return changed;
}

// Helper to check if a set contains epsilon ('e')
bool has_epsilon(char set[])
{
    for (int i = 0; set[i] != '\0'; i++)
    {
        if (set[i] == 'e')
            return true;
    }
    return false;
}

// Find rule index for a given non-terminal character
int find_rule_index(char head)
{
    for (int i = 0; i < rule_count; i++)
    {
        if (grammar[i].head == head)
            return i;
    }
    return -1;
}

void compute_first()
{
    bool changed = true;
    while (changed)
    {
        changed = false;

        for (int i = 0; i < rule_count; i++)
        {
            char head = grammar[i].head;
            int idx = head - 'A';

            for (int j = 0; j < grammar[i].prod_count; j++)
            {
                char *prod = grammar[i].body[j];
                int prod_len = strlen(prod);

                if (prod[0] == 'e')
                { // Epsilon rule
                    if (add_to_set(first_sets[idx], 'e'))
                        changed = true;
                }
                else
                {
                    bool next_symbol = true;
                    for (int k = 0; k < prod_len &&
                                    next_symbol;
                         k++)
                    {
                        char sym = prod[k];
                        if (!is_non_terminal(sym))
                        {
                            if (add_to_set(first_sets[idx],
                                           sym))
                                changed = true;
                            next_symbol = false;
                        }
                        else
                        {
                            int sym_idx = sym - 'A';
                            if (merge_sets(first_sets[idx],
                                           first_sets[sym_idx], true))
                                changed = true;
                            if (!has_epsilon(first_sets[sym_idx]))
                            {
                                next_symbol = false;
                            }
                        }
                    }
                    if (next_symbol)
                    {
                        if (add_to_set(first_sets[idx],
                                       'e'))
                            changed = true;
                    }
                }
            }
        }
    }
}

void compute_follow(char start_symbol)
{
    // Rule 1: Add '$' to FOLLOW of start symbol
    add_to_set(follow_sets[start_symbol - 'A'], '$');

    bool changed = true;
    while (changed)
    {
        changed = false;

        for (int i = 0; i < rule_count; i++)
        {
            char head = grammar[i].head;
            int head_idx = head - 'A';

            for (int j = 0; j < grammar[i].prod_count; j++)
            {
                char *prod = grammar[i].body[j];
                int prod_len = strlen(prod);

                for (int k = 0; k < prod_len; k++)
                {
                    char current_sym = prod[k];
                    if (is_non_terminal(current_sym))
                    {
                        int cur_idx = current_sym - 'A';
                        bool next_symbol = true;

                        // Look at all symbols following the current non-terminal
                        for (int n = k + 1; n < prod_len && next_symbol; n++)
                        {
                            char next_sym = prod[n];
                            if (!is_non_terminal(next_sym))
                            {
                                if (add_to_set(follow_sets[cur_idx], next_sym))
                                    changed = true;
                                next_symbol = false;
                            }
                            else
                            {
                                int next_idx = next_sym - 'A';
                                if (merge_sets(follow_sets[cur_idx], first_sets[next_idx], true))
                                    changed = true;
                                if (!has_epsilon(first_sets[next_idx]))
                                {
                                    next_symbol = false;
                                }
                            }
                        }
                        // If everything after it can be epsilon, or it's the last token
                        if (next_symbol)
                        {
                            if (merge_sets(follow_sets[cur_idx], follow_sets[head_idx], false))
                                changed = true;
                        }
                    }
                }
            }
        }
    }
}

int main()
{
    // Initialize sets with empty strings
    for (int i = 0; i < 26; i++)
    {
        first_sets[i][0] = '\0';
        follow_sets[i][0] = '\0';
    }

    // Grammar from your notes:
    // S -> C C
    // C -> c C | d
    // Note: 'e' represents epsilon

    // Add rule for S
    grammar[rule_count].head = 'S';
    strcpy(grammar[rule_count].body[0], "CB");
    grammar[rule_count].prod_count = 1;
    rule_count++;

    // Add rule for C
    grammar[rule_count].head = 'C';
    strcpy(grammar[rule_count].body[0], "cC");
    strcpy(grammar[rule_count].body[1], "d");
    grammar[rule_count].prod_count = 2;
    rule_count++;

    // Add rule for B
    grammar[rule_count].head = 'B';
    strcpy(grammar[rule_count].body[0], "bC");
    // strcpy(grammar[rule_count].body[1], "e");
    grammar[rule_count].prod_count = 2;
    rule_count++;

    char start_symbol = 'S';

    // Computations
    compute_first();
    compute_follow(start_symbol);

    // Print Results
    printf("--- FIRST SETS ---\n");
    for (int i = 0; i < rule_count; i++)
    {
        char head = grammar[i].head;
        printf("FIRST(%C) = { ", head);
        char *s = first_sets[head - 'A'];
        for (int j = 0; s[j] != '\0'; j++)
        {
            printf("%c%s", s[j], (s[j + 1] != '\0') ? ", " : "");
        }
        printf(" }\n");
    }

    printf("\n--- FOLLOW SETS ---\n");
    for (int i = 0; i < rule_count; i++)
    {
        char head = grammar[i].head;
        printf("FOLLOW(%C) = { ", head);
        char *s = follow_sets[head - 'A'];
        for (int j = 0; s[j] != '\0'; j++)
        {
            printf("%c%s", s[j], (s[j + 1] != '\0') ? ", " : "");
        }
        printf(" }\n");
    }

    return 0;
}