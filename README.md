# coders:
Master POSIX threads (pthreads) and mutexes to manage shared states, avoid deadlocks, and prevent "burnout" (process starvation) under strict timing constraints.

Le but de Codexion est d'apprendre à maîtriser le multithreading (faire tourner des choses en même temps avec des threads)  et la synchronisation.

When you run a program on your computer (like a web browser, a video game, or your C program), the operating system creates what is called a process.
    The Process: This is simply the program currently executing. It is given its own memory space, its own variables, and its own code.

    The PID (Process IDentifier): This is a unique identification number that the operating system assigns to this process. This number allows your computer's system to track and manage the process (like giving it more memory or forcing it to stop).


If your program needs to do several things at the exact same time. You have two main options:

1 Create new processes (using fork())
When a process creates a new process (the "child"), it makes an exact copy of itself.
The child receives a new PID, completely different from the parent's PID.
Memory is separated: The child gets a full copy of all the parent's variables. But remember, these are just copies. If the child changes a variable in its own memory, it changes absolutely nothing in the parent's memory. They are completely isolated from one another.

2 Create Threads (using pthread_create())
A thread is a line of execution that lives inside an already existing process.
they all share the same PID.
Memory is shared: This is the most crucial difference. Threads do not copy memory. They all use the exact same central memory of the process. If Thread A modifies a variable, Thread B will see this change instantly because they are looking at the exact same physical spot in the computer's memory.



the process : /.codexion 
thread : codeurs 
mutext : cadnas qui va sur chaque dongle pour eviter que 2 thread ne prenne le meme dongle



The general idea is to pick the right tool for the job you are trying to do:

    Processes (separated): You use them when you want to run completely independent tasks that shouldn't interfere with each other (like launching two different programs). The advantage is safety (one process crashing won't break the other's memory), but communication between them is slow and complicated.

    Threads (shared): You use them when you have one big task that you want to split into smaller workers that collaborate on the same data (like your Codexion project). The advantage is that communication and data sharing are instant and very fast. The downside (and the big trap) is that you must be very careful to ensure two threads don't try to change the same piece of data at the exact same millisecond, otherwise the data gets corrupted. This is exactly why you are required to use tools like "mutexes" to protect your shared variables!



Race condition:
A race condition occurs when two threads try to modify the same shared variable at the exact same time, which completely corrupts the data.

Why does the computer make mistakes?
An operation, such as ++ (incrementing), is not instantaneous; it happens in 3 distinct steps for the processor:

    Read: The thread reads the current value from the shared program memory/RAM.

    Increment: The thread adds +1 to this value in its own internal register (the value inside the CPU).

    Write: The thread rewrites the new final value back into the shared memory.

The problem arises because the operating system can put a thread "on pause" at any given microsecond to let another thread work.

It is precisely because step 2 (the calculation) is done on a private scratchpad in the CPU that chaos ensues. If Thread 1 and Thread 2 read the same value from the table at the same time, they will both add +1 on their private scratchpads independently, without knowing what the other is doing. The last one to write its scratchpad back to the table will completely overwrite the work of the first one!



how to solve this issue us MUTEXT
pthread_mutex_t mutex;  --> creat mutex
pthread_mutex_init(&mutex, NULL) --> init mutex pourquoi faire cela apres la creation du mutex ?
pthread_mutex_destroy(&mutex) --> Free le mutex 
pthread_mutex_lock(&mutex); --> lock operation en cours protect against other thread trying to run the code at the same time 
phtread_mutex_unlock(&mutex) --> laisse la place libre 
pthread_join                 --> suspends the execution of the current thread until the specified thread terminates, allowing you to retrieve its return value and cleanly release its system resources.



to creat phread inside a loop:
phread th;
while i < nbr_coders
    if (phread_creat(th[i]), NULL, &routine, NULL) != 0
    perror("failed to creat thread")
    return -1;

while i < nbr_coders
    if (phread_join(th[i], NULL) != 0 )
        return ??,

pthread_mutex_destroy(&mutex);


do 2 loops because if you do in the same loop we will not have multithread 




get return of thread function into the main :

*void name_function()
int value = (valuer quon veut)
int* result = malloc(sizeof(int))

*result = value;
return (void*) result,

phread_join(th , (void **) &res)
take a function void **


The Mutex (pthread_mutex_t): It protects access to your data (e.g., is_ready). It ensures that no one reads is_ready while you are modifying it.

The Condition Variable (pthread_cond_t): It is used for time synchronization. It doesn't protect anything at all. Its sole purpose is to tell threads: "Sleep here until someone wakes you up."





1 etape parsing 
2 etape init dongle , coders et une simulation qui permettera de faire le top depart.

chaque etape penser a free correctement ce qui a ete deja cree tout comme la destrution des mutex !

comment fonctionne l etape de la simulation : 

pthread_mutex_init(&data->sim_mutex, NULL) != 0)
Le Mutex sert à protéger la pancarte is_ready pour qu'elle ne soit pas lue/modifiée par 10 personnes en même temps.
C'est ce qu'on appelle une "Race Condition" (ils font la course pour lire la donnée). Le mutex les force à se mettre en file indienne pour lire l'information proprement.


    1. On prépare le mécanisme (l'initialisation)
    pthread_cond_init(&data->start_cond, NULL);

    2. Plus tard, le thread dit : "Je m'endors en attendant un signal"
    (Il libère le mutex et s'endort grâce à start_cond)
    pthread_cond_wait(&data->start_cond, &data->sim_mutex);

    3. Encore plus tard, le main dit : "Allez, réveillez-vous tous !"
    pthread_cond_broadcast(&data->start_cond);


pthread_cond_init(&data->start_cond, NULL):
Une variable de condition n'est pas une simple variable comme un int ou un bool. C'est un objet complexe du système (le noyau) qui contient une file d'attente.

    Allocation/Préparation : pthread_cond_init indique au système : "Je vais utiliser cette zone mémoire pour gérer une file d'attente de threads qui dorment".

    Création de la file : Elle prépare les structures internes pour que, plus tard, quand tu feras un pthread_cond_wait, le système sache exactement où mettre le thread pour qu'il s'endorme sans consommer de CPU.



lors de la creation de la simulation : 
on commence par creer les coders qui font faire un appel a une routine 
chaque phread va etre cree puis ils vont s endormir pour attendre que tous le monde soit cree 
si y a un probleme la demarche a suivre est la suivante : 

    Signaler l'erreur : is_ready = -1 + broadcast

    Attendre tout le monde : boucle pthread_join.

    Nettoyer tout (Free/Destroy) : appele fonction de nettoyage global.

ensuite on lance la creation du monitor : 

si y a un probleme on suit globalement le meme schema que plus haut :
Le signal d'arrêt : En mettant is_ready = -1 et en faisant un broadcast, tu réveilles tous tes coders qui dormaient sur pthread_cond_wait.

Le déblocage : threads sortent de leur wait, vérifient is_ready, voient qu'il vaut -1, font un unlock du mutex, et se terminent (return (NULL)).

La synchronisation : Appel à ft_finish_simulation(data) (qui contient pthread_join) force le thread principal à attendre que chaque thread ait réellement fini son exécution.

Le nettoyage sécurisé : Une fois que ft_finish_simulation a rendu la main, tu es garanti qu'aucun thread ne tourne plus. ft_release_all en toute sécurité dans ton main sans risquer de détruire un mutex pendant qu'il est utilisé.



le Scheduler avec File de Priorité.:
Plutôt que de laisser les codeurs se jeter sur les dongles il faut mettre en place un système de "Ticket".

    Un codeur veut un dongle.

    Il demande au dongle de s'inscrire dans sa file d'attente (en donnant son id et son heure limite avant le burnout).

    Le codeur s'endort (pthread_cond_wait).

    Quand le dongle est libre, c'est le dongle lui-même (ou plutôt la fonction de libération) qui regarde sa file d'attente, choisit le gagnant selon la règle (FIFO ou EDF), et le réveille (pthread_cond_signal).







actions_dongle : 
attribution des dongles lors du premier tour : 

algorithmique, la Hiérarchie des ressources (ou solution de Dijkstra). En forçant tous tes threads à toujours acquérir les ressources partagées (les mutex des dongles) dans un ordre global strict (du plus petit ID au plus grand), 
je brise la condition d'attente circulaire. Il est mathématiquement impossible d'avoir un deadlock général avec cette méthode.
Le problème : L'Attente Circulaire (Le scénario catastrophe)

1. Le problème : L'Attente Circulaire (Le scénario catastrophe)

Imaginons la méthode naïve, où chaque codeur prend toujours son dongle de gauche d'abord, puis celui de droite. Prenons une table avec 3 codeurs et 3 dongles :

    Codeur 1 est entre le Dongle 1 (gauche) et le Dongle 2 (droite).

    Codeur 2 est entre le Dongle 2 (gauche) et le Dongle 3 (droite).

    Codeur 3 est entre le Dongle 3 (gauche) et le Dongle 1 (droite).

Si tous les codeurs démarrent exactement en même temps et sont parfaitement synchronisés :

    Codeur 1 prend le Dongle 1.

    Codeur 2 prend le Dongle 2.

    Codeur 3 prend le Dongle 3.

Résultat : Tout le monde a un dongle en main. Maintenant, tout le monde cherche son deuxième dongle :

    Codeur 1 veut le Dongle 2... qui est bloqué par le Codeur 2.

    Codeur 2 veut le Dongle 3... qui est bloqué par le Codeur 3.

    Codeur 3 veut le Dongle 1... qui est bloqué par le Codeur 1.

C'est l'attente circulaire. La boucle est fermée. Personne ne lâchera son premier dongle avant d'avoir le deuxième, et le deuxième n'arrivera jamais. Le programme est figé pour l'éternité (ou jusqu'au burnout).

2. La solution : La Hiérarchie des Ressources (Ton code)

Ton code introduit une règle absolue et universelle : "On prend TOUJOURS le dongle avec le plus petit ID en premier, peu importe s'il est à gauche ou à droite."

Reprenons exactement la même table de 3 codeurs avec cette nouvelle règle :

    Codeur 1 a les Dongles 1 et 2. Il doit prendre le 1 en premier.

    Codeur 2 a les Dongles 2 et 3. Il doit prendre le 2 en premier.

    Codeur 3 a les Dongles 3 et 1. Il doit prendre le 1 en premier (car 1 < 3).



Les Codeurs = Les Threads : Comme tu l'as très bien souligné, le sujet impose que chaque codeur soit représenté par un thread (pthread_create).

Les Dongles = Les Mutexes (Ressources partagées) : dois protéger l'état de chaque dongle avec un mutex (pthread_mutex_t) pour éviter qu'ils ne soient dupliqués par les codeurs.




dans is_ready de la simulation jai 3 etat ok = 1 stand-by = 0 erreur = -1 


simulation cest la (Synchronisation au démarrage)
car les thread parte tous en meme temps. lobjectif cest de les faire commencer seulement quand tous le monde est creer .

Résumé de l'utilité : 
Élément             Rôle                        Pourquoi ?
is_ready            Barrière de départ          Garantit que tous les codeurs partent en même temps.
start_cond          Signal de départ            Réveille tous les threads endormis d'un coup.
stop_simulation     Interrupteur global         Arrête proprement tous les threads lors d'un burnout 
                                                ou fin de mission.
sim_mutex   Gardien                     Empêche les Data Races sur les flags ci-dessus.


lors de la creation des thread si y a un probleme :

Le secret : pthread_cond_wait déverrouille le mutex pour toi

Quand tu appelles pthread_cond_wait(&data->start_cond, &data->sim_mutex);, la fonction exécute une opération indivisible en trois étapes précises :

    Elle met le thread en sommeil.

    Elle DÉVERROUILLE automatiquement le mutex (data->sim_mutex).

    Elle attend le signal de réveil.

chaque thread va etre plonger dans un sommeil attendant le signal de depart ou de lerreur. 

pthread_cond_broadcast réveille tous les threads en attente afin qu'ils vérifient la valeur modifiée de is_ready (-1), leur permettant ainsi de sortir de leur boucle d'exécution et de terminer proprement pour que le thread principal puisse les join sans interblocage.

lors du broadcast deux chemin peux se faire : 
les threads se reveille mais non pas encore la main sur le mutex donc il attend , 
une fois le mutex_unlock de la simulation fait 
le threads qui est dans la condition wait recupere le lock et travaille soit il va dans la boucle erreur soit il passe le temoins a un autre thread . 
il n y a donc jamais de lock ou d unlock 2 fois 


ensuite tu fais la meme chose avec le monitor 

puis une fois que tout le monde est sur la ligne de depart tu broadcast le message go !! 
tout le monde sactive en meme temps 

le but de monitor : 
verifier quand arreter le programme 
lors dun bournout 
ou lorsque tout les thread ont compiler suffisameent 

precission : 
usleep n'est pas "temps réel", usleep ne garantit pas une précision à la microseconde près.

    Quand tu fais usleep(1000), tu dis au système d'exploitation : "S'il te plaît, endors ce thread pour au moins 1 milliseconde".

    Le système d'exploitation (Linux/macOS) a son propre planificateur de threads (scheduler). Il est possible que le système mette un peu plus de temps à réveiller ton thread (par exemple 1.2ms ou 1.5ms) s'il est très occupé par d'autres tâches.



pour la fonction du stop on s adresse on dongle et pas au thread : 
Pourquoi s'adresser aux dongles ?

Si tu ne signales pas la condition du dongle (dongle->cond), le thread qui attend ce dongle va rester "figé" dans son pthread_cond_timedwait jusqu'à la fin de son timeout (le temps que tu as défini dans ft_wait_for_dongle).

Même si tu mets data->stop_simulation = true dans ta structure, le thread, lui, ne le sait pas ! Il est en train de dormir profondément dans la bibliothèque système.

En faisant pthread_cond_broadcast sur la condition du dongle :

    Tu forces le réveil immédiat de tous les threads qui attendaient sur ce dongle précis.
    Le thread se réveille, sort de son wait.
    Il revient dans la boucle while (ft_check_simulation_stop(data) == 0) dans ft_coder_routine.
    À ce moment-là, il vérifie le flag stop_simulation qui est désormais à true, et il s'arrête proprement.



ensuite tout le monde registre ces propre dongle dans leur liste dattente 
codeur 1 enregistre le 0 et le 1 
le codeur 2 veut enregistrer le 1 et le 2 dongle mais il ne peux pas car le 1 a deja le dongle 1 
le codeur 3 enregistre le 2 et le 3




resumer de ce qui se passe apres le top depart : 
Dans un programme multithreadé, il faut imaginer que plusieurs "personnes" (les threads) travaillent en même temps. Voici exactement ce qu'il se passe de manière chronologique dans ton code :
1. Le chef d'orchestre (le Thread Principal)

C'est le thread qui lance le programme. Son rôle dans ft_start_simulation est de :

    Créer les codeurs (ft_create_coders).

    Créer le monitor (pthread_create(&data->monitor, ...)).

    Donner le signal de départ (le fameux pthread_cond_broadcast).

    Se mettre en pause en appelant pthread_join sur les codeurs (via ft_finish_simulation). Il attend que les codeurs aient terminé.

    Une fois les codeurs terminés, il appelle pthread_join(data->monitor, ...) pour attendre que le monitor finisse.

2. Les ouvriers (Les Threads Codeurs)

Dès qu'ils reçoivent le signal de départ, ils rentrent dans leur boucle while (ft_check_simulation_stop(data) == 0). Ils vont boucler sans fin (prendre les dongles, compiler, debug, etc.) tant que personne ne change la variable stop_simulation.
3. Le surveillant (Le Thread Monitor)

Lui, il a sa propre fonction (ft_monitor_routine). Dès le signal de départ, il fait ça :

    Il rentre dans une boucle infinie.

    Il regarde l'heure (ft_check_status).

    S'il voit qu'un codeur a dépassé le temps (now - last_compile_start > time_to_burnout) :

        Il change data->stop_simulation = true.

        Il affiche le message burned out.

        Il quitte sa fonction avec un return. (C'est à ce moment précis que le thread monitor "meurt" / se termine).

L'effet domino (La fin de la simulation)

Voici comment tout s'emboîte au moment où le monitor détecte la mort :

    Le Monitor met stop_simulation à true et se termine (il a fini son job).

    Au tour de boucle suivant, les Codeurs voient que stop_simulation est à true. Ils quittent leur boucle while et se terminent.

    Le Thread Principal, qui était bloqué dans son pthread_join à attendre les codeurs, est débloqué (puisqu'ils viennent de se terminer).

    Le Thread Principal passe à la ligne suivante : le pthread_join(data->monitor, ...). Comme le monitor est déjà terminé (depuis l'étape 1), ce join se valide instantanément sans attendre.

    Le programme principal s'arrête.