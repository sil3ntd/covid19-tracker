#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLEN 1000

char raw_data[MAXLEN];

int get_raw_data(char *, int);
void get_tokens(char *, char *[]);
void header();
int pretty_print(char *country[], int item_num);
void print_update_time(char *);

main(int argc, char *argv[])
{
	char c; 
	int i, len, p;
	char *tokens[100];
	int count = 0;
	int cases_total = 0;		/* Total cases */
	int cases_today = 0;		/* Total cases for the day */
	int deaths_total = 0;
	int deaths_today = 0;		/* Total deaths for the day */
	int recovered_total = 0;
	int active_total = 0;
	int critical_total = 0;

	/* print header */
	header();

	/* start capturing only after the data keyword.
	 * Data are enclosed within [..]. */
	while((c = getchar()) != '[')
		;

	while((len = get_raw_data(raw_data, MAXLEN)) > 0){
		/* separate the data into tokens */
		get_tokens(raw_data, tokens);

		/* Compute the totals */
			
		cases_total += atoi(tokens[17]);
		cases_today += atoi(tokens[19]);
		deaths_total += atoi(tokens[21]);
		deaths_today += atoi(tokens[23]);
		recovered_total += atoi(tokens[25]);
		active_total += atoi(tokens[27]);
		critical_total += atoi(tokens[29]);
			

		/* Print the table */	
		p = pretty_print(tokens, count+1);	
		
		if(!p)
			;
		else
			printf("\n");
			
		++count;				/* Number of countries */
	}
	
	putchar(0x20);
	for(i = 0; i < 153; i++)
		putchar('-');
	printf("\n");

	/* Print the  total */
	printf("|%6c|", 0x20); 
	printf("%14c%6s%14c|", 0x20, "World", 0x20);
	printf("%12d%1c|", cases_total, 0x20);
	printf("%14d%1c|", cases_today, 0x20);
	printf("%13d%1c|", deaths_total, 0x20);
	printf("%15d%1c|", deaths_today, 0x20);
	printf("%16d%1c|", recovered_total, 0x20);
	printf("%13d%1c|", active_total, 0x20);
	printf("%15d%1c|", critical_total, 0x20);
	printf("\n");
	putchar(0x20);
	for(i = 0; i < 153; i++)
		putchar('-');
	printf("\n");

	/* Print the last update time */
	
	print_update_time(tokens[1]);
	

	return 0;
}

void header()
{
	int i;
	printf("\n");
	putchar(0x20);
	for(i = 0; i < 153; i++)
		putchar('-');
	printf("\n");
	printf("|%1c%4s%1c|", 0x20, "Rank", 0x20);
	printf("%13c%7s%14c|", 0x20, "Country", 0x20);
	printf(" %-11s |", "Total Cases");
	printf(" %-13s |", "Today's cases");
	printf(" %-12s |", "Total Deaths");
	printf(" %-14s |", "Today's Deaths");
	printf(" %-15s |", "Total recovered");
	printf(" %-12s |", "Total active");
	printf(" %-14s |", "Total critical");
	printf("\n");
	putchar(0x20);
	for(i = 0; i < 153; i++)
		putchar('-');
	printf("\n");
}

/* get_raw_data: read each character from the and save to s */
int get_raw_data(char s[], int max)
{
	int i, c;

	i = 0;
	while(i < max && (c = getchar()) != '}' && c != EOF){
		if(c == ']') /* test for end of data indicator */
			return -1;
		s[i++] = c;
	}
	s[i++] = c;

	while(i < max && (c = getchar()) != '}' && c != EOF)
		s[i++] = c;
	s[i++] = c;
	s[i++] = '\0';

	return i;
}

/* get_tokens: separate the raw data in s into tokens and save
 * each token (pointer to a char) to t */

void get_tokens(char *s, char *t[])
{
	char *tok;
	int i = 0;

	i = 0;
	tok = strtok(s, ":");
	t[i++] = tok;
	while(tok != NULL){
		tok = strtok(NULL, ",");
		t[i++] = tok;
		tok = strtok(NULL, ":");
		t[i++] = tok;
	}
}

int pretty_print(char *s[], int item)
{
	char *Id;			/* country Id code */

	Id = strtok(s[5], ":");
	Id = strtok(NULL, ":");
	/* Skips printing data for countries whose name has a character
	 * which cannot be represented by the char data type. Solution is
	 * part of todo. Comment the if statement to view all.*/
	
	if(!strcmp(Id, "638") || !strcmp(Id, "384") || !strcmp(Id, "531"))
		return 0;
	

	printf("|%1c%4d%1c|", 0x20, item, 0x20); 
	printf("%2c%-32s|", 0x20, strtok(s[3], "\""));
	printf("%12d%1c|", atoi(s[17]), 0x20);
	printf("%14d%1c|", atoi(s[19]), 0x20);
	printf("%13d%1c|", atoi(s[21]), 0x20);
	printf("%15d%1c|", atoi(s[23]), 0x20);
	printf("%16d%1c|", atoi(s[25]), 0x20);
	printf("%13d%1c|", atoi(s[27]), 0x20);
	printf("%15d%1c|", atoi(s[29]), 0x20);

	return 1;
}

#include <time.h>

void print_update_time(char *s)
{
	time_t update_time;
	struct tm *pstr_update_time;
	
	/* calendar time in JSON is in milliseconds since Jan 1, 1970
	 * while calendar time in C is in seconds since Jan 1, 1970 */
	update_time = strtol(s, NULL, 10) / 1000; 	

	pstr_update_time = gmtime(&update_time);

	printf("\nLast updated on: ");
	printf("(UTC) %s\n", asctime(pstr_update_time));

	printf("\n");
												   
}
