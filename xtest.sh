#!/usr/bin/env bash
#Pouziti:
#vytvor testovaci soubor a vloz ho do ./testy/{JMENOPROGRAMU}
#{JMENOPROGRAMU} by melo popsat co program testuje
#vytvor ocekavany vystup tohoto programu a vloz ho do ./testy/out/{JMENOPROGRAMU}Out
#vytvor ocekavany navratovy kod k tomuto programu a vloz ./testy/out/{JMENOPROGRAMU}Ecode
#opakuj pro libovolny pocet programu IFJ13


res=0;


#barvy
green='\e[0;32m'
red='\e[0;31m'
lred='\e[1;31m'
yellow='\e[1;33m'
NC='\e[0m' # no color


echo -e "TEST 0  ECODE a OUT .: ${red} FAIL  ${NC} --- ${yellow}p2${NC}"
echo -e "TEST 0  ECODE a OUT .: ${red} FAIL  ${NC} --- ${yellow}p3${NC}"
echo -e "TEST 0  ECODE a OUT .: ${red} FAIL  ${NC} --- ${yellow}p8${NC} je to deadlock ale mel by se chovat jinak"
echo -e "TEST 0  ECODE a OUT .: ${red} FAIL  ${NC} --- ${yellow}p9${NC} je taky nekonecny a spadne"
#ziskani testovacich souboru
location="./testy"
foundExp=`find ./ -maxdepth 1 -type f ! -name '.*'` 
foundExp=$(echo "$foundExp" | sed "s|./||g")

foundProj=`find $location -maxdepth 1 -type f ! -name '.*'`
foundProj=`echo "$foundProj" | rev | cut -d"/" -f1-1 | rev`
	
n1=$'\n'
found="${foundExp}${n1}${foundProj}"
found=$(echo  "$found" | sort -u)
echo "$found" > ./testy/.files


#testovani pokud jsou nalezeny testy a vysledky k nim
CT=1; #cislo testu
cat ./testy/.files | while read line; do
	if [ -f "./testy/${line}" ] && [ -f "./testy/out/${line}Out" ]
	then
		./interpret testy/${line} > ./testy/.out
		echo $? > ./testy/.ecode 
		diff -u "./testy/.out" "./testy/out/${line}Out" >/dev/null 2> /dev/null
		if [ $? -eq 0 ]
		then 	echo -e "TEST ${CT}  OUTPUT.: ${green}SUCCESS${NC} --- ${yellow}${line}${NC}"

		else
			echo -e "TEST ${CT}  OUTPUT.: ${red} FAIL  ${NC} --- ${yellow}${line}${NC}"
		fi	

		diff -u "./testy/.ecode" "./testy/out/${line}Ecode" > /dev/null 2> /dev/null
		if [ $? -eq 0 ]
		then 	echo -e "TEST ${CT}  ECODE .: ${green}SUCCESS${NC} --- ${yellow}${line}${NC}"
		else
			echo -e "TEST ${CT}  ECODE .: ${red} FAIL  ${NC} --- ${yellow}${line}${NC}"
		fi	

		CT=$((CT+1));  # inkrementace
	fi	
	
done


rm ./testy/.out 2> /dev/null > /dev/null
rm ./testy/.files 2> /dev/null > /dev/null
rm ./testy/.ecode 2> /dev/null > /dev/null
exit 0
