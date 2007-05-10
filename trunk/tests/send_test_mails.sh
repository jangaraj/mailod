#!/bin/csh -f
#count - pocet uzivatelov
@ count=10
@ i=1
while ($i <= $count )
	echo "Posielam test_text pouzivatelovi posttest$i"
	cat test_text | mail -s testing_mailod posttest$i
	@ i++
end
