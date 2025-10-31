for dir in $(find example -mindepth 1 -maxdepth 1 -type d); do
	echo "Running $dir"
	echo "$(cd $dir && ../../packers)"
done
