for f in ./alltests/simplefiles/*; do
    ./ourParser $f
    read -p "Press a ENTER to continue..."
done
