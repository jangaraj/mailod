#!/bin/csh -f
#count - pocet uzivatelov
@ count=10
@ i=1
#chmod ma moc prav - obmedzit
chmod -R 770 /home/posttest*/.maildir
while ($i <= $count )
	echo "Menim prava pre posttest$i"
	chown -R postfix:users /home/posttest$i/.maildir
	setfacl -b /home/posttest$i/.maildir
	setfacl -k /home/posttest$i/.maildir
	setfacl -m g:users:rwx /home/posttest$i/.maildir
	setfacl -b /home/posttest$i/.maildir/new
	setfacl -k /home/posttest$i/.maildir/new
	setfacl -m g:users:rwx /home/posttest$i/.maildir/new
	setfacl -d -m u::---,u:postfix:rw-,u:posttest$i\:rw-,g::---,o::--- /home/posttest$i/.maildir/new
	setfacl -b /home/posttest$i/.maildir/cur
	setfacl -k /home/posttest$i/.maildir/cur
	setfacl -m g:users:rwx /home/posttest$i/.maildir/cur
	setfacl -d -m u::---,g::---,o::--- /home/posttest$i/.maildir/cur
	setfacl -m g:users:rwx /home/posttest$i/.maildir/tmp
	chown posttest$i\:users /home/posttest$i/.maildir/courierimapuiddb
	chmod 600 /home/posttest$i/.maildir/courierimapuiddb
	chown -R posttest$i\:posttest$i /home/posttest$i/.maildir/courierimapkeywords
	chmod -R 700 /home/posttest$i/.maildir/courierimapkeywords
	chown posttest$i\:posttest$i /home/posttest$i/.maildir/courierimapsubscribed
	chmod 600 /home/posttest$i/.maildir/courierimapsubscribed
	chown -R posttest$i\:users /home/posttest$i/.maildir/.Trash
	chmod -R 700 /home/posttest$i/.maildir/.Trash
	chmod 600 /home/posttest$i/.maildir/.Trash/courierimapacl
	chmod 600 /home/posttest$i/.maildir/.Trash/courierimapuiddb
	chmod 600 /home/posttest$i/.maildir/.Trash/maildirfolder
	@ i++
end
