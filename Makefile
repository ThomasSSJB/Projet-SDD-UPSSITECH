# Définition des variables
EXEC = prog
SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)
FLAGS = -Wall -ggdb -Iinclude -lm

# Règle principale
all: $(EXEC)

# Génération de l'exécutable à partir des fichiers objets
$(EXEC): $(OBJ)
	@gcc $^ -o $@ $(FLAGS)
	@echo "Exécutable généré : $(EXEC)"

# Génération des fichiers objets à partir des fichiers sources
%.o: %.c
	@gcc -c $< -o $@ $(FLAGS)
	@echo "Compilation : $< -> $@"

# Lancement de l'exécutable
run: all
	@./$(EXEC)

# Supprime les .o
clean:
	@rm *.o
	@echo "Dossier lib nettoyé"