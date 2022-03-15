#define _CRT_SECURE_NO_WARNINGS
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

void Reading_and_creating_array_function(char words[], float distribution[], char* argument, int* number_items, int* number_letters)
{
	FILE* file = fopen(argument, "r");
	
	float dstr;
	int index = 0;
	char letter;

	while (!feof(file))
	{
		if (fscanf(file, "%f", &dstr))
		{
			distribution[index] = dstr;
			distribution[index] = int(10 * distribution[index]);
			distribution[index] /= 10;
			index++;
		}
		else
		{
			while ((letter = fgetc(file)) != EOF)
			{
				if (isalpha(letter))
				{
					*number_letters += 1;
				}
				strcpy(&words[*number_items], &letter);
				*number_items += 1;
			}
			
		}
	}

}

void frequency(char words[], float frequency_vector[], int number_items, int number_letters)
{
	for (int i = 0; i < number_items; i++)
	{
		if (words[i] >= 'a' && words[i] <= 'z')
		{
			frequency_vector[words[i] - 97] += 1;
		}
		else if (words[i] >= 'A' && words[i] <= 'Z')
		{
			frequency_vector[words[i] - 65] += 1;
		}
	}

	for (int i = 0; i < 26; i++)
	{
		frequency_vector[i] = (frequency_vector[i] / number_letters) * 100 ;
	}
}

void Chi_squared_distance(float distribution[], float frequency_vector[], float* distance)
{
	for (int index = 0; index < 26; index++)
	{
		*distance += pow((frequency_vector[index] - distribution[index]), 2);
		*distance /= distribution[index];
	}
}

void break_code(char words[], float distribution[], float frequency_vector[], int number_items, int* shiftss)
{
	float minim_sum = 100;
	float minim_shift = 200;
	float last_element = 0;
	int shift = 1;
	float temp;
	float distance = 0;
	for (int i = 0; i < 25; i++)
	{
		shift = 1;
		while (shift > 0)
		{
			temp = frequency_vector[25];
			for (int index = 25; index > 0; index--)
			{
				frequency_vector[index] = frequency_vector[index - 1];
			}
			frequency_vector[0] = temp;
			shift--;
		}

		Chi_squared_distance(distribution, frequency_vector, &distance);
		if (distance < minim_shift)
		{
			minim_shift = distance;
		}
		distance = 0;
	}

	int shifts = (int)sqrt(minim_shift) + 1;
	for (int index = 0; index < number_items; index++)
	{
		if (isalpha(words[index]))
		{
			if (words[index] >= 'a' && words[index] <= 'z')
			{
				words[index] = ((words[index] - 97 + shifts) % 26) + 97;
			}
			else if (words[index] >= 'A' && words[index] <= 'Z')
			{
				words[index] = ((words[index] - 65 + shifts) % 26) + 65;
			}
		}
	}
	*shiftss = shifts; 
}

int main(int argc, char* argv[])
{
	// variables for the program;
	int choice;
	float distribution[100];
	_Post_ _Notnull_ char* words;
	int n = 300000;
	int number_letters = 0;
	int number_items = 0;
	float distance = 0;
	float frequency_vector[27];
	int uppers = 0;
	int lowers = 0;
	int shiftss = 0;

	for (int i = 0; i < 26; i++)
	{
		frequency_vector[i] = 0;
	}

	words = (char *)malloc(sizeof(char) * n);

	do
	{
		printf("	***Main Menu*** \n");
		printf("0.Run all functions of the program\n");
		printf("1.Display the encoded message\n");
		printf("2.Display the number of letters and the number of characters\n");
		printf("3.Display the letters distribution in English alphabet\n");
		printf("4.Display the letters distribution in the encoded message\n");
		printf("5.Display the Chi-Square distance between distribution in English alphabet and letters distribution in the encoded text\n");
		printf("6.Display the text decoded\n");
		printf("7.More information about Caeser Cipher and Chi-Squared distance\n");
		printf("8.More information about the program\n");
		printf("9.Run tests\n");
		printf("10.Exit the program\n");
		printf("\n");
		printf("Your command is:");
		scanf("%d", &choice);

		switch (choice)
		{
		case 0:
			Reading_and_creating_array_function(words, distribution, argv[1], &number_items, &number_letters);
			frequency(words, frequency_vector, number_items, number_letters);
			Chi_squared_distance(distribution, frequency_vector, &distance);
			break;

		case 1:
			printf("The encoded text is: ");
			for (int index = 0; index < number_items; index++)
			{
				printf("%c", words[index]);
			}
			printf("\n");
			printf("\n");
			break;

		case 2:
			printf("The number of letter is %d and the numbers of characters is %d\n", number_letters, number_items);
			printf("\n");
			printf("\n");
			break;

		case 3:
			printf("The the letters distribution in English alphabet is: ");
			for (int i = 0; i < 26; i++)
			{
				printf("%c: %.1f  ", (i + 97), distribution[i]);
			}
			printf("\n");
			printf("\n");
			break;

		case 4:
			printf("The the letters distribution in encoded message is: ");
			for (int i = 0; i < 26; i++)
			{
				printf("%c: %.1f  ", (i + 97), frequency_vector[i]);
			}
			printf("\n");
			printf("\n");
			break;

		case 5:
			printf("The Chi-Square distance between distribution in English alphabet and letters distribution in the encoded text is: %.1f", sqrt(distance));
			printf("\n");
			printf("\n");
			break;

		case 6:
			break_code(words, distribution, frequency_vector, number_items, &shiftss);
			printf("The decoded text is: ");
			for (int index = 0; index < number_items; index++)
			{
				printf("%c", words[index]);
			}
			printf("\n");
			printf("The shift with which the code was coded is: %d\n", shiftss);
			printf("\n");
			printf("\n");
			break;

		case 7:
			printf("The Caesar Cipher technique is one of the earliest and simplest method of encryption technique.\n");
			printf("It’s simply a type of substitution cipher, i.e., each letter of a given text is replaced by a letter\n");
			printf("some fixed number of positions down the alphabet. For example with a shift of 1, A would be replaced by B\n");
			printf("B would become C, and so on. The method is apparently named after Julius Caesar, who apparently used it to\n");
			printf("communicate with his officials.\n");
			printf("\n");
			printf("Basically, for each possible character (i goes from a to z), we measure the discrepancy between how often\n"); 
			printf("it appeared in the encrypted text C(i) and how often it is expected to appear in English texts E(i);\n");
			printf("the difference C(i) - E(i)is squared such that we remove negative signs.The division by E(i) is simply\n");
			printf("a normalization factor.The lower the squared Chi-square distance (C,E), the more similar the histograms C and E are.\n");
			printf("compute the histogram for all possible shifts, and compute the Chi Squared distance between these histograms and the\n");
			printf("average distribution of the characters in English. The shift with the lowest Chi Squared distance is the solution\n");
			printf("\n");
			break;

		case 8:
			printf("The program will break the message encoded with Caesar's Cipher using the Chi-Squared distance.\n");
			printf("The program is divided into several functions for different purposes. In the Reading_and_creating_array_function()\n");
			printf("we read the distribution of the letters from a file and stores it into an array and in another array stores the encoded message\n");
			printf("In the function frequency(), we calculate the frequency of the letters in the coded text and in the function Chi-Squared_distance()\n");
			printf("we calculate the Chi-Square distance between two histograms, one being the average distribution of the characters in English\n");
			printf("In the function break_code(), we break the message and calculate the shift used in Caesar's cipher coding\n");
			printf("\n");
			break;

		case 9:
			if (number_letters == 217 && number_items == 286)
			{
				printf("Test for command 2 is passed successfully\n");
				printf("\n");
			}
			else
			{
				printf("Test for command 2 is failed\n");
				printf("\n");
			}

			if (int(sqrt(distance)) == 19)
			{
				printf("Test for command 5 is passed successfully\n");
				printf("\n");
			}
			else
			{
				printf("Test for command 5 is failed\n");
				printf("\n");
			}

			if (shiftss == 14)
			{
				printf("Test for command 6 is passed successfully\n");
				printf("\n");
			}
			else
			{
				printf("Test for command 6 is failed\n");
				printf("\n");
			}
			break;

		case 10:
			printf("Thank you for using my program. Have a nice day!\n");
			printf("\n");
			return 0;
			break;
		}


	} while (choice != 11);
	free(words);
	return 0;
}
