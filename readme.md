This is a very simple covid19 tracker.
Displays a table of covid19 cases of a country or of the world.


To compile:

	% c89 covid19.c -o covid19

To view the table for the whole world:

	% ./covid19 -w

To view the table for a specific country:

	% ./covid19 -c PH or
	% ./covid19 -c Philippines or
	% ./covid19 -c 608

To view a list of country with country codes:

	% ./covid19 -l

To view the options:

	% ./covid19 -h


The data.json file can be downloaded in json format via:

	% curl https://corona-stats.online?format=json > data.json

	or

	% curl https://corona.lmao.ninja/countries?sort=cases > data.json


It is best to download first the data.json file before running the
program to ensure that you have the latest information.

References:

https://github.com/sagarkarira/coronavirus-tracker-cli  
https://github.com/novelcovid/api

