#!/bin/bash

if [ "$1" = "-h" ] || [ "$1" = "" ]; then
	echo "$0 ------------------------------
	
	Create a save of your bot's personnality
	which includes it's configuration files, it's
	data files and it's users files. All that is stored in
	a targzball named <botnick>_backup.tgz where botnick
	is by default 'Yarilo'.
	
	Syntax :
	
	$0 -h : Displays this help
	
	$0 --backup [botnick] : Create the targzball. By default
	                        botnick is Yarilo. The fille will
	                        be <botnick>_backup.tgz.
	                        
	$0 --restore [file]  : Restores from file of from the
	                        default Yarilo_backup.tgz file.
	                        All the changes made after the 
	                        save will be lost (all the
	                        directories will be removed
	                        before restoring)
"
elif [ "$1" = "--backup" ]; then
	shift
	botnick="Yarilo"
	if ! [ "$1" = "" ]; then
		botnick=$1
	fi;
	arname="$botnick"_save.tgz
	echo "Saving $arname..."
	tar -zcf $arname data conf
	echo "Bot saved to $arname"
elif [ "$1" = "--restore" ]; then
	shift
	botnick="Yarilo"
	if ! [ "$1" = "" ]; then
		botnick=$1
	fi;
	arname="$botnick"_save.tgz
	if [ -f $arname ]; then
		echo "Restoring $arname..."
		rm -rf data conf
		tar -zxf $arname
	else
		echo "Error, the file $arname does not exist !"
	fi
fi;
		
