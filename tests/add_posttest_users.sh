#!/bin/csh -f
#count - pocet pridavanych uzivatelov
@ count=10
@ i=1
while ($i <= $count )
  echo "Pridavam uzivatela posttest$i"
  useradd -G users -m posttest$i
  #na pripadne mazanie vytvorenych uzivatelov + mazenie ich home-ov
  #userdel -f -r posttest$i
  @ i++
end
