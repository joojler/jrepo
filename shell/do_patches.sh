

for file in $1
do
	patch -p1 <$file
done
