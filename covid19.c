#include <stdio.h>
#include <string.h>

#define MAXLEN 1000

char str[MAXLEN];

void header();
void print_details(char *country[]);
int pretty_print(char *country[], int item_num);

main(int argc, char *argv[])
{
	char c; 
	int i, len;
	char *t;
	char *data[100];
	int j;
	int count = 0;
	int p;
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

	i = 0;
	while((len = get_data_per_country(str, MAXLEN)) > 0){
		/* write details per country to the data array */
		t = strtok(str, ":");
		data[i++] = t;
		while(t != NULL){
			t = strtok(NULL, ",");
			data[i++] = t;
			t = strtok(NULL, ":");
			data[i++] = t;
		}

		/* Compute the totals */
		cases_total += atoi(data[17]);
		cases_today += atoi(data[19]);
		deaths_total += atoi(data[21]);
		deaths_today += atoi(data[23]);
		recovered_total += atoi(data[25]);
		active_total += atoi(data[27]);
		critical_total += atoi(data[29]);
	
		/* To see the detailed information saved in the data array
		 * uncomment the lines starting from the for-statement  up to
		 * the return-statement. */
		/*			
		for(j = 0; j < i; ++j)
			printf("data[%d]: %s\n", j, data[j]);
		return 0;
		*/

		p = pretty_print(data, count+1);	
		
		if(!p)
			;
		else
			printf("\n");
		
		/* To see the information per country in another form,
		 * uncomment the line starting from the print_details()
		 * function call up to the printf statement. */
		/*
		print_details(data);
		printf("\n");
		*/


		/* reinitialize index to 0 to save the information for
		 * another country. */
		i = 0;

		++count;		/* Represents the number of countries */
	}

	putchar(0x20);
	for(i = 0; i < 153; i++)
		putchar('-');
	printf("\n");

	/* Print the  total */
	printf("|%6c|", 0x20); 
	printf("%14c%6s%14c|", 0x20, "World", 0x20);
	printf("%13d|", cases_total);
	printf("%15d|", cases_today);
	printf("%14d|", deaths_total);
	printf("%16d|", deaths_today);
	printf("%17d|", recovered_total);
	printf("%14d|", active_total);
	printf("%16d|", critical_total);
	printf("\n");
	putchar(0x20);
	for(i = 0; i < 153; i++)
		putchar('-');
	printf("\n");


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

int get_data_per_country(char str[], int max)
{
	int i, c;

	i = 0;
	while(i < max && (c = getchar()) != '}' && c != EOF){
		if(c == ']') /* test for end of data indicator */
			return -1;
		str[i++] = c;
	}
	str[i++] = c;

	while(i < max && (c = getchar()) != '}' && c != EOF)
		str[i++] = c;
	str[i++] = c;
	str[i++] = '\0';

	return i;
}
int pretty_print(char *s[], int item)
{
	char *Id;

	Id = strtok(s[3], ":");
	Id = strtok(NULL, ":");
	/* Skips printing data for countries whose name has a character
	 * which cannot be represented by the char data type. Solution is
	 * part of todo. Uncomment the if statement to view all.*/
	
	if(!strcmp(Id, "638") || !strcmp(Id, "384") || !strcmp(Id, "531"))
		return 0;
	

	printf("|%1c%4d%1c|", 0x20, item, 0x20); 
	printf("%2c%-32s|", 0x20, strtok(s[1], "\""));
	printf("%13d|", atoi(s[17]));
	printf("%15d|", atoi(s[19]));
	printf("%14d|", atoi(s[21]));
	printf("%16d|", atoi(s[23]));
	printf("%17d|", atoi(s[25]));
	printf("%14d|", atoi(s[27]));
	printf("%16d|", atoi(s[29]));

	return 1;
}

void print_details(char *s[])
{
	char *t;

	printf("Country: %s\n", strtok(s[1], "\""));
	printf("Country Info:\n"); 
	t = strtok(s[41], "\"");
	printf("\tCountry code: %s\n", (strcmp(t, "}") ? t : "null"));
	strtok(s[3], ":");
	printf("\tId: %s\n", strtok(NULL, ":"));
	printf("\tiso2: %s\n", strtok(s[5], "\""));
	printf("\tiso3: %s\n", strtok(s[7], "\""));
	printf("\tlatitude: %d degrees\n", atoi(s[9]));
	printf("\tlongitude: %d degrees\n", atoi(s[11]));
	printf("Total Cases: %d\n", atoi(s[17]));
	printf("Today's cases: %d\n", atoi(s[19]));
	printf("Total Deaths: %d\n", atoi(s[21]));
	printf("Today's deaths: %d\n", atoi(s[23]));
	printf("Total recovered: %d\n", atoi(s[25]));
	printf("Total confirmed: %d\n", atoi(s[39]));
	printf("Total tests: %d\n", atoi(s[35]));
	printf("Total active: %d\n", atoi(s[27]));
	printf("Total critical: %d\n", atoi(s[29]));
}

