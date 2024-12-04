#!/bin/bash

mkdir /etc/.EyeLight
touch /etc/.EyeLight/day.txt /etc/.EyeLight/night.txt /etc/.EyeLight/transition.txt
echo "5000" | cat > /etc/.EyeLight/day.txt
echo "3500" | cat > /etc/.EyeLight/transition.txt
echo "2000" | cat > /etc/.EyeLight/night.txt


