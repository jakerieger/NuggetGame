if [ -d ../Distributables ]; then
  rm -r ../Distributables
fi

mkdir ../Distributables

mkdir ../Distributables/Windows

mkdir ../Distributables/Windows/Debug
mkdir ../Distributables/Windows/Debug/lib
mkdir ../Distributables/Windows/Debug/include

mkdir ../Distributables/Windows/Release
mkdir ../Distributables/Windows/Release/lib
mkdir ../Distributables/Windows/Release/include

cp ../build/Debug/Core.lib ../Distributables/Windows/Debug/lib/Core.lib

# ==================================================================== #
#                         COPY HEADER FILES                            #
# ==================================================================== #
root_dir="../Core"
headers_exts=(".h" ".inl")

for ext in "${headers_exts[@]}"; do
  inc_dirs=("$root_dir"/*"$ext" "$root_dir"/**/*"$ext")
  for file in "${inc_dirs[@]}"; do
    if [ -f "$file" ]; then
      relative_path="${file#$root_dir/}"
      include_path="../Distributables/Windows/Debug/include"
      dest_path="$include_path/${relative_path}"
      mkdir -p "$(dirname "$dest_path")"
      cp "$file" "$dest_path"
      echo "Copied $file => $dest_path"
    fi
  done
done