#!/bin/sh

sh_dir=`\dirname $0`

run_times=$2
if [ "" = "$run_times" ]
then
	run_times=5
fi

~/usr/bin/g++ -Wall -Wextra -std=c++2a -O2 -I $sh_dir/../include $sh_dir/$1/source.cpp

for file in `\find $sh_dir/$1/in -maxdepth 1 -type f | sed -E 's!^.*/(.+)\.in!\1!' | sort -f`; do

	./a.out < "$sh_dir/$1/in/$file.in" > tmp_output

	ave_time=0
	for i in `seq $run_times`; do
		start_time=$(date +"%s%3N")

		./a.out < "$sh_dir/$1/in/$file.in" > tmp_output
	
		end_time=$(date +"%s%3N")

		ave_time=$((ave_time + end_time - start_time))
	done
	
	ave_time=$((ave_time/$run_times))

	if [ "" = "`diff -q tmp_output $sh_dir/$1/out/$file.out`" ]
	then
		echo "$file : "$ave_time"ms"
	else
		echo "WA on $file"
		break
	fi
done

rm tmp_output
rm a.out
