default: make1

make1:
        g++ Launcer.c -o Launcher -lm
      #  g++ launcherReceiver.c -o launcherReceiver -lm //to be defined yet G+1, G-1
        g++ P.c -o P -lm
        g++ S.c -o S -lm
        g++ L.c -lpthread -o L -lm


clean:
        rm -f Launcher
        #rm -f launcherReceiver
		    rm -f P
        rm -f S
        rm -f L
        #rm -f G-1
