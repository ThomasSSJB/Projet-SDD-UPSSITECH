# Définition des variables
EXEC = bin/prog
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=lib/%.o)
FLAGS = -Wall -ggdb -Iinclude -lm

# Règle principale
all: $(EXEC)

# Génération de l'exécutable à partir des fichiers objets
$(EXEC): $(OBJ)
	@gcc $^ -o $@ $(FLAGS)
	@echo "Exécutable généré : $(EXEC)"

# Génération des fichiers objets à partir des fichiers sources
lib/%.o: src/%.c
	@gcc -c $< -o $@ $(FLAGS)
	@echo "Compilation : $< -> $@"

# Lancement de l'exécutable
run: all
	@./$(EXEC)

# Supprime les .o
clean:
	@rm lib/*.o
	@echo "Dossier lib nettoyé"