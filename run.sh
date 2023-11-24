#!/bin/bash
make
if [[ $? -ne 0 ]]; then
    echo "Compilation failed"
    exit 1
fi
if [[ ! -f "students.csv" ]]; then
    echo "students.csv not found"
    exit 1
fi
if [[ ! -f "template.txt" ]]; then
    echo "template.txt not found"
    exit 1
fi
bin/templr template.txt students.csv
