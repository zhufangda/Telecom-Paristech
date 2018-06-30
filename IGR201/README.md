# IGR

## B1.1
Un siganl est émis vers le monde extérieur
- par un objet quand il change d'état
- on n'indique pas à qui il s'adresse

Un slot est un récepteur
- en partique c'st une method

Les signaux sont connectés à des slots, les slots alors appelés automatiquement

Avantage/inonvénients
- le même signal peut être connecté à plusieurs slots
- plusieurs signaux peuvent être connectés à un même slot
- il ne sait pas si le signal est réçu.
- Le recepteur n'a pas besoin de connaître le ou les récepteur
- SLOT et SIGNAL sont des macros-> phase de precompilation

```c++
QObject::connect(quitBtn, SIGNAL(clicked()), app, SLOT(quit()));
```

## B2
Solution 1: On peut utilise <code>QObject::sender()</code> dans la definition de
le slot afin d'obtenir l'émetteur.

Solution 2: On peut utilise <code> QSignalMapper</code> afin d'établir une relation 
entre un senble de signaux sans paramètre et un signal ou un slot ayant un seul 
paramètre.

Solution 3: On peut utilise <code>QActionGroup</code>, et récupère l'action via le 
paramètre de slot.

## B3
On peut lier les signaux des objects créés interractivement avec n'import quel slot 
- par auto-connexion. On crée un slot avec un signature sous format 
<code> on_objectName_signalName(....)</code>, ce slot va connecter automatiquement avec 
le signal indiqué par son signature.
- ou via le mode Edition Signaux/Slots de QtCreator

## B4
Schéma vu PPT 52

<code> update()</code> indique qu'une zone est endommagée. Cette fonction va mettre
les demandes dans une file d'attente zone. 
<code> repaint() </code> entraine un réaffichage immédiat.

<code> paintEvent()</code> effectue le réaffichage. il est automatiquement appelée 
quand il faut réafficher.

On doit affihcer le dessin dans <code>paintEvent()</code> et ne pas appeler
<code> paintEvent()</code> directement.

## B5
Path
- figure composeé d'une suite arbitraire de lignes et courbes
  - affiché par: <code>QPainter::drawPath()</code>
  - peut aussi servir pour remplissage, profilage, découpage

Clipping
  - decoupage de la figure avec un forme indiqué

Alpha-compostion
  - une composition transparente

Anti-aliasing
- éviter l'effect d'escalier
- particulièrement utile pour les polices de caractère

## B6
[活动(activity)的生命周期](http://noahzu.github.io/2015/12/04/%E6%B4%BB%E5%8A%A8-activity-%E7%9A%84%E7%94%9F%E5%91%BD%E5%91%A8%E6%9C%9F)
- onCreate(): Caché --活动被创建时执行 
- onStart(): Visible --活动开始可见时执行
- onResume(): Visible -- 活动准备好跟用户交互时执行
- onPause(): partiellement Visible -- 活动失去焦点时执行
- onStop(): Caché --活动完全被覆盖时执行，活动任在内存，未被销毁
- onDestory(): Caché --内存不足时，活动销之前执行
- OnRestart(): Visible --从stop状态回到start状态过过程中执行


"# IGR201" 
