#!/bin/bash

gawk -f p3a1.awk unsortedNames.txt | sort |  gawk -f p3a1.awk > p3a.out
