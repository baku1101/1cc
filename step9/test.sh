#!/bin/sh
# templated by http://qiita.com/blackenedgold/items/c9e60e089974392878c8
usage() {
	cat <<HELP
NAME:
   $0 -- {one sentence description}

SYNOPSIS:
  $0 [-h|--help]
  $0 [--verbose]

DESCRIPTION:
   {description here}

  -h  --help      Print this help.
	  --verbose   Enables verbose mode.

EXAMPLE:
  {examples if any}

HELP
}

main() {
	SCRIPT_DIR="$(cd $(dirname "$0"); pwd)"

	while [ $# -gt 0 ]; do
		case "$1" in
			--help) usage; exit 0;;
			--verbose) set -x; shift;;
			--) shift; break;;
			-*)
				OPTIND=1
				while getopts h OPT "$1"; do
					case "$OPT" in
						h) usage; exit 0;;
					esac
				done
				shift
				;;
			*) break;;
		esac
	done

	# do something
	try() {
		expected="$1"
		input="$2"

		./1cc "$input" > tmp.s
		gcc -o tmp tmp.s
		./tmp
		actual="$?"

		if [ "$actual" = "$expected" ]; then
			echo "$input => $actual"
		else
			echo "$expected expected, but got $actual"
			exit 1
		fi
	}
	# try 6 2+++4 error
	try 11 "5 + 10 - 4;"
	try 47 "5+6*7;"
	try 15 "5*(9-6);"
	try 4 "(3+5)/2;"
	try 5 "-10+15;"
	try 1 "2>1;"
	try 0 "0>1;"
	try 1 "0<1;"
	try 0 "2<1;"
	try 1 "1<=1;"
	try 0 "1<=0;"
	try 1 "2==2;"
	try 0 "2==1;"
	try 1 "2!=1;"
	try 0 "2!=2;"
	try 4 "1+2;1*2+3;4/1;"
	try 1 "a=1;"
	try 1 "z=1;"
	try 3 "z=1;a=2;a+z;"

	echo OK
}

main "$@"

