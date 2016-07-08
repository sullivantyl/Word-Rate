/*
 *
 * Word count and frequency analyzer program
 * for Operating Systems course.
 *
 * By: Tyler Sullivan
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* statistically negligable number of words longer than 32 characters but can be changed if needed */
#define MAX_WORD_LENGTH 32

/* struct to build list of and contain word information */
typedef struct typeWord
{
  char word[MAX_WORD_LENGTH];
  int count;
  double freq;
} word_t;

/* qsort compare function for descending order */
int cmpr(const void* a, const void *b)
{
  word_t *fa = (word_t*)a;
  word_t *fb = (word_t*)b;
  return (int)(100 * fb->freq - 100 * fa ->freq);
}

int main (int argc, char* args[])
{
  char* words = NULL;
  char* words_extend = NULL;
  int inchar;
  int wordcount = 0;
  int wordscount = 0;
  int charcount = 0;
  char* token;
  char* uptoken;
  word_t* wordList = NULL;
  word_t* wordList_extend = NULL;
  int i = 0;
  int confirm = 0;

  FILE *infile;

  if (argc != 2){
		fprintf(stderr, "USAGE: %s input_file\n", args[0]);
		return EXIT_FAILURE;
	}

  if (NULL == (infile = fopen(args[1],"r"))){
		perror("input_file_error");
		return EXIT_FAILURE;
	}

/* gathers information from unknown length lines in file char by char */
  while ((inchar = fgetc(infile)) != EOF){
    if (isalpha(inchar) || isspace(inchar)){
      if (inchar == '\n') inchar = ' ';
      charcount++;
      words_extend = (char*) realloc (words, charcount * sizeof(char));
      if (words_extend != NULL){
        words = words_extend;
        words[charcount-1] = inchar;
      } else {
        perror("malloc");
        free(words);
      }
    }
  }

  fclose(infile);

/* begin token extraction, word list build, and comparison */
  token = strtok(words, " ");
  wordcount = 1;
  uptoken = token;
  while(token != NULL){
    wordcount++;
    confirm = 0;
    for (i = 0; i < sizeof(token); i++){
      uptoken[i] = toupper(token[i]);
    }
/* checks if word exists in list already and sets condition to skip adding new word if found */
    for (i = 0; i < wordscount; i++){
      if (wordList != NULL){
        if (0 == strncmp(uptoken, wordList[i].word, MAX_WORD_LENGTH)){
          wordList[i].count++;
          confirm = 1;
        }
      }
    }
/* adds new word to word list */
    if (confirm == 0){
      wordscount++;
      wordList_extend = realloc (wordList, wordscount * sizeof(word_t));
      if (wordList_extend != NULL){
        wordList = wordList_extend;
        strncpy(wordList[wordscount-1].word, uptoken, MAX_WORD_LENGTH);
        wordList[wordscount-1].count = 1;
      } else {
        perror("malloc");
        free(words);
        free(wordList);
        return EXIT_FAILURE;
      }
    }
    token = strtok(NULL, " ");
  }
/* frees "words" since it is not used further */
  free(words);

/* calculates frequency of words in list */
  for (i = 0; i < wordscount; i++){
    wordList[i].freq = (double) wordList[i].count / (double) wordcount;
  }

/* sorts in descending order */
  qsort(wordList, wordscount, sizeof(word_t), cmpr);

/* output word followed by its frequency */
  for(i = 0; i < wordscount; i++){
    printf("%16s:  %f\n", wordList[i].word, wordList[i].freq);
  } /* formatting here is different than MAX_WORD_LENGTH for aesthetic purposes */

/* frees remaining allocated memory */
  free(wordList);
  return EXIT_SUCCESS;
}
