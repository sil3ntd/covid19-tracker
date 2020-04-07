This is a very simple covid19 tracker. 

The data is taken from https://github.com/sagarkarira/coronavirus-tracker-cli.


To compile:

	% c89 covid19.c -o covid19

To run:

	% ./covid19 < data.json

The data.json file can be downloaded in json format via:

	% curl https://corona-stats.online?format=json > data.json


It is best to download first the data.json file before running the
program to ensure that you have the latest information.
