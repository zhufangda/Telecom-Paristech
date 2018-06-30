/*---------------------------------------------------------------*/
/* Telecom Paristech - J-L. Dessalles 2018                       */
/* Symbolic Natural Language Processing                 */
/*            http://teaching.dessalles.fr/SNLP                   */
/*---------------------------------------------------------------*/


% partial elementary English grammar

% --- Productions rules
s --> np(Number), vp(Number).
np(Number)--> det(Number), n(Number).		% Simple noun phrase
np(Number) --> np(Number), pp.		% Noun phrase + prepositional phrase 
np --> [kirk].
vp(Number) --> v(Number).           % Verb phrase, intransitive verb
vp(Number) --> v(Number), np.		% Verb phrase, verb + complement:  like X
vp(Number) --> v(Number), pp.		% Verb phrase, verb + indirect complement : think of X 
vp(Number) --> v(Number), np, pp.	% Verb phrase, verb + complement + indirect complement : give X to Y 
vp(Number) --> v(Number), pp, pp.	% Verb phrase, verb + indirect complement + indirect complement : talk to X about Y
pp --> p, np.		% prepositional phrase

% -- Lexicon
det(singular) --> [a].
det(plural) --> [some].
det(plural) --> [many].
det(_) --> [the].
det(_) --> [my].
det(_) --> [her].
det(_) --> [his].
n(singular) --> [dog].
n(singular) --> [daughter].
n(singular) --> [son].
n(singular) --> [sister].
n(singular) --> [aunt].
n(singular) --> [neighbour].
n(singular) --> [cousin].
n(plural) --> [dogs].
n(plural) --> [daughters].
n(plural) --> [sons].
n(plural) --> [sisters].
n(plural) --> [aunts].
n(plural) --> [neighbours].
n(plural) --> [cousins].
v(singular) --> [grumbles].
v(singular) --> [likes].
v(singular) --> [gives].
v(singular) --> [talks].
v(singular) --> [annoys].
v(singular) --> [thinks].
v(singular) --> [hates].
v(singular) --> [cries].
v(singular) --> [barks].
v(plural) --> [grumble].
v(plural) --> [like].
v(plural) --> [give].
v(plural) --> [talk].
v(plural) --> [annoy].
v(plural) --> [think].
v(plural) --> [hate].
v(plural) --> [crie].
v(plural) --> [bark].
p --> [of].
p --> [to].
p --> [about].