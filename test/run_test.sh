
CALLDIR=`pwd`

cd $1

gpp -std=c++2a -O2 source.cpp

for file in `\find ./in -maxdepth 1 -type f | sed -E 's!^.*/(.+)\.in!\1!'`; do
	
	start_time=$(date +"%s%3N")

	./a.out < "./in/$file.in" > tmp_output
	
	end_time=$(date +"%s%3N")

	if [ "" = "`diff -q tmp_output out/$file.out`" ]
	then
		echo "$file : " `expr $end_time - $start_time`"ms"
	else
		echo "WA on $file"
		break
	fi

	rm tmp_output
done

echo ""
echo "Passed all testcases!"

cd $CALLDIR
