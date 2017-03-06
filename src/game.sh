#!/bin/bash
n=10
k=100
s=1
p1="NO INPUT"
p2="NO INPUT"
ai1="NO INPUT"
ai2="NO INPUT"
guiName=./sredniowiecze_gui_with_libs.sh


function isNumber {
	re='^[0-9]+$'
	if ! [[ $1 =~ $re ]] ; then
	   exit 1
	fi
	if ! (( $1 < 2147483648 ))
	then
		exit 1
	fi
}

function isProperFile {
	FILE=$1

if [ ! -f "$FILE" ]
then
    echo "File $FILE does not exists"
fi
}

function checkIfKilledAIWithWrongCode {
	if (! kill -0 $1 &>/dev/null)
		then
		wait $1
		exitCode=$?

		if  ! [ "$exitCode" -eq "0" ] && ! [ "$exitCode" -eq "1" ] && ! [ "$exitCode" -eq "2" ]
		then
			exit 1
		fi
	fi
}

function checkIfKilledGUIWithWrongCode {
	if ! kill -0 $1 &>/dev/null
		then
		wait $1
		exitCode=$?
		kill $1 &>/dev/null
		kill $2 &>/dev/null
		kill $3 &>/dev/null

		if ! [ "$exitCode" -eq "0" ]
		then
			exit 1
		fi
	fi
}

function checkIfKilledAI3 {

	if (! kill -0 $1 &>/dev/null)
		then
		wait $1
		exitCode=$?

		if [ "$exitCode" -eq "0" ] || [ "$exitCode" -eq "1" ] || [ "$exitCode" -eq "2" ]
		then
			checkIfKilledAIWithWrongCode $2
			checkIfKilledGUIWithWrongCode $3
			kill $1 &>/dev/null
			kill $2 &>/dev/null
			kill $3 &>/dev/null

			exit 0
		else
			kill $1 &>/dev/null
			kill $2 &>/dev/null
			kill $3 &>/dev/null
			exit 1
		fi
	fi
}

function checkIfKilledAI2 {

	if (! kill -0 $1 &>/dev/null)
		then
		wait $1
		exitCode=$?

		if [ "$exitCode" -eq "0" ] || [ "$exitCode" -eq "1" ] || [ "$exitCode" -eq "2" ]
		then
			checkIfKilledGUIWithWrongCode $2
			kill $1 &>/dev/null
			kill $2 &>/dev/null
			kill $3 &>/dev/null
			exit 0
		else
			kill $1 &>/dev/null
			kill $2 &>/dev/null
			kill $3 &>/dev/null
			exit 1
		fi
	fi
}

function checkIfKilledGUI3 {
	if ! kill -0 $1 &>/dev/null
		then
		wait $1
		exitCode=$?

		if [ "$exitCode" -eq "0" ]
		then
			checkIfKilledAIWithWrongCode $2
			checkIfKilledAIWithWrongCode $3
			kill $1 &>/dev/null
			kill $2 &>/dev/null
			kill $3 &>/dev/null
			exit 0
		else
			kill $1 &>/dev/null
			kill $2 &>/dev/null
			kill $3 &>/dev/null
			exit 1
		fi
	fi
}

function checkIfKilledGUI2 {
	if ! kill -0 $1 &>/dev/null
		then
		wait $1
		exitCode=$?


		if [ "$exitCode" -eq "0" ]
		then
			checkIfKilledAIWithWrongCode $2
			kill $1 &>/dev/null
			kill $2 &>/dev/null
			kill $3 &>/dev/null
			exit 0
		else
			kill $1 &>/dev/null
			kill $2 &>/dev/null
			kill $3 &>/dev/null
			exit 1
		fi
	fi
}

while (( $# > 0 ))
do
    if [ "$1" == "-n" ]
	then
		n=$2
		isNumber $n
	else if [ "$1" == "-k" ]
	then
		k=$2
		isNumber $k
	else if [ "$1" == "-s" ]
	then
		s=$2
		isNumber $s
	else if [ "$1" == "-p1" ]
	then
		p1=$2

	else if [ "$1" == "-p2" ]
	then
		p2=$2

	else if [ "$1" == "-ai1" ]
	 then
		 ai1=$2
		 isProperFile $ai1
	else if [ "$1" == "-ai2" ]
	then
		ai2=$2
		isProperFile $ai2
	else
		exit 1
	fi fi fi fi fi fi fi

    shift
	shift
done

if [ "$p1" != "NO INPUT" ]
then
	x1=${p1%,*}
	y1=${p1#*,}
	isNumber $x1
	isNumber $y1
fi

if [ "$p2" != "NO INPUT" ]
then
	x2=${p2%,*}
	y2=${p2#*,}
	isNumber $x2
	isNumber $y2
fi

if [ "$p1" == "NO INPUT" ] && [ "$p2" == "NO INPUT" ]
then
	x1=$(($RANDOM%(n-3)+1))
	y1=$(($RANDOM%(n-8)+1))
	x2=$(($RANDOM%(n-3)+1))
	y2=$(($RANDOM%(n-y1-7)+8+y1))
else if [ "$p1" == "NO INPUT" ]
then

	if [ $x2 -le 14 ] && [ $(($x2+14)) -gt $n ] && [ $y2 -le 8 ] && [ $(($y2+8)) -gt $n ]
	then
		exit 1
	fi

	x1=$(($RANDOM%(n-3)+1))
	y1=$(($RANDOM%(n)+1))
	while
	!( (( x1-x2 > 8 )) || (( x2-x1 > 8 )) || (( y1-y2 > 8 )) || (( y2-y1 > 8 ))  )
	do
		x1=$(($RANDOM%(n-3)+1))
		y1=$(($RANDOM%(n)+1))
	done

else if [ "$p2" == "NO INPUT" ]
then
	if [ $x1 -le 14 ] && [ $(($x1+14)) -gt $n ] && [ $y1 -le 8 ] && [ $(($y1 + 8)) -gt $n ]
	then
		exit 1
	fi

	x2=$(($RANDOM%(n-3)+1))
	y2=$(($RANDOM%(n)+1))
	while
	!( (( x1-x2 > 8 )) || (( x2-x1 > 8 )) || (( y1-y2 > 8 )) || (( y2-y1 > 8 ))  )

	do
		x2=$(($RANDOM%(n-3)+1))
		y2=$(($RANDOM%(n)+1))
	done
fi fi fi
sleep 1


PIPE=$(mktemp -u)
mkfifo $PIPE
exec 3<>$PIPE 13>$PIPE 23<$PIPE
rm $PIPE

PIPE=$(mktemp -u)
mkfifo $PIPE
exec 4<>$PIPE 14>$PIPE 24<$PIPE
rm $PIPE

PIPE=$(mktemp -u)
mkfifo $PIPE
exec 5<>$PIPE 15>$PIPE 25<$PIPE
rm $PIPE

PIPE=$(mktemp -u)
mkfifo $PIPE
exec 6<>$PIPE 16>$PIPE 26<$PIPE
rm $PIPE

PIPE=$(mktemp -u)
mkfifo $PIPE
exec 7<>$PIPE 17>$PIPE 27<$PIPE
rm $PIPE

#AI vs AI
if [ "$ai1" != "NO INPUT" ] && [ "$ai2" != "NO INPUT" ]
then

	$ai1 <&3 >&4 &
	pid1=$!
	exec 4>&-
	exec 14>&-

	$ai2 <&5 >&6 &
	pid2=$!
	exec 6>&-
	exec 16>&-

	$guiName <&7 >/dev/null &
	pid3=$!

	echo INIT $n $k 1 $x1 $y1 $x2 $y2 >&13
	echo INIT $n $k 2 $x1 $y1 $x2 $y2 >&15
	echo INIT $n $k 1 $x1 $y1 $x2 $y2 >&17
	echo INIT $n $k 2 $x1 $y1 $x2 $y2 >&17

	while :
	do
		while
			read a <&24
			if ! [ -z "$a" ]
			then
				echo $a >&7
			else
				checkIfKilledAI3 $pid1 $pid2 $pid3
			fi
			echo $a >&15
			sleep $s
			checkIfKilledGUI3 $pid3 $pid1 $pid2
			[ "$a" != "END_TURN" ]
		do :
		done

		while
			read a <&26
			if ! [ -z "$a" ]
			then
				echo $a >&7
			else
				checkIfKilledAI3 $pid2 $pid1 $pid3
			fi
			echo $a >&13
			sleep $s
			checkIfKilledGUI3 $pid3 $pid1 $pid2
			[ "$a" != "END_TURN" ]
		do :
		done

	done

#AI vs human
else if [ "$ai1" != "NO INPUT" ]
then
	$ai1 <&3 >&4 &
	pid1=$!
	exec 4>&-
	exec 14>&-

	$guiName -human2 <&5 >&6 &
	pid2=$!
	exec 6>&-
	exec 16>&-

	echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&3
	echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&5
	echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&5

	while :
	do


		while
				read a <&24
				if ! [ -z "$a" ]
				then
					echo $a >&15
				fi
				#sleep $s
				checkIfKilledAI2 $pid1 $pid2

			[ "$a" != "END_TURN" ]
		do :
		done



		while
				read -t 0.4 a <&26
				if ! [ -z "$a" ]
				then
				echo $a >&13
				fi
				checkIfKilledGUI2 $pid2 $pid1

			[ "$a" != "END_TURN" ]
		do :
		done
	done

else if [ "$ai2" != "NO INPUT" ]
then
	$ai2 <&3 >&4 &
	pid1=$!
	exec 4>&-
	exec 14>&-

	$guiName -human1 <&5 >&6 &
	pid2=$!
	exec 6>&-
	exec 16>&-

	echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&3
	echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&5
	echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&5

	while :
	do

		while
				read -t 0.4 a <&26
				if ! [ -z "$a" ]
				then
					echo $a >&13
				fi

				checkIfKilledGUI2 $pid2 $pid1

			[ "$a" != "END_TURN" ]
		do :
		done

		while
				read a <&24
				if ! [ -z "$a" ]
				then
					echo $a >&15
				fi
				#sleep $s
				checkIfKilledAI2 $pid1 $pid2


			[ "$a" != "END_TURN" ]
		do :
		done




	done
#Human vs human
else
	echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&3
	echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&3

	$guiName -human1 -human2 <&3 >/dev/null &
	pid=$!

	wait -n $pid
	exitCode=$?

	if [ "exitCode"=="0" ]
	then
		exit 0
	else
		exit 1

	fi

fi fi fi
