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
			echo "$input => $actual \e[32;1mOK\e[m"
		else
			echo "$input => $actual \e[31;1mFAILED\e[m\n$expected expected, but got $actual"
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

	# variable
	try 1 "a=1;"
	try 1 "z=1;"
	try 3 "z=1;a=2;a+z;"
	try 5 "a=1;a=5;a;"
	try 1 "return 1;"
	try 3 "a=2;b=1;return a+b;"
	try 5 "a=2;b=3;a=a+b;return a;"
	try 2 "a=2;return a;return 0;"
	try 2 "hoge=2;return hoge;"
	try 4 "hoge=2;fuga=2;return hoge*fuga;"

	# 制御構文
	try 5 "if(1==1) 5;"
	try 2 "if(1!=1) 5; else 2;"
	try 3 "if(0) 5; else if(0) 2; else 3;"
	try 2 "if(0) 5; else if(1) 2; else 3;"
	try 2 "if(0) 5; if(1) 2;"
	try 2 "if(0) a=5; if(1) a=2;return a;"
	try 5 "if(1) a=5; if(0) a=2;return a;"
	try 6 "b = 0;for(a = 1; a < 4; a = a+1) b = b + a;return b;"
	try 4 "b = 0;for(a = 1; a < 4; a = a+1) b = b + a;return a;"
	try 36 "b = 0;for(a = 1; a < 4; a = a+1) b = b + a;for(a = 1; a < 4; a = a+1) b = b * a;return b;"
	try 30 "b = 0;for(a = 0; a < 10; a = a+1) if(a>5) b = b + a;return b;"
	try 6 "b = 0;for(;;b = b+1) if(b > 5) return b;"
	try 6 "b = 0;while(b<6) b = b+1;return b;"
	try 8 "b = 1;while(b<6) b = b*2;return b;"


	echo OK
}

main "$@"

