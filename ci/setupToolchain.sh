#!/bin/env bash

set -e

aptInstall() {
	sudo apt-get update
	sudo apt-get install "$@" --no-install-recommends -y
}

installGCC() {
	sudo add-apt-repository "deb https://ppa.launchpadcontent.net/ubuntu-toolchain-r/ppa/ubuntu/ plucky main"
	aptInstall g++-"$1" binutils
}

installLLVM() {
	wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key | sudo tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc
	sudo add-apt-repository "deb http://apt.llvm.org/noble/ llvm-toolchain-noble-$1 main"
	aptInstall clang-"$1"
}

intallSoftwareProperties() {
	aptInstall software-properties-common
}

main() {
	# gcc-15 or llvm-21
	local compiler
	local version
	IFS='-' read -r compiler version <<<"$1"
	case "$compiler" in
	"g++" | "gcc")
		installGCC "$version"
		;;
	"llvm" | "clang" | "clang++")
		installLLVM "$version"
		;;
	*)
		echo "Unknown compiler: $compiler"
		exit 1
		;;
	esac
}

main "$@"
