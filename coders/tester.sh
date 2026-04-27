#!/bin/bash

EXEC="./codexion"
VALGRIND="valgrind --tool=helgrind"

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${YELLOW}=== TESTS HELGRIND : CHASSE AUX DATA RACES ===${NC}"
echo -e "Rappel : Helgrind ralentit le programme. Des burnouts prématurés sont normaux."
echo -e "L'objectif est d'avoir 0 erreur (ou 1 seule erreur si c'est le faux positif glibc).\n"

# ---------------------------------------------------------
echo -e "${CYAN}Test 1 : Le Solitaire (1 codeur)${NC}"
echo "Objectif : Vérifier que la routine spéciale à 1 codeur n'a pas de fuite/data race au moment de la mort."
$VALGRIND $EXEC 1 800 200 200 100 2 20 fifo 2>&1 | grep "ERROR SUMMARY"

# ---------------------------------------------------------
echo -e "${CYAN}Test 2 : Le Trafic Fluide (4 codeurs, FIFO)${NC}"
echo "Objectif : Vérifier la gestion classique des mutexes et des files d'attente sans conflit majeur."
$VALGRIND $EXEC 4 800 200 200 100 2 20 fifo 2>&1 | grep "ERROR SUMMARY"

# ---------------------------------------------------------
echo -e "${CYAN}Test 3 : Le Conflit Direct & Tie-Breaker (5 codeurs, EDF)${NC}"
echo "Objectif : Forcer 3 threads à réclamer les mêmes dongles en même temps avec la même deadline."
# Des temps très courts forcent le CPU à faire beaucoup de context switches, ce qui trahit les data races.
$VALGRIND $EXEC 5 150 40 10 0 2 10 edf 2>&1 | grep "ERROR SUMMARY"

# ---------------------------------------------------------
echo -e "${CYAN}Test 4 : Le Crash Test d'Arrêt d'Urgence (5 codeurs, FIFO)${NC}"
echo "Objectif : Un burnout rapide. Vérifier que la fonction ft_stop_all du monitor coupe proprement tous les threads sans data race."
$VALGRIND $EXEC 5 100 80 80 0 10 10 fifo 2>&1 | grep "ERROR SUMMARY"

echo -e "\n${GREEN}=== FIN DES TESTS ===${NC}"