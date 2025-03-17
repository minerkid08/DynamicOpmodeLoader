for file in lua/*; do
  if [ -f "$file" ]; then
    curl -T "$file" -u user:user ftp://127.0.0.1:2121/
  fi
done
