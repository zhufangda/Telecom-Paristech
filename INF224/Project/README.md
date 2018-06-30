# INF224 README

## HOW TO COMPLIER AND RUN
Utiliser le command ci-dessous
```makefile
make run
```
The programe will complie and execute automatiquement. Le serveur va parcourir le sous-dossier "testset/video" et "testset/photo" comme un base de donné.

## How to use client

Le client support deux type de mode d'opération
Vous pouvez saisir le nom de multimédia dans le text filed et taper button "play". Le multimedia va afficher. Vous pouvez aussi saisir le nom de goupe ou nim de multimedia pour afficher son info.

Le client aussi supporte le mode de command. Vous pouvez saisir le command et appuyer sur "Send". Vous trouvez ci-joint le command

show : afficher le list de multimedia
show group: affihicher le list de group
remove group [groupname]: supprimer le multimedia indiqué par filaname
remove file [filename]: supprimer le multimedia indiqué par filaname
search file [filename]: recherche le fichier et affiher son info
search group [filename]: recherche le fichier et afficher son info
play [filename]: play the multimedia specified by filename 