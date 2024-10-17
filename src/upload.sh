if ! command -v picotool &> /dev/null
then
    echo "picotool could not be found"
    exit
fi

# Check if there is a .uf2 file in the build directory
if ! ls build/*.uf2 1> /dev/null 2>&1; then
    echo "No .uf2 file found in the build directory"
    exit
fi

uf2_file=$(ls build/*.uf2)
picotool load $uf2_file