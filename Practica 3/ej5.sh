#!/bin/bash
for i in `seq 1 5`; do
    java AskRemote localhost &
    echo 'client created!'
done

