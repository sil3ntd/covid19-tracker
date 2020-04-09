This is a very simple covid19 tracker. 

References from which data is taken are from:

https://github.com/sagarkarira/coronavirus-tracker-cli  
https://github.com/novelcovid/api


To compile:

	% c89 covid19.c -o covid19

To run:

	% ./covid19 < data.json


The data.json file can be downloaded in json format via:

	% curl https://corona-stats.online?format=json > data.json

	or

	% curl https://corona.lmao.ninja/countries?sort=cases > data.json


It is best to download first the data.json file before running the
program to ensure that you have the latest information.
