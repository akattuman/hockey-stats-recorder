# hockey-stats-recorder

The program begins with a initial menu of options:

1. Pass
2. Ball Won
3. Ball Lost
4. Free Won
5. Penalty Conceded
6. Elimination
7. Defensive Error
8. Ball out of Play
9. D Entry
10. Penalty Corner
11. Opposition D Entry
12. Opposition Shot
13. Undo
14. End

From each of these, there is an appropriate submenu asking the user to input relevant data e.g. the number of the player(s) involved or the type of defensive error. If the event has location data, a pitch map will pop up and the user can click on the appropriate part of the pitch.

The menu structure is designed to carry some of the load for the user. For example, after the user records a D entry, we pass into a submenu for being in the attacking D. This includes the options to shoot or win a penalty stroke. The aim is to only show options which are relevant at that period of the game, thus allowing the user to record match events more efficiently without seeing all possibilities at all times.

At the end, a summary stats sheet as well as graphical pitchmaps are created for the team as well as each player individually. An example of a pitchmap would be one that shows where the team (or a particular player) lost the ball, and whether it was a tackle or an interception. Examples of these can be seen in the 'Sample Output' folder.
