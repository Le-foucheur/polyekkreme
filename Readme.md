Pour un VF du readme, [voir plus bas](#polyekkreme-vf)

# Polyekkreme VEN

Polyekkreme from εκκρεμη (ekkremé), meaning pendulum in plural, with the prefix πολυ- (poly-), meaning several.
It is a small software for simulating multiple pendulums using RK4 (Runge-Kutta 4).

**N.B.:** This project is part of my studies; do not expect a "good" simulation.

## Installation

To install the software, simply clone the repository via
```
git clone https://github.com/Le-foucheur/polyekkreme/tree/main
cd polyekkreme
```

## Usage

Once the configuration is done, run `make run` or `make run_plot` to plot the produced data at the same time.

## Configuration

You can modify the configuration, such as the integration time step $dt$, the number of pendulums directly in the file `config/config.cfg`.
For more details, see the file in question.

### Custom Pendulum
If you want to configure each pendulum individually, you need to set the `custom_pendulum` parameter to `true` in the config file.
Then, you can edit the file `config/pendule.csv`, where each line represents a pendulum and each column represents a parameter of the pendulum.

| id | mass | length | theta0 | omega0 |
| :---: | :---: | :---: | :---: | :---: |
| the pendulum identifier, it must be a unique positive integer | the mass of the pendulum | the length of the pendulum | the initial angle between the pendulum and the vertical | the initial radial velocity of the pendulum |

## Output

The software produces 3 files in `.csv` format in the `target/data/` folder:

- `pendule_coordonnee.csv`: which contains the coordinates $(x_i, y_i)$ of each pendulum $i$, for each calculated time $t$
- `pendule_energie.csv`: which contains the potential, kinetic, and mechanical energies for each time $t$
- `pendule_portrait.csv`: which contains the coordinates $(\theta_i, \dot{\theta}_i)$ for each pendulum $i$, for each time $t$

### Plotting

**Warning:** To plot the data, you need the [Typst](https://typst.app/) compiler (GitHub: https://github.com/typst/typst)

To plot the produced data, run `make plot` or `make run_plot` if you want to run the software before plotting.
Due to Typst limitations, plotting is limited to the first 10,000 lines for each file.

## Troubleshooting

### Incorrect Pendulum Display in TUI
The pendulums may not display correctly due to the fact that installed fonts are unable to render certain characters used (Unicode block: ["Symbols for Legacy Computing Supplement"](https://en.wikipedia.org/wiki/Symbols_for_Legacy_Computing_Supplement), octant block).

On Linux, you can check if you have fonts capable of displaying these characters using the command:
```
fc-list \:charset=1CD00
```

To fix this, you can set the `resolution` parameter to `false` in the config file.
This will switch to less precise characters for display, but ones that are more widely supported by fonts (Unicode block: ["Symbols for Legacy Computing"](https://en.wikipedia.org/wiki/Symbols_for_Legacy_Computing), sextant block).

If this still doesn't work, either your terminal cannot use a font that supports at least one of the two Unicode blocks,
or you need to install a font that supports them, for example:

- Catrinity: https://catrinity-font.de/characters.html
- Cozette: https://github.com/the-moonwitch/Cozette
- Google Noto: https://fonts.google.com/noto ([Noto Sans Symbols 2](https://fonts.google.com/noto/specimen/Noto+Sans+Symbols+2?preview.script=Latn&query=Symbols) with `resolution` set to `false`)

# Polyekkreme VF

**Polyekkreme** vient de εκκρεμη (ekkremé), « pendule » au pluriel, avec le préfixe πολυ- (poly-), signifiant « plusieurs ».
Il s’agit d’un petit logiciel de simulation de pendules multiples via RK4 (Runge-Kutta 4), avec un rendue en TUI.

**N.B. :** Ce projet est un projet réalisé dans le cadre de mes études ; n’y attendez pas une « bonne » simulation.

## Installation

Pour installer le logiciel, il vous suffit de cloner le dépôt via :
```
git clone https://github.com/Le-foucheur/polyekkreme/tree/main
cd polyekkreme
```

## Utilisation

Une fois la configuration effectuée, utilisez `make run` ou `make run_plot` pour tracer en même temps les données produites.

## Configuration

Vous pouvez modifier les paramètres, comme le pas de temps $dt$ d’intégration ou le nombre de pendules, directement dans le fichier `config/config.cfg`.
Pour plus de détails, consultez le fichier en question.

### Pendule personnalisé
Si vous souhaitez configurer individuellement chaque pendule, il faut mettre le paramètre `custom_pendulum` à `true` dans le fichier de configuration.
Ensuite, vous pouvez éditer le fichier `config/pendule.csv`, où chaque ligne représente un pendule et chaque colonne un paramètre du pendule :
   id | masse | longueur | theta0 | omega0 |
 | :---: | :---: | :---: | :---: | :---: |
 | L’identifiant du pendule, il doit être un entier positif unique | la masse du pendule | la longueur du pendule | l’angle initial entre le pendule et la verticale | la vitesse radiale initiale du pendule |

## Sortie

Le logiciel produit 3 fichiers au format `.csv` dans le dossier `target/data/` :

- `pendule_coordonnee.csv` : contient les coordonnées $(x_i, y_i)$ de chaque pendule $i$, pour chaque instant $t$ calculé ;
- `pendule_energie.csv` : contient les énergies potentielle, cinétique et mécanique pour chaque instant $t$ ;
- `pendule_portrait.csv` : contient les coordonnées $(\theta_i, \dot{\theta}_i)$ pour chaque pendule $i$, pour chaque instant $t$.

### Tracé des graphiques

**Attention :** Pour tracer les données, vous avez besoin du compilateur [Typst](https://typst.app/) (GitHub : https://github.com/typst/typst).

Pour tracer les données produites, utilisez `make plot` ou `make run_plot` si vous souhaitez lancer le logiciel avant de tracer les graphiques.
En raison des limitations de Typst, le tracé est limité aux cas où les `.csv` font au plus 10 000 lignes pour chaque fichier.

## Troubleshooting

### Mauvais affichage des pendules dans le TUI
Il se peut que les pendules ne s’affiche pas correctement, c’est du au faits que les polices installées sont incapable de rendre certains caractères utilisés ( block unicode : [« Symbols for Legacy Computing Supplement »](https://en.wikipedia.org/wiki/Symbols_for_Legacy_Computing_Supplement), block octant ).

Sur linux vous pouvez vérifier si vous avez des polices capable d’y afficher via la commande :
```
fc-list :charset=1CD00
```

Pour régler celà, vous pouvez passer mettre à `false` le paramètre `resolution` dans le fichier de config\
Celà auras pour effet de passer sur des caratère moins précis pour l’affichage mais plus supporté par les polices d’écriture ( block unicode : [« Symbols for Legacy Computing »](https://en.wikipedia.org/wiki/Symbols_for_Legacy_Computing), block sextant ).

Si celà ne marche toujours pas c’est que, soit votre terminal ne peut utilisé de telle police qui support au moins un des deux block unicode.
Soit il vous faut installer une police qui les support, par exemple :

 - Catrinity : https://catrinity-font.de/characters.html
 - Cozette : https://github.com/the-moonwitch/Cozette
 - Google Noto : https://fonts.google.com/noto ([Noto Sans Symbols 2](https://fonts.google.com/noto/specimen/Noto+Sans+Symbols+2?preview.script=Latn&query=Symbols) avec `resolution` mis à `false`)
