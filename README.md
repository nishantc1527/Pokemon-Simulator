
# Pokemon-Simulator
A Text Based Pokemon Simulator Game, With The Ability To Create Teams And Battle.
# Running The Program
First clone the repository.
```
$ git clone https://github.com/nishantc1527/Pokemon-Simulator.git
$ cd Pokemon-Simulator/Pokemon
```
Then in cygwin (or in git bash if you have g++ installed) run it.
```
$ g++ main.cpp -o main
$ ./main
```
# Contributing
Contributing is welcome and appreciated. Even if you don't know c++, you can contribute in easy ways. Here are some ways you can contribute:
## Add A Pokemon
In the pokemon_data.txt file, you can add pokemon in this format:  
\# pokemon_name type1 type2 hp attack defense special_attack special_defense speed  
move_1  
move_2  
move_3  
...  

pokemon_name is the name of the pokemon  
type1 is the first type  
type2 is the second type (if it has no second type, put a star *)  
hp, attack, defense, special_attack, special_defense, and speed are their corresponding stat. Example:  

\# Azumarill water fairy 100 50 80 60 80 50  
aqua jet  
play rough  
tackle  
knock off  
... 

Of course it has more moves.  

The last step is to go to [this website](https://manytools.org/hacker-tools/convert-images-to-ascii-art/) to generate the text images. Look up the image of the Pokemon online, submit it into the website, and get your ascii image. Then, copy and past that into a text file called name_of_the_pokemon.txt.

## Add Moves
In the move_data.txt file, there you can add moves in this format:  

move_name  
move_type  
physical or special  
base_damage  
accuracy  
priority  

move_name is the name of the move, move_type is the move's type, physical or special will be p if the move is physical, or s if it's   special, base_damage is the move's damage, accuracy is the move's accuracy, and priority is the moves priority. Example:  

aqua jet  
water  
p  
40  
100  
1  

## Add Moves To Current Pokemon

This one is really easy. If you see that a Pokemon can learn a move, but it isn't listed in the moves list in the file, then you can add it. Example:  

Before:  
\# Azumarill water fairy 100 50 80 60 80 50  
aqua jet  
play rough  
tackle  

After:  
\# Azumarill water fairy 100 50 80 60 80 50  
aqua jet  
play rough  
tackle  
knock off  
