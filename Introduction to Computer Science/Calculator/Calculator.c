#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "stack.h"

#define DEBUG 0
#define EULER 2.71828182846

typedef struct node NODE;
struct node {

       char content[5];
       NODE* parent;
       NODE* left;
       NODE* right;

       int isBracket;
};

NODE* New_Node() {

       NODE* node = malloc(sizeof(NODE));
       strcpy(node->content, "\0");

       node->parent = NULL;
       node->left = NULL;
       node->right = NULL;

       node->isBracket = 0;
       return node;
}

void Tree_Destroy(NODE* node) {

       if (node->left != NULL) Tree_Destroy(node->left);
       if (node->right != NULL) Tree_Destroy(node->right);

       free(node);
       return;
}

char* Tree_Solve(NODE* node) {

       //printf("%s\n", node->content);
       if (node->left != NULL) Tree_Solve(node->left);
       if (node->right != NULL) Tree_Solve(node->right);

       if (strcmp(node->content, "\0") == 0) {
              strcpy(node->content, node->left->content);

              free(node->left);
              if (node->right != NULL) free(node->right);

              return node->content;
       }

       float ans = 0;

       //IS NUMBER
       if (node->right == NULL
       && (strcmp(node->content, "sqrt") != 0
       && strcmp(node->content, "exp") != 0
       && strcmp(node->content, "log") != 0)) {
              free(node->left);
              return node->content;
       }

       if (strcmp(node->content, "sqrt") == 0) ans = sqrt(atof(node->left->content));
       else if (strcmp(node->content, "exp") == 0) ans = pow(EULER, atof(node->left->content));
       else if (strcmp(node->content, "log") == 0) ans = log2(atof(node->left->content));
       else if (strcmp(node->content, "+") == 0) ans = atof(node->left->content) + atof(node->right->content);
       else if (strcmp(node->content, "-") == 0) ans = atof(node->left->content) - atof(node->right->content);
       else if (strcmp(node->content, "*") == 0) ans = atof(node->left->content) * atof(node->right->content);
       else if (strcmp(node->content, "/") == 0) ans = atof(node->left->content) / atof(node->right->content);
       else if (strcmp(node->content, "**") == 0) ans = pow(atof(node->left->content), atof(node->right->content));

       char newContent[50];
       sprintf(newContent, "%f", ans);
       strcpy(node->content, newContent);

       free(node->left);
       if (node->right != NULL) free(node->right);

       return node->content;
}

void Error(PILHA* p, NODE* node) {

       printf("Expressao incorreta.\n");

       while (node->parent != NULL)
              node = node->parent;

       Tree_Destroy(node);
       Pilha_Limpar(p, 0);

       return;
}

int main(int argc, char const *argv[]) {

       PILHA* p = Pilha_Criar();

       //PRIORITY OPERATION
       char** priority = malloc (8 * sizeof(char*));

       priority[0] = malloc (5 * sizeof(char));
       priority[1] = malloc (4 * sizeof(char));
       priority[2] = malloc (4 * sizeof(char));

       strcpy(priority[0], "sqrt");
       strcpy(priority[1], "exp");
       strcpy(priority[2], "log");

       for (int i = 3; i < 8; i++) {

              priority[i] = malloc(3 * sizeof(char));

              scanf("%s\n", priority[i]);
              if (DEBUG) printf("%s\n", priority[i]);
       }
       if (DEBUG) printf("\n");

       NODE* currentOper = New_Node();

       //BUILD BINARY TREE
       for (char c; scanf("%c\n", &c) != EOF;) {

              if (c == ',' || c == ';'){

                     if (!Pilha_Vazia(p)) {

                            Error(p, currentOper);
                            currentOper = New_Node();
                            continue;
                     }
                     else if (DEBUG) printf("\n");

                     while (currentOper->parent != NULL) currentOper = currentOper->parent;

                     //SOLVE BINARY TREE
                     float ans = atof(Tree_Solve(currentOper));
                     if (isnan(ans)) printf("Expressao incorreta.\n");
                     else printf("%.2f\n", ans);

                     free(currentOper);
                     currentOper = New_Node();

                     if (c == ';') break;

                     continue;
              }

              //OPEN BRACKETS
              if (c == '(' || c == '[' || c == '{') {

                     Pilha_Empilhar(p, c);

                     NODE* newOper = New_Node();
                     newOper->parent = currentOper;

                     newOper->isBracket = 1;

                     if (currentOper->left == NULL) currentOper->left = newOper;
                     else currentOper->right = newOper;

                     currentOper = newOper;
              }
              //CLOSE BRACKETS
              else if (c == ')' || c == ']' || c == '}') {
                     if (!Pilha_Vazia(p)
                     && ((Pilha_ItemTopo(p) == '(' && c == ')')
                     || (Pilha_ItemTopo(p) == '[' && c == ']')
                     || (Pilha_ItemTopo(p) == '{' && c == '}'))) {

                            Pilha_Desempilhar(p);

                            while(!currentOper->isBracket) {

                                   if (currentOper->parent == NULL) {

                                          NODE* newBracket = New_Node();
                                          newBracket->left = currentOper;
                                          currentOper->parent = newBracket;
                                          break;
                                   }

                                   currentOper = currentOper->parent;
                            }

                            currentOper = currentOper->parent;
                     }
                     //ERROR
                     else {
                            Error(p, currentOper);
                            scanf("%*[^\n]\n");

                            currentOper = New_Node();
                            continue;
                     }
              }
              //UNARY OPERATION
              else {
                     if (c == 's' || c == 'e' || c == 'l') {

                            NODE* newOper = New_Node();
                            newOper->parent = currentOper;

                            if (currentOper->left == NULL)
                                   currentOper->left = newOper;
                            else
                                   currentOper->right = newOper;

                            if (c == 's') strcpy(newOper->content, "sqrt");
                            if (c == 'e') strcpy(newOper->content, "exp");
                            if (c == 'l') strcpy(newOper->content, "log");

                            scanf("%*[^([{]\n");

                            currentOper = newOper;

                            if (DEBUG) printf("%s ", currentOper->content);
                     }

                     //BINARY OPERATION
                     else {
                            if (c == '+' || c == '-' || c == '*' || c == '/') {

                                   //CHECK ERRORS
                                   if (scanf("%[+/-]\n", &c) == 1 || (c != '*' && scanf("%[*]\n", &c) == 1)) {

                                          Error(p, currentOper);
                                          scanf("%*[^\n]\n");

                                          currentOper = New_Node();
                                          continue;
                                   }

                                   // EMPTY NODE
                                   if (strcmp(currentOper->content, "\0") == 0) {

                                          char str[3];
                                          if (scanf("%[*]\n", str) == 1) {
                                                 if (strcmp(str, "*") != 0) {

                                                        Error(p, currentOper);
                                                        scanf("%*[^\n]\n");

                                                        currentOper = New_Node();
                                                        continue;
                                                 }
                                                 else strcpy(currentOper->content, "**");
                                          }
                                          else strcpy(currentOper->content, (char[2]) { c, '\0'} );
                                   }
                                   else {
                                          NODE* newOper = New_Node();

                                          if (scanf("%[*]", &c) == 1) strcpy(newOper->content, "**");
                                          else strcpy(newOper->content, (char[2]) { c, '\0'} );

                                          for (int i = 0; i < 8; i++) {

                                                 //CURRENT NODE HAS PRIORITY
                                                 if (strcmp(priority[i], currentOper->content) == 0) {

                                                        newOper->left = currentOper;

                                                        if (currentOper->isBracket) {

                                                               newOper->parent = currentOper->parent;
                                                               if (currentOper->parent != NULL) {
                                                                      if (currentOper->parent->left == currentOper) currentOper->parent->left = newOper;
                                                                      else currentOper->parent->right = newOper;
                                                               }

                                                               currentOper->isBracket = 0;
                                                               newOper->isBracket = 1;
                                                        }
                                                        else if (currentOper->parent != NULL) {

                                                               if (strcmp(currentOper->parent->content, "\0") == 0) {
                                                                      if (scanf("%[*]\n", &c) == 1) strcpy(currentOper->parent->content, "**");
                                                                      else strcpy(currentOper->parent->content, (char[2]) { c, '\0'} );
                                                                      free(newOper);

                                                                      newOper = currentOper->parent;
                                                                      break;
                                                               }
                                                               else{
                                                                      for (int j = 0; j < 8; j++) {

                                                                             if (strcmp(newOper->content, priority[j]) == 0) break;

                                                                             if (strcmp(currentOper->parent->content, priority[j]) == 0) {

                                                                                    currentOper = currentOper->parent;
                                                                                    newOper->left = currentOper;

                                                                                    break;
                                                                             }
                                                                      }
                                                               }

                                                               newOper->parent = currentOper->parent;
                                                               if (currentOper->parent != NULL) currentOper->parent->right = newOper;
                                                        }

                                                        currentOper->parent = newOper;

                                                        break;
                                                 }

                                                 // NEW NODE HAS PRIORITY
                                                 else if (strcmp(priority[i], newOper->content) == 0) {

                                                        newOper->parent = currentOper;
                                                        if (currentOper->left == NULL) currentOper->left = newOper;
                                                        else if (currentOper->right == NULL) currentOper->right = newOper;
                                                        else {

                                                               newOper->left = currentOper->right;
                                                               newOper->left->parent = newOper;
                                                               currentOper->right = newOper;
                                                        }

                                                        break;
                                                 }
                                          }

                                          currentOper = newOper;
                                   }

                                   if (DEBUG) printf("%s ", currentOper->content);
                            }

                            //NUMBER
                            else {

                                   char str[50];
                                   str[0] = '\0';

                                   scanf("%[0-9.]\n", str);

                                   if (scanf("%[([{]\n", &c) == 1) {
                                          Error(p, currentOper);
                                          scanf("%*[^\n]\n");

                                          currentOper = New_Node();
                                          continue;
                                   }

                                   char number[50];

                                   sprintf(number, "%c%s", c, str);
                                   NODE* childNode = New_Node();

                                   strcpy(childNode->content, number);

                                   childNode->parent = currentOper;

                                   if (currentOper->left == NULL) currentOper->left = childNode;
                                   else currentOper->right = childNode;

                                   if (DEBUG) printf("%s ", childNode->content);
                            }
                     }
              }
       }

       for (int i = 0; i < 8; i++) free(priority[i]);
       free(priority);

       free(currentOper);
       Pilha_Limpar(p, 1);

       return 0;
}
