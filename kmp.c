#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>

int kmparr[30];

#define MAX_SIZE 3000
#define SLIDE_SIZE 20

/* Color Schemes.*/
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"


struct results
{
	char st[200];
};

/*************** Kmp Functions. ***************/

void compute(int * kmparr, char * pattern, int patternlength) {
	int m = patternlength;
	kmparr[0] = 0;
	int len = 0, i;
	for (i = 1; i < m; ++i)
	{
		if (pattern[i] == pattern[len])
		{
			len++;
			kmparr[i] = len;
		}
		else {
			if (len != 0)
			{
				len = kmparr[len - 1];
				i--;
			}
			else {
				kmparr[len] = 0;
			}
		}
	}
}

/************************ End functions. *********************/

void updateSlidingWindow(char * slide, char newChar) {
	int i = 0, len = strlen(slide);
	if (len < SLIDE_SIZE) {
		i = len;
	}
	else {
		for (; i < len - 1; i++)
			slide[i] = slide[i+1];
	}

	slide[i] = newChar;
	slide[i+1] = '\0';
}

void getLastCharsFromString(char * slide, char * result, int cut_off_length) {
	int i = 0, j = 0;
	int current_length = strlen(slide);
	while(i < current_length - cut_off_length) i++;
	while(slide[i] != '\0'){
		if (slide[i] == '\n') {
			j = 0;
			i++;
		}
		result[j++] = slide[i++];
	}
	result[j] = '\0';
}

void getNextCharactersFromString(char * text, char * result, int current_position, int cut_off_length) {
	int current_length = strlen(result);
	int i = current_position;
	printf("%d\n", i);
	int t = 0;
	while(text[i++] != '\0') {
		//if(t++ == 10)
		//	break;
		if (text[i] == '\n' || cut_off_length-- == 0)
			break;
		result[current_length++] = text[i];
	}
	result[current_length] = '\0';
}

int stateStore = 0, matches;

void findResultInBuffer(struct results * res, int tillFound, char text[], char * pattern, char * slide, int * kmpArray, int patternlength, int textlength) {
	int i = 0, j = 0;
	j = stateStore;
	for (i = 0; i < textlength; ++i)
	{
		updateSlidingWindow(slide, text[i]);
		while(1) {
			if (text[i] == pattern[j]) {
				j++;
				if (j == patternlength) {
					matches++;
					j = kmparr[j];
					char result[150];
					getLastCharsFromString(slide, result, 20);
					printf("Before = %s\n", result);
					getNextCharactersFromString(text, result, i, 20);
					//printf("%s\n", result);
					printf("After = %s\n", result);
					//res[tillFound++].st = result;
				}
				break;
			} else if (j == 0) break;
			else j = kmparr[j];
		}
	}
	stateStore = j;
}


// implement all the argument validations.
int check_args(int argc, char * argv[]) {

}

int main(int argc, char *argv[]) {

	if (argc == 3)
	{
		struct results res[100];
		clock_t begin = clock();
		
		if(access(argv[2], F_OK ) != -1 && access(argv[2], R_OK) != -1) {
			int file_descriptor = open(argv[2], O_RDWR, S_IREAD);
			char buffer[MAX_SIZE], SLIDING_WINDOW[MAX_SIZE];
			int buffer_size;

			int i = 0;
			char pattern[150];
			const int patternlength= strlen(argv[1]);

			for (i = 0; i < patternlength; ++i)
				pattern[i] = argv[1][i];

			pattern[strlen(argv[1])] = '\0';
	
			int kmparray[patternlength];
			compute(kmparr, pattern, patternlength);

			while((buffer_size = read(file_descriptor, buffer, MAX_SIZE)) > 0) {
				buffer[buffer_size] = '\0';
				findResultInBuffer(res, 0, buffer, pattern, SLIDING_WINDOW, kmparr, patternlength, buffer_size);
			}

			//const int buffer_size = read(file_descriptor, buffer, MAX_SIZE);
			// take from sysargs.
			clock_t end = clock();
			double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
			printf("Time : %lf\n", time_spent);
		}
		else {
			printf("Specify the file with corrent name and permissions.\n");
		}

	}
	else {

		// write help code.

		printf("The correct usage is - ./grep 'pattern' 'filename'\n");
		printf("Without quotes.\n");
		printf("For help, ./grep help\n");
	}

	return 0;
}
	/*
	char * text = "AAACAAAAACAACAAAAACAAACAAAAACAAACAAAAACAAACAAAAAC";


	while()

	//SLIDING_WINDOW = updateSlidingWindow(SLIDING_WINDOW, 'D');
	updateSlidingWindow(SLIDING_WINDOW, 'D');
	printf("%s\n", SLIDING_WINDOW);
	return 0;
//	int matches = 0;
//	compute();
//
//	for (int i = 0; i < strlen(pattern); ++i)
//	{
//		printf("%d\n", kmparr[i]);
//	}
//
//	int i = 0, j;
//	while(i < strlen(text)) {
//		for(j = 0; pattern[j] != '\0' && text[i+j] != '\0' && pattern[j] == text[i+j]; j++);
//		if(pattern[j] == '\0')
//			matches++;
//		i += max(1, j-kmparr[j]);
//	}
//	printf("matches - %d\n", matches);
//	return 0;
}*/