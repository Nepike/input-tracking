from pyswip import Prolog
prolog = Prolog()
prolog.consult("rules.pl")
actions = [...]
result = list(prolog.query("valid_save_sequence(%s)" % actions))


# rules.pl

# is_swipe_right(Gesture) :-
#   Gesture.start_coords(X1, _),
#   Gesture.end_coords(X2, _),
#   X2 > X1 + 50.

# is_double_click(Actions) :-
#   findall(Time, member(action(Time, mouse, _, pressed), Actions), Times),
#   consecutive(T1, T2, Times),
#   T2 - T1 < 0.5.