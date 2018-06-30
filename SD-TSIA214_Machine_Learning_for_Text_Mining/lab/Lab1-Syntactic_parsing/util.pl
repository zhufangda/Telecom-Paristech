/*---------------------------------------------------------------*/
/* Telecom Paristech - J-L. Dessalles 2018                       */
/* Symbolic Natural Language Processing                 */
/*            http://teaching.dessalles.fr/SNLP                   */
/*---------------------------------------------------------------*/


/*
help :-
	write('verifier le fichier ''regles.txt''\n'),
	prompt1('>'),
	get0(_).
*/
help :-
	write('Bye...\n'),
	%prompt1('>'),
	sleep(1).


entree(L1, [M|L2]) :-
	read(M),
	!,
	entree(L1, L2).
entree(L, L).


get_line(Phrase) :-
        collect_wd(String), 
        str2wlist(Phrase, String).

collect_wd([C|R]) :-
        get0(C), C \== -1, C \== 10, C \== 13, !, 
        collect_wd(R).
collect_wd([]).

str2wlist(Phrase, Str) :-
	str2wlist([], Phrase, [], Str).
	
str2wlist(Phrase, [Mot|Phrase], Motcourant, []) :-
	reverse(Motcourant, Motcourant1),
        atom_codes(Mot, Motcourant1).
str2wlist(Phrasin, [Mot|Phrasout], Motcourant, [32|Str]) :-
	!,
	reverse(Motcourant, Motcourant1),
        atom_codes(Mot, Motcourant1),
	str2wlist(Phrasin, Phrasout, [], Str).
str2wlist(Phrasin, Phrasout, Motcourant, [C|Str]) :-
	str2wlist(Phrasin, Phrasout, [C|Motcourant], Str).

dcg2rules(GramFile) :-
	get_rules(GramFile).

get_rules(GramFile) :-
	retractall(rule(_, _)),
	see(GramFile),
	recupere,
	seen.

recupere :-
	%catch(read_clause(R), _, (write('Error in rules'), nl)),
	catch(read(R), _, (write('Error in rules'), nl)),
	%write(R),
	R =.. [-->, T|Q],
	!,
	transforme(T, Q, Q1),
	assert(rule(T, Q1)),
	recupere.
recupere.

transforme(T, [A], L) :-
	!,
	transforme(T, A, L).
transforme(T, (A, B), [A|B1]) :-
	!,
	transforme(T, B, B1).
transforme(T, (A; B), B1) :-
	!,
	assert(rule(T, A)),
	transforme(T, B, B1).
transforme(_, A, [A]).

%%%%%%% predicats utilitaires
writel([]).
writel([M|Ml]) :-
	write(M), write(' '),
	writel(Ml).



% "display_tree" realise l'affichage de l'arbre syntaxique
% la variable "Indent" est une chaine de caratere qui,
% affichee en debut de ligne, reproduit le dessin des
% branches en fonction de la position dans l'arbre.  

display_tree(StructPhrase) :-
	nl,
	display_tree("         ", "         ", StructPhrase),
	nl, nl.

display_tree(_, _, StructPhrase) :-
	StructPhrase =.. [LibPhrase],
	/* il s'agit d'un terminal */
	!,
	ecris([" : ", LibPhrase]).
display_tree(Indent, Prefixe, StructPhrase) :-
	%StructPhrase =.. [LibPhrase, AttrPhrase|SousStruct],
	%nl, ecris([Prefixe, LibPhrase, AttrPhrase]),
	StructPhrase =.. [LibPhrase|SousStruct],
	nl, ecris([Prefixe, LibPhrase]),
	afficheFils(Indent, SousStruct).

afficheFils(_, []).
afficheFils(Indent, [SP]) :-
	% c'est le dernier fils, on ne dessine plus
	% la branche parente                        
	!,
	string_concat(Indent, "   ", NewIndent),
	string_concat(Indent, "  |__", IndentLoc),
	display_tree(NewIndent, IndentLoc, SP).
afficheFils(Indent, [SP|SPL]) :-
	string_concat(Indent, "  |", NewIndent),
	string_concat(Indent, "  |__", IndentLoc),
	display_tree(NewIndent, IndentLoc, SP),
	afficheFils(Indent, SPL).

ecris([]).
ecris([S|Sl]) :-
	% string_to_list(S1, S),
	!,
	write(S),
	ecris(Sl).

