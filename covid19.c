#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int get_data();
void print_header();
void print_country(int);
void print_total(int);
void print_update_time(char *s);
void print_list(int);
void print_usage(char *);

struct Country{
		char name[50];
		struct {
			char id[5];
			char iso2[10];
			char iso3[10];
			double latitude;
			double longitude;
		}info;
		char updated[20];
		int cases;
		int today_cases;
		int deaths;
		int today_deaths;
		int recovered;
		int active;
		int critical;
		int cases_per_one_million;
		int deaths_per_one_million;
		int tests;
		int tests_per_one_million;
		char continent[20];
} country[250];

main(int argc, char *argv[])
{
	char c; 
	int w_opt = 0, c_opt = 0, l_opt = 0, h_opt = 0, k_opt;
	char *c_code, *prog_name = argv[0];
	int total_country;
	int i, found, id;

	/* start collecting the data for each country */
	total_country = get_data();	
	if(total_country < 0)
		return -1;
	
	while(--argc > 0 && (*++argv)[0] == '-'){
		c = *++argv[0];
		switch(c){
			case 'w':
				w_opt = 1;
				break;
			case 'l':
				l_opt = 1;
				break;
			case 'c':
				c_opt = 1;
				break;
			case 'h':
				h_opt = 1;
				break;
			case 'k':
				k_opt = 1;
				break;
			default:
				printf("%s: invalid option -- '-%c'\n", prog_name, c);
				printf("Try %s -h for help\n", prog_name);
				return -1;
		}
	}

	if(argc == 0){
		if(w_opt){ /* print world */
			print_header();
			for(i = 0; i < total_country; i++){
				/* Skips printing data for countries whose name has a character
	 			* which cannot be represented by the char data type. Solution is
	 			* part of todo. Comment the if statement to view all.*/
				if((id = atoi(country[i].info.id)) == 638 || id == 384 || id == 531)
					continue;
				print_country(atoi(country[i].info.id));
			}
			for(i = 0; i < 128; i++)
				putchar('=');
			printf("\n");
			print_total(total_country);
			print_update_time(country[0].updated);
		}else if(l_opt)
			print_list(total_country);
		else if(h_opt)
			print_usage(prog_name);
		else if(c_opt || k_opt){
			printf("%s: too few parameters for '-%c' option\n", prog_name, c);
			printf("Try %s -h for help\n", prog_name);
			return -1;
		}else
			print_usage(prog_name);
	}else{
		if(c_opt){
			c_code = *argv;
			found = 0;
			for(i = 0; !found && i < total_country; i++){
				if(strcmp(c_code, country[i].name) == 0 || strcmp(c_code,
							country[i].info.id) == 0 || strcmp(c_code,
								country[i].info.iso2) == 0)
					found = 1;
			}
			if(!found){
				printf("%s: country not found\n", prog_name);
				return -1;
			}else{
				print_header();
				print_country(atoi(country[i-1].info.id));
				putchar(0x20);
				for(i = 0; i < 128; i++)
					putchar('=');
				printf("\n");
				print_total(total_country);
				print_update_time(country[0].updated);
			}
		}else if(k_opt){
			found = 0;
			for(i = 0; !found && i < total_country; i++){
				if(strcmp(*argv, country[i].continent) == 0)
					found = 1;
			}
			if(!found){
				printf("%s: continent not found\n", prog_name);
				printf("Try %s -h for help\n", prog_name);
				return -1;
			}else{
				print_header();
				for(i = 0; i < total_country; i++)
					if(strcmp(*argv, country[i].continent) == 0)
						print_country(atoi(country[i].info.id));

				putchar(0x20);
				for(i = 0; i < 128; i++)
					putchar('=');
				printf("\n");
				print_total(total_country);
				print_update_time(country[0].updated);
			}
		}else{
			printf("%s: too many parameters\n", prog_name);
			printf("Try %s -h for help\n", prog_name);
			return -1;
		}
		
	}

	return 0;
}

void print_usage(char *s)
{
	printf("Usage: %s [OPTIONS]\n", s);
	printf("Display the table of covid-19 cases of a country or of the world\n");	
	printf("\nOptions:\n");
	printf("%3s, %-12s Print the table for a specific country\n", "-c",
			"COUNTRY");
	printf("%17c COUNTRY can be the whole name or country code e.g.\n", 0x20);
	printf("%17c Philippines, PH, ..etc. ", 0x20);
	printf("Use -l to print country codes.\n", 0x20);
	printf("%3s  %12c Print the table for the world\n", "-w", 0x20);
	printf("%3s, %-12s Print the table for a continent. ", "-k", "CONTINENT");
	printf("Use -l to see list of continents.\n");
	printf("%3s  %12c Print a list of country\n", "-l", 0x20);
	printf("%3s  %12c Prints this usage information.\n", "-h", 0x20);

	printf("\n");
			
}

/* get_data: collect the data from the data.json file. */
#define MAXLEN 1000
#define DATA_END 0
#define ARR_FULL -1
int get_raw(char [], int);
void get_tokens(char [], char *[]);

FILE *fp;

int get_data()
{
	char rawdata[MAXLEN], *tokens[50];
	int ret, i;
	char temp[10];
	char *tok;

	fp = fopen("data.json", "r");

	if(!fp){
		printf("The file \"data.json\" is not found. Download first.\n");
		return -1;
	}
	/* start capturing only after the data keyword. Data are enclosed within the
	 * [ ] */
	while(fgetc(fp) != '[')
		;

	i = 0;
	while((ret = get_raw(rawdata, MAXLEN)) > 0){
		get_tokens(rawdata, tokens);

		/* Save the data for each country */
		strcpy(country[i].name, strtok(tokens[3], "\""));
		strcpy(country[i].info.id, strtok(tokens[6], "\""));
		strcpy(country[i].info.iso2, strtok(tokens[8], "\""));
		strcpy(country[i].info.iso3, strtok(tokens[10], "\""));
		country[i].info.latitude = atof(tokens[12]);
		country[i].info.longitude = atof(tokens[14]);
		strcpy(country[i].updated, strtok(tokens[1], "\""));
		country[i].cases = atoi(tokens[19]);
		country[i].today_cases = atoi(tokens[21]);
		country[i].deaths = atoi(tokens[23]);
		country[i].today_deaths = atoi(tokens[25]);
		country[i].recovered = atoi(tokens[27]);
		country[i].active = atoi(tokens[29]);
		country[i].critical = atoi(tokens[31]);
		country[i].cases_per_one_million = atoi(tokens[33]);
		country[i].deaths_per_one_million = atoi(tokens[35]);
		country[i].tests = atoi(tokens[37]);
		country[i].tests_per_one_million = atoi(tokens[39]);
		/* Do not write to continent member for "Diamond Princess" and "MS
		 * Zaandam" */
		if(strcmp(country[i].name, "Diamond Princess") == 0 ||
				strcmp(country[i].name, "MS Zaandam") == 0)
			;
		else
			strcpy(country[i].continent, strtok(tokens[41],"\""));

		i++;
	}
	fclose(fp);
	return i;
}

int get_raw(char s[], int lim)
{
	int i, c;

	i = 0;
	while(i < lim && (c = fgetc(fp)) != '}' &&  c != EOF){
		/* check for end of data indicator */
		if(c == ']')
			return DATA_END;
		s[i++] = c;	
	}
	if(i == lim)
		return ARR_FULL;
	if(c == EOF)
		return c;
	s[i++] = c;
	while(i < lim && (c = fgetc(fp)) != '}' && c != EOF)
		s[i++] = c;

	if(i == lim)
		return ARR_FULL;
	if(c == EOF)
		return c;
	s[i++] = c;
	s[i] = '\0';

	return i;
}

void get_tokens(char s[], char *t[])
{
	char *tok, *delim = ":,";
	int i;

	for(i = 0, tok = strtok(s, delim); tok != NULL; tok = strtok(NULL, delim))
		t[i++] = tok;
	
	t[i] = tok;
}

void print_header()
{
	int i;
	printf("\n");
	putchar(0x20);
	for(i = 0; i < 128; i++)
		putchar('-');
	printf("\n");
	printf("|%1c%4s%1c|", 0x20, "Rank", 0x20);
	printf("%13c%7s%14c|", 0x20, "Country", 0x20);
	printf(" %-11s |", "Total Cases");
	printf(" %-9s |", "New cases");
	printf(" %-12s |", "Total Deaths");
	printf(" %-10s |", "New Deaths");
	printf(" %-9s |", "Recovered");
	printf(" %-6s |", "Active");
	printf(" %-8s |", "Critical");
	printf("\n");
	putchar(0x20);
	for(i = 0; i < 128; i++)
		putchar('-');
	printf("\n");
}

void print_country(int id)
{
	int i;

	for(i = 0; atoi(country[i].info.id) != id; i++) 
		;

	printf("|%1c%4d%1c|", 0x20, i+1, 0x20); 
	printf("%2c%-32s|", 0x20, country[i].name);
	printf("%12d%1c|", country[i].cases, 0x20);
	printf("%10d%1c|", country[i].today_cases, 0x20);
	printf("%13d%1c|", country[i].deaths, 0x20);
	printf("%11d%1c|", country[i].today_deaths, 0x20);
	printf("%10d%1c|", country[i].recovered, 0x20);
	printf("%7d%1c|", country[i].active,0x20);
	printf("%9d%1c|", country[i].critical, 0x20);
	printf("\n");
	
	
}

void print_total(int n)
{
	int i;
	int cases_total = 0, cases_today = 0, deaths_total = 0;
	int deaths_today = 0, recovered_total = 0, active_total = 0;
	int critical_total = 0;

	for(i = 0; i < n; i++){
		cases_total += country[i].cases;
		cases_today += country[i].today_cases;
		deaths_total += country[i].deaths;
		deaths_today += country[i].today_deaths;
		recovered_total += country[i].recovered;
		active_total += country[i].active;
		critical_total += country[i].critical;
	}

	printf("|%6c|", 0x20); 
	printf("%14c%6s%14c|", 0x20, "World", 0x20);
	printf("%12d%1c|", cases_total, 0x20);
	printf("%10d%1c|", cases_today, 0x20);
	printf("%13d%1c|", deaths_total, 0x20);
	printf("%11d%1c|", deaths_today, 0x20);
	printf("%10d%1c|", recovered_total, 0x20);
	printf("%7d%1c|", active_total, 0x20);
	printf("%9d%1c|", critical_total, 0x20);
	printf("\n");
	putchar(0x20);
	for(i = 0; i < 128; i++)
		putchar('-');
	printf("\n");
	
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

/* print_list: print country list */
void print_list(int n)
{
	int i;

	printf("\nList of country. \nListed in the following format: ");
	printf("[id] \"country code\" \"continent\" \"name\"\n\n");
	
	for(i = 0; i < n; i++){
		printf("[%3d]  %-4s ", atoi(country[i].info.id), country[i].info.iso2);
		printf("%-19s ", country[i].continent);
		printf("%-33s ", country[i].name, 0x20);
		if((i+1)%2 == 0)
			printf("\n");
	}
	printf("\n");
}
