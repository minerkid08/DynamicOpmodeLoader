for file in lua/*; do
  if [ -f "$file" ]; then
    adb push $file "/sdcard/$file"
  fi
done
