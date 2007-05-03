#!/bin/csh -f
#count - pocet uzivatelov
@ count=10
@ i=1
while ($i <= $count )
	echo "Posielam test_email pouzivatelovi posttest$i"
	cat ../src/test_mail  | mail posttest$i
	@ i++
end
