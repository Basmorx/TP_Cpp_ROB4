# --- Variables de compilation ---
CXX = g++

# On définit le chemin pour pkg-config basé sur ton environnement Conda actif
PKG_ENV = PKG_CONFIG_PATH=$(CONDA_PREFIX)/lib/pkgconfig

# Flags de compilation demandés dans le sujet (-std=c++17) et inclusion des headers
CXXFLAGS = -std=c++17 -Wall -Wextra $(shell $(PKG_ENV) pkg-config --cflags eigen3 pinocchio)

# Flags d'édition de liens (les librairies Pinocchio/Eigen et le rpath pour éviter LD_LIBRARY_PATH)
LDFLAGS = $(shell $(PKG_ENV) pkg-config --libs eigen3 pinocchio) -Wl,-rpath,$(CONDA_PREFIX)/lib

# Flags spécifiques pour Google Test
LDFLAGS_GTEST = -lgtest -lgtest_main -pthread

# Liste des exécutables à générer
TARGETS = run_tests run_tests_bras simulateur

# Cibles factices (qui ne sont pas des fichiers)
.PHONY: all clean test test_bras run

# --- Règles de compilation ---

# Cible par défaut : compile tout
all: $(TARGETS)

run_tests: test_joints.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS) $(LDFLAGS_GTEST)

run_tests_bras: test_bras.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS) $(LDFLAGS_GTEST)

simulateur: simulateur.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

# --- Règles d'exécution ---

test_joints: run_tests
	./run_tests

test_bras: run_tests_bras
	valgrind --leak-check=full --track-origins=yes ./run_tests_bras

run: simulateur
	./simulateur

# --- Nettoyage ---
clean:
	rm -f $(TARGETS)