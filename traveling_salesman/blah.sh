while true; do
    python create_test.py 5;
    A=$(cat output.ans);
    B=$(./a.out < output.in);
    if [ "$A" != "$B" ]; then
        echo "Failed";
        break
    fi
done
