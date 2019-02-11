#!/bin/bash
read -p 'start id: ' startId
read -p 'end id: ' endId
if [ ! -d $startId'-'$endId ]; then
    mkdir $startId'-'$endId
fi
cd $startId'-'$endId
cp ../check.sh .
printf '# POJ2019 '$startId'-'$endId'\n' > "README.md"
for i in $(seq -w $startId $endId); do
    printf '\n- [ ] '$i'\n\n||\n|:--|\n||\n' >> "README.md"
done
cd ..