#!/bin/bash

EXEC="./codexion"

# Couleurs pour le terminal
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

if [ ! -f "$EXEC" ]; then
    echo -e "${RED}Erreur : L'exécutable $EXEC est introuvable. As-tu fait 'make' ?${NC}"
    exit 1
fi

echo -e "${YELLOW}=== TESTS DE GESTION DES ERREURS ===${NC}"
echo "Test d'un scheduler invalide (doit être rejeté)..."
$EXEC 4 410 200 200 0 5 10 random_scheduler
echo "Test avec des arguments manquants..."
$EXEC 4 410 200 200 0 5 10

echo -e "\n${YELLOW}=== TESTS DE SURVIE BASIQUE ===${NC}"
echo -e "${GREEN}Test 1 : FIFO - 4 codeurs tranquilles${NC}"
# Ils ont largement le temps (410ms) pour des actions de 200ms. Personne ne doit mourir.
$EXEC 4 410 200 200 0 5 10 fifo

echo -e "${GREEN}Test 2 : EDF - 4 codeurs tranquilles${NC}"
# Même chose, mais avec EDF. Le comportement doit être similaire.
$EXEC 4 410 200 200 0 5 10 edf


echo -e "\n${YELLOW}=== TESTS SOUS PRESSION (La vraie différence) ===${NC}"
# Explication : Avec 3 codeurs et des temps très serrés, il y aura forcément une file d'attente pour les dongles.
# FIFO donne le dongle au premier qui a demandé, même si un autre codeur va faire un "burnout" dans 1ms.
# EDF donne le dongle à celui qui a la deadline de burnout la plus proche[cite: 189].

echo -e "${RED}Test 3 : Le crash test FIFO (Un burnout est très probable ou attendu)${NC}"
$EXEC 3 120 40 40 0 5 10 fifo

echo -e "${GREEN}Test 4 : Le sauvetage EDF (Ils devraient tous survivre)${NC}"
# Ici, le scheduler EDF doit prioriser celui qui est sur le point de mourir, permettant au groupe de survivre.
$EXEC 3 120 40 40 0 5 10 edf


echo -e "\n${YELLOW}=== TEST AVEC REFACTORING ===${NC}"
echo -e "${GREEN}Test 5 : Cycle complet avec refactoring (EDF)${NC}"
$EXEC 5 800 200 200 100 3 20 edf
