FILES=""
for file in *; do
  if [ -f "$file" ]; then
    NAME="${file%.*}"
    EXT="${file##*.}"
    if [ "$EXT" = "c" ]; then
      gcc -c "$file" -o "$NAME.o"
      FILES="$FILES$NAME.o "
    fi
  fi
done
ar rcs lua.a $FILES
