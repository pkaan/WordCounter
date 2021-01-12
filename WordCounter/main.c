#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>


//Oxford dictionary has 171476 words.
#define HASH_TABLE_SIZE 175000
//Longest official english word is 45 letters.
#define STRING_SIZE 50

int *pr;
int hash (char cr[]);
int compare(const void *a,const void *b);


int main() {
		// Pointer to a file.
		FILE *file;
		// A string that can contain special characters.
		char tempString[STRING_SIZE ];
		//Special characters.
		const char delimeter[50] = " 0123456789#�$�?%&@\"*^=-+!^�~,._:;()[]{}������";
		char *token;
		//A string without special characters. Characters are in upper case.
		char string[STRING_SIZE];
		//Indexes are hashed. Value of the index is the amount of the words.
		int *hashTable;
		//Indexes are hashed. Value of the index is a word.
		char *words[HASH_TABLE_SIZE];
		/*Is used to store the original indexes. Original indexes are needed to
		connect the words and the amounts after sorting the hashTable with the qsort().
		/Why? QSort won't trace the original indexes. */
		int *original_index;
		//Counts the unique words in a file.
		int differentWords = 0;
		//Counts all the words in a file.
		int allWords = 0;
		//User's choice.
		char choice[50];
		//If file found.
		int found = 0;
		//Calculating running time. Example found in Exercise 3.
		clock_t start,end;
		double totaltime;

		//Dynamic memory allocation
		hashTable = (int *)malloc(1000000*sizeof(int));
		original_index = (int *)malloc(1000000*sizeof(int));


		//Fills the array starting from 0.
		for(int i=0; i < HASH_TABLE_SIZE; ++i) {
	        original_index[i] = i;
	    }

		//Asks what file the user wants to open.
		while (found == 0) {
			printf("Enter the name of the txt.file: ");
			//Flushes the buffer.
			fflush(stdout);
			//Scans the user choice
			scanf("%s", choice);
			//Opens the file in read mode.
			file = fopen(choice, "r");
			if (file == NULL) {
				printf("File not found!\n");
			}
			else {
			found = 1;
			}
		}

		//Start the count
		start = clock();

	   // Scans the strings (tempString) in a file.
	   while (fscanf(file,"%s",tempString) != EOF) {
		   //Splits the tempString if any special characters. Gutenberg-tm -> Gutenberg & tm.
		   token = strtok((tempString), delimeter);
		   while( token != NULL ) {
			   for (int i = 0; i < STRING_SIZE ; i++) {
				   	   //Converts the string to the upper case letters.
					   string[i] = toupper(token[i]);
			   }
			   // Gets the hash
			   int index = hash(string);
			   // Stores the string to the hashTable.
			   //If the value of the index is 0, the value will be 1.
			   if (hashTable[index] == 0) {
				   hashTable[index] = 1;
				   // Dynamic memory allocation.
				   // Keeps track of the hashed indexes and original words.
				   words[index] = malloc(sizeof(string) + 1);
				   strcpy(words[index], string);
				   //Counts the words.
				   differentWords++;
				   allWords++;
			   } else {
				   //If the value of the index is not 0, the value will be increased by 1.
				   hashTable[index] = hashTable[index] + 1;
				   //Counts the words.
				   allWords++;
			   }
			   // A string is processed.
			   token = strtok(NULL, delimeter);
		   }
	   }

	   // Closes the file.
	   fclose(file);

	   // Assigns a pointer to the hashTable.
	   pr = hashTable;

	   // Sorts the array. Uses QuickSort algorithm. Idea taken from exercise 4.
	   qsort (original_index, HASH_TABLE_SIZE, sizeof(*original_index), compare);

	   //Stop the count.
	   end = clock();
	   totaltime = (double)(end-start)/CLOCKS_PER_SEC;

	   //Prints the 100 most common words.
	   printf("Total number of words = %d\nNumber of different words = %d\n", allWords, differentWords);
	   printf("The 100 most common words:\nWORD: NUMBER OF OCCURRENCES\n");
	   for (int i = 0; i < 100; i++) {
		   //Prints the word and the amount.
			printf("%s: %d\n", words[original_index[i]], hashTable[original_index[i]]);
		}

	   //Prints the time count.
	   printf("Running time: %lf seconds", totaltime);
	   return(0);
}

//Returns the hash.
int hash(char ch[]) {
	unsigned long hashKey = 7;
	//Generates the hash for every character. Also multiplies the hash by 33.
	//Idea taken from https://stackoverflow.com/questions/2624192/good-hash-function-for-strings.
	for (int i=0; i < strlen(ch); i++) {
		hashKey = hashKey*33 + ch[i];
	}
	return hashKey%HASH_TABLE_SIZE;
}

// Function is used in QSort. Idea taken from exercise 4.
int compare(const void *a,const void *b){
    int x = *(int *)(a);
    int y = *(int *)(b);
    //pointer pr points to the hashTable.
    //Descending order.
    return (pr[y]-pr[x]);
}
