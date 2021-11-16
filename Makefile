license-manager: main.cc
	g++ main.cc -I/usr/local/lib -llicensepp -lcryptopp -std=c++2a -O3 -o license-manager


