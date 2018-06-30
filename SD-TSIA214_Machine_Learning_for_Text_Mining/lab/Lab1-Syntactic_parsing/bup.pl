/*---------------------------------------------------------------*/
/* Telecom Paristech - J-L. Dessalles 2009                       */
/*            http://teaching.dessalles.fr                       */
/*---------------------------------------------------------------*/

	%%%%%%%%%%%%%
	% bottom-up recognition  %
	%%%%%%%%%%%%%

:- consult('util.pl').     % input-output routines
:- dcg2rules('Grammar.pl').      % performs the conversion by asserting rule(np, [det, n])

bup([s]).  % success when one gets s after a sequence of transformations
bup(P):-
	write(P), nl, get0(_),
	append(Pref, Rest, P),   % P is split into three pieces 
	append(RHS, Suff, Rest), % P = Pref + RHS + Suff
	rule(X, RHS),	% bottom up use of rule
	append(Pref, [X|Suff], NEWP),  % RHS is replaced by X in P:  NEWP = Pref + X + Suff
	bup(NEWP).  % lateral recursive call

go :-
	bup([the, sister, talks, about, her, cousin]).
