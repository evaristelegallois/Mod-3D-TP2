THIEL Samantha, PAREL Gabriel, M1 Informatique

Projet de Modélisation de surfaces 3D (mars-avril 2021)


Compilation : clic droit sur visuCourbe.pro > "Ouvrir avec" > Qt Creator


Ce fichier fait office de mini-rapport afin de résumer ici nos fonctionnalités réussies, ainsi que les éléments manquants à l'issue de notre projet.


> Fonctionnalités et éléments graphiques :

    - une interface graphique avec une fenêtre OpenGL permettant de visualiser les objets géométriques dessinés ;
    
    - la possibilité d'afficher le polyèdre de contrôle en cochant la case correspondante ;
    
    - le type de dessin à choisir dans une liste déroulante pour les carreaux de Béziers : soit en mode surface (avec des triangles), soit en mode filaire (avec des lignes), ou soit en mode discret (avec des points) ;

    - le choix des valeurs des paramètres t et s ainsi que du pas de discrétisation ;
    
    - on peut faire tourner l'objet graphique selon l'axe x avec la touche X du clavier ;
    
    - on peut agrandir la fenêtre ;


> Ce qu'il manque / ce qu'il y aurait à améliorer :

    - quelques détails sont commentés dans le code, notamment à propos de la discrétisation ;
    
    - pour les fichiers 0BJ, nous avons essayé de décrire les données des points de contrôles dans un fichier mais nous n'avons pas pu le faire lire correctement (seulement une partie des carreaux de Béziers s'affichaient), nous avons donc décidé de laisser les points de contrôles en statique dans myOpenGLWidget.
