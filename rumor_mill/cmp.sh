O2=$(./2.out < output.txt)
O3=$(./3.out < output.txt)

if [ "$O2" == "$O2" ]; then
    echo "SAME"
else
    echo "DIFFERENT"
fi

if [ "$O3" == "$O3" ]; then
    echo "SAME"
else
    echo "DIFFERENT"
fi

if [ "$O2" == "$O3" ]; then
    echo "SAME"
else
    echo "DIFFERENT"
fi
