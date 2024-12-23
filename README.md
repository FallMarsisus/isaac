# Isaac

Jeu de fou

## Description
Isaac est un jeu développé en C utilisant la bibliothèque SDL2. Le jeu permet de contrôler un personnage, de se déplacer dans différentes salles et d'afficher des textes à l'écran.



## Compilation et exécution

### Linux

Pour compiler le projet sur Linux, utilisez la commande suivante :

```sh
make build
make run
```

### Windows

Pour compiler le projet sur Windows, utilisez la commande suivante :

```sh
make winbuild
./bins/game.exe
```


## Dépendances 

- SDL2
- SDL2_image
- SDL2_ttf
- Pleins de trucs de SDL2 en gros

Pour les installer sur Linux, utilisez la commande suivante (si c'est pas bon c'est la faute de copilot il a généré 90% de ce README) :

```sh
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```