
gpp -std=c++2a -O2 ./$1/source.cpp

for file in `\find ./$1/in -maxdepth 1 -type f | sed -E 's!^.*/(.+)\.in!\1!' | sort -f`; do

	./a.out < "./$1/in/$file.in" > tmp_output

	ave_time=0
	for i in `seq 5`; do
		start_time=$(date +"%s%3N")

		./a.out < "./$1/in/$file.in" > tmp_output
	
		end_time=$(date +"%s%3N")

		ave_time=$((ave_time + end_time - start_time))
	done
	
	ave_time=$((ave_time/5))

	if [ "" = "`diff -q tmp_output ./$1/out/$file.out`" ]
	then
		echo "$file : "$ave_time"ms"
	else
		echo "WA on $file"
		break
	fi
done

rm tmp_output
rm a.out
