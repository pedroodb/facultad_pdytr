#!/bin/bash

rm -r ./output/*

java -cp "../jade/lib/jade.jar:../classes" jade.Boot -gui &
sleep 1
MAIN_CONTAINER_ID=$!
java -cp "../jade/lib/jade.jar:../classes" jade.Boot -container -container-name c1 >> ./output/c1.output &
sleep .5
java -cp "../jade/lib/jade.jar:../classes" jade.Boot -container -container-name c2 >> ./output/c2.output &
sleep .5
java -cp "../jade/lib/jade.jar:../classes" jade.Boot -container -container-name c3 >> ./output/c3.output &
sleep .5
java -cp "../jade/lib/jade.jar:../classes" jade.Boot -container -container-name c4 -agents "visitador:examples.Ej1.Ej1(c1, c2, c3)" >> ./output/c4.output &

sleep 2
kill $MAIN_CONTAINER_ID
echo ".......Finalizó el script. Los archivos se encuentran en ./output."