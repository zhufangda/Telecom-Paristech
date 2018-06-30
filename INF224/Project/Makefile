##########################################
#
#  Makefile pour client/serveur TCP
#  Eric Lecolinet - Reda Dehak - Telecom ParisTech 2015
#  INF224 - TP C++ - http://www.telecom-paristech.fr/~elc/inf224
# 
##########################################

#
# Nom du programme
#
SERVER=server
CLIENT=client
CLISERV=cliserv

#
# Fichiers sources (NE PAS METTRE les .h ni les .o mais seulement les .cpp)
#
SERVER_SOURCES=cpp/server.cpp cpp/tcpserver.cpp cpp/cppsocket.cpp cpp/Multimedia.cpp cpp/Photo.cpp cpp/Video.cpp cpp/Film.cpp cpp/Group.cpp cpp/MultimediaManager.cpp
#
# Fichiers objets (ne pas modifier, sauf si l'extension n'est pas .cpp)
#
SERVER_OBJETS=${SERVER_SOURCES:%.cpp=%.o}

#
# Compilateur C++ (suivant les systemes CXX ou CCC)
#
CXX= c++
JC= javac

#
# Options du compilateur C++
#   -g pour debugger, -O optimise, -Wall affiche les erreurs, -I pour les headers
#   -std=c++11 pour C++11
# Example: CXXFLAGS= -std=c++11 -Wall -O -I/usr/local/qt/include
#
CXXFLAGS= -std=c++11 -Wall -g
JFLAGS = -g
#
# Options de l'editeur de liens
#
LDFLAGS= 

#
# Librairies a utiliser
# Example: LDLIBS = -L/usr/local/qt/lib -lqt
#
LDLIBS= -lpthread -lboost_system -lboost_filesystem


##########################################
#
# Regles de construction/destruction des .o et de l'executable
# depend-${PROG} sera un fichier contenant les dependances
#

all: ${CLIENT} ${SERVER}

run: run-${CLIENT} run-${SERVER}

run-${SERVER}:
	./${SERVER}

run-${CLIENT}: ${CLIENT}
	cd swing; (java Main &)

${CLIENT}:
	cd swing; javac -g Main.java ClientFrame.java 

${SERVER}: depend-${SERVER} ${SERVER_OBJETS}
	${CXX} -o $@ ${CXXFLAGS} ${LDFLAGS} ${SERVER_OBJETS} ${LDLIBS}

clean:
	-@$(RM) *.o depend-${SERVER} *.class depend-${CLIENT} core 1>/dev/null 2>&1

clean-all: clean
	-@$(RM) -${CLIENT} -${SERVER} 1>/dev/null 2>&1
  

# Gestion des dependances : creation automatique des dependances en utilisant 
# l'option -MM de g++ (attention tous les compilateurs n'ont pas cette option)
#

depend-${SERVER}:
	${CXX} ${CXXFLAGS} -MM ${SERVER_SOURCES} > depend-${SERVER}

###########################################
#
# Regles implicites
#

.SUFFIXES: .cpp .cxx .c

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c (CFLAGS) $(INCPATH) -o $@ $<


#############################################
#
# Inclusion du fichier des dependances
#
-include depend-${SERVER}
