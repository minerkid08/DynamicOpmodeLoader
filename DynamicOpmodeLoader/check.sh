for file in "lua"/*; do
  if [[ -f "$file" ]]; then # Check if it's a regular file (not a directory)
    diff "$file" "upload/$file" "-s"
  else
for file2 in "$file"/*; do
  if [[ -f "$file2" ]]; then # Check if it's a regular file (not a directory)
    diff "$file2" "upload/$file2" "-s"
  fi
done
  fi
done
