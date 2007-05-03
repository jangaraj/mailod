#!/bin/csh -f
#count - pocet uzivatelov
@ count=10
@ i=1
while ($i <= $count )
	echo "Cistim maildir pouzivatelovi posttest$i"
	rm /home/posttest$i/.maildir/new/*
	rm /home/posttest$i/.maildir/cur/*
	@ i++
end
