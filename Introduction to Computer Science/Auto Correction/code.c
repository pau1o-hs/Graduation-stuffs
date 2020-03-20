#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define DEBUG 0
#define MAXWords 600000
#define MAXLetters 50

typedef struct dictionary {

       char (*words)[MAXLetters];
       int length;
}DICTIONARY;

void MergeSort (DICTIONARY* dict, int l, int r) {

       if (l >= r)
              return;

       int mid = (l + r) / 2;

       MergeSort(dict, l, mid);
       MergeSort(dict, mid + 1, r);

       DICTIONARY temp;
       temp.words = malloc (MAXWords * sizeof(*temp.words));

       int i = l, j = mid + 1, k = 0;

       while (i <= mid && j <= r) {

              if (strcmp(dict->words[i], dict->words[j]) < 0)
                     strcpy(temp.words[k++], dict->words[i++]);
              else
                     strcpy(temp.words[k++], dict->words[j++]);
       }

       while (i <= mid)
              strcpy(temp.words[k++], dict->words[i++]);

       while (j <= r)
              strcpy(temp.words[k++], dict->words[j++]);

       for (i = l, k = 0; i <= r; i++, k++)
              strcpy(dict->words[i], temp.words[k]);

       free(temp.words);
}

int BinarySearch(DICTIONARY dict, char* key) {

       int l = 0, r = dict.length;

       while (l <= r) {

              int mid = (l + r) / 2;

              if (strcmp(dict.words[mid], key) == 0) return 1;
              if (strcmp(dict.words[mid], key) > 0) r = mid - 1;
              else l = mid + 1;
       }

       return 0;
}

DICTIONARY AdjustDict (DICTIONARY dict) {

       DICTIONARY temp;
       temp.words = malloc (MAXWords * sizeof(*temp.words));
       temp.length = 0;

       for (int i = 0; i < dict.length; i++) {

              if (!BinarySearch(temp, dict.words[i]))
                     strcpy(temp.words[temp.length++], dict.words[i]);
       }

       return temp;
}

int main(int argc, char const *argv[]) {

       char bookFilename[50];
       scanf("%s", bookFilename);

       char tweetFilename[50];
       scanf("%s", tweetFilename);

       FILE *bookFile, *tweetsFile;
       bookFile = fopen(bookFilename, "r");
       tweetsFile = fopen(tweetFilename, "r");

       DICTIONARY bookWords, dict;
       bookWords.words = malloc (MAXWords * sizeof(*bookWords.words));
       bookWords.length = 0;

       //----------------------------------------------------------------------------------------------------------------------------------
       fscanf(bookFile, "%*[^a-zA-Z]");
       while (fscanf(bookFile, "%[a-zA-Z]%*[^a-zA-Z]", bookWords.words[bookWords.length++]) == 1) {

              for (int i = 0; bookWords.words[bookWords.length - 1][i] != '\0'; i++)
                     bookWords.words[bookWords.length - 1][i] = tolower(bookWords.words[bookWords.length - 1][i]);

              continue;
       }

       MergeSort(&bookWords, 0, bookWords.length);
       dict = AdjustDict(bookWords);                                                                          //CREATE DICTIONARY
       free(bookWords.words);

       if (DEBUG)
              for (int i = 0; i < dict.length; i++)
                     printf("|%s| ", dict.words[i]);

       if (DEBUG) printf("\n\n");
       // //----------------------------------------------------------------------------------------------------------------------------------
       char tweet[50];
       bool noTweets = true;

       while (fscanf(tweetsFile, "%s", tweet) == 1) {

              if (DEBUG) printf("%s\n", tweet);
              if (strcmp(tweet, "\"text\":") != 0)
                     continue;

              noTweets = false;

              fscanf(tweetsFile, "%*[^\"]%*[\"]");
              while (fscanf(tweetsFile, "%50[a-zA-Z]%*[^a-zA-Z\"]", tweet) == 1) {

                     char temp[100];                                                                        //GET TWEETS
                     strcpy(temp, tweet);

                     for (int i = 0; tweet[i] != '\0'; i++)
                            tweet[i] = tolower(tweet[i]);

                     if (DEBUG) printf("|%s| ", tweet);

                     if (!BinarySearch(dict, tweet)) {
                            printf("%s ", temp);
                     }
              }

              printf("\n");
              fscanf(tweetsFile, "\"%*[^\"]%*[\"]");
       }
       //----------------------------------------------------------------------------------------------------------------------------------

       if (noTweets)
              printf("No tweets to check");

       free(dict.words);
       fclose(bookFile);
       fclose(tweetsFile);
       return 0;
}
