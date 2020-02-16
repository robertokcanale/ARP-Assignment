
default: make1

make1:
	g++ Launcher.c -o Launcher -lm
  g++ P.c -o P -lm
  g++ S.c -o S -lm
  g++ L.c -o L -lm
	g++ G.c -o G -lm
	g++ G_plus_one_socket_test.c -o G_plus_one_socket_test -lm

clean:
  rm -f Launcher
	rm -f P
  rm -f S
  rm -f L
	rm -f G
	rm -f G_plus_one_socket_tests
