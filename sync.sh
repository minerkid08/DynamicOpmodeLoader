for file in TeamCode/src/main/java/org/firstinspires/ftc/teamcode/lua/*; do
  if [ -f "$file" ]; then
    adb push "$file /sdcard/$file"
  fi
done
