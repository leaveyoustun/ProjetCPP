# C++ PROJECT – Itinerary Calculator

Author: **STAN Liviu‑Mihai**  


---

## Development environment
The entire codebase was written and tested in **Visual Studio Code**.

---

## How to compile, build and run

### 1 – Compile

```bash
g++ -std=c++11 -Wall -Wextra -Werror -pedantic -pedantic-errors -O3 -o Itinerary_Calculator Itinerary_Calculator.cpp
```
### 2 – Make the binary executable

```bash
chmod a+x Itinerary_Calculator
```

### 3 – Run

```bash
./Itinerary_Calculator data/name_of_stations_file.csv \
                       data/name_of_connections_file.csv
```

> **Note** If your CSV files live inside a `data/` folder (as in this repository), include
> the folder name in the path like shown above.

---

## Output examples:

The errors I show here will arrive for both datasets, the small ones and the big ones.

### 1.  Compilation and execution

The code should be compiled and executed like in the README.txt file.

If we execute the code with too many or not enough arguments, we will get these errors:

```text
Command: ./Itinerary_Calculator data/stations.csv data/connections.csv true
Usage: ./Itinerary_Calculator <stations file> <connections file>
Compilation error!
Too many arguments
```

```text
Command : ./Itinerary_Calculator data/stations.csv
Usage: ./Itinerary_Calculator <stations file> <connections file>
Compilation error!
Not enough arguments
```

If we mix the datasets:

```text
Command ./Itinerary_Calculator data/stations.csv data/c.csv or
        ./Itinerary_Calculator data/s.csv data/connections.csv
```

Or if we put different names:

```text
Command: ./Itinerary_Calculator data/example.csv data/connect.csv
Compilation error!
ERROR: The names of the data sets are not correct!
```

---

### 2.  Small sets

When running the first set you will first be asked if you want to see the demonstration of the
`read_stations` and `read_connections` methods. You need to press **y** (or **Y**) or **n** (or **N**).
If you press another character:

```text
Hello!

Would you like to see the demonstration of read_stations and read_connections? (y/n) a
Compilation error!
Boolean error – please enter 'y' or 'n'.
```

*This will be the case for every y/n question that may arrive in the code.*

If you press **y**, you will get this screen, with the demonstration:

```
Stations:

Station: A (line 1)
Station: B (line 2)
Station: C (line 3)
Station: D (line 4)
Station: E (line 5)
Station: F (line 6)
Station: G (line 7)
Station: H (line 8)
Station: I (line 9)
Station: J (line 10)

Connections:

From Station: I (line 9) to Station: J (line 10) in 84 seconds
From Station: I (line 9) to Station: F (line 6) in 250 seconds
From Station: J (line 10) to Station: E (line 5) in 493 seconds
From Station: J (line 10) to Station: I (line 9) in 84 seconds
From Station: J (line 10) to Station: H (line 8) in 167 seconds
From Station: H (line 8) to Station: D (line 4) in 183 seconds
From Station: H (line 8) to Station: C (line 3) in 91 seconds
From Station: H (line 8) to Station: J (line 10) in 167 seconds
From Station: C (line 3) to Station: A (line 1) in 217 seconds
From Station: C (line 3) to Station: H (line 8) in 103 seconds
From Station: C (line 3) to Station: G (line 7) in 186 seconds
From Station: G (line 7) to Station: C (line 3) in 186 seconds
From Station: F (line 6) to Station: B (line 2) in 80 seconds
From Station: F (line 6) to Station: I (line 9) in 278 seconds
From Station: B (line 2) to Station: A (line 1) in 85 seconds
From Station: B (line 2) to Station: F (line 6) in 80 seconds
From Station: E (line 5) to Station: A (line 1) in 173 seconds
From Station: E (line 5) to Station: J (line 10) in 502 seconds
From Station: D (line 4) to Station: H (line 8) in 183 seconds
From Station: A (line 1) to Station: E (line 5) in 173 seconds
From Station: A (line 1) to Station: B (line 2) in 85 seconds
From Station: A (line 1) to Station: C (line 3) in 217 seconds

Press ENTER to go to the itinerary calculation!
```

Here you can press **ENTER** to get to the itinerary calculator. (If you press **n** you will get directly to it)

Example with **Station A** and **Station J** (they will need to be typed one by one):

```text
Hello!

What itinerary would you like to know?

Station of departure: A
Station of arrival: J
```

```
Best way from A (line Ligne de la station A) to J (line Ligne de la station J) is :

Take line Ligne de la station A   Ligne de la A (Terminus undefined -> Terminus undefined) – Aller
From A to C (217 seconds)
Take line Ligne de la station C   Ligne de la C (Terminus undefined -> Terminus undefined) – Aller
From C to H (103 seconds)
Take line Ligne de la station H   Ligne de la H (Terminus undefined -> Terminus undefined) – Aller
From H to J (167 seconds)

After 487 seconds, you have reached your destination!
```

The code is **not case sensitive** so you can write for example `a` and `j` and it will work.

If you write the station wrongly, for example, **Ax** instead of **A**:

```text
Hello!

What itinerary would you like to know?

Station of departure: Ax
Station of arrival: J
```

```text
Ax station was not found, did you mean Station: A (line 1)? (y/n)
```

If you type **'n'**:

```text
Stations error!
Station not found
(base) liviu@192-168-0-141 ProjetCPP %
```

If you type **'y'**, the code will give you the itinerary from A to the arrival station you typed.

---

### 3.  Big sets

First, you will get this screen:

```text
Hello!

What itinerary would you like to know?

Station of departure:
```

Same as before, you can write a station of departure and a station of arrival.  
Example with **Jussieu** and **Gare du Nord**

```text
Hello!

What itinerary would you like to know?

Station of departure: Jussieu
Station of arrival: Gare du Nord
```

```
Best way from Jussieu (line 10) to Gare du Nord (line 5) is :

Walk to Jussieu, line 7 (14 seconds)
Take line 7 (LA COURNEUVE – 8 MAI 1945 <-> VILLEJUIF‑L. ARAGON / MAIRIE D'IVRY) – Aller
From Jussieu to Châtelet (386 seconds)
Walk to Châtelet, line 4 (248 seconds)
Take line 4 (PORTE DE CLIGNANCOURT <-> MAIRIE DE MONTROUGE) – Aller
From Châtelet to Gare de l'Est (Verdun) (622 seconds)
Walk to Gare de l'Est (Verdun), line 5 (72 seconds)
Take line 5 (BOBIGNY – PABLO PICASSO <-> PLACE D'ITALIE) – Aller
From Gare de l'Est (Verdun) to Gare du Nord (156 seconds)

After 1498 seconds, you have reached your destination!
```

Again, same as the small set, the code is **not case sensitive** so you can write for example
“jussieu” and “gare du nord”.

If the station you typed is wrongly typed, the code will ask you here as well if you meant
something else. Example for “jusieu” and “gara de nords”.  
Here you can press **'y'** if it’s the good station, or **'n'** and you will get the **Station error** from before:

```text
Hello!

What itinerary would you like to know?

Station of departure: jusieu
Station of arrival: gara de nords
```

```text
jusieu station was not found, did you mean Station: Jussieu (line 2233)? (y/n)
```

```text
Stations error!
Station not found
```

Same for the second station:

```text
gara de nords station was not found, did you mean Station: Gare du Nord (line 2213)? (y/n)
```

And then we get the result:

```
Best way from Jussieu (line 10) to Gare du Nord (line 5) is :

Walk to Jussieu, line 7 (14 seconds)
Take line 7 (LA COURNEUVE – 8 MAI 1945 <-> VILLEJUIF‑L. ARAGON / MAIRIE D'IVRY) – Aller
From Jussieu to Châtelet (386 seconds)
Walk to Châtelet, line 4 (248 seconds)
Take line 4 (PORTE DE CLIGNANCOURT <-> MAIRIE DE MONTROUGE) – Aller
From Châtelet to Gare de l'Est (Verdun) (622 seconds)
Walk to Gare de l'Est (Verdun), line 5 (72 seconds)
Take line 5 (BOBIGNY – PABLO PICASSO <-> PLACE D'ITALIE) – Aller
From Gare de l'Est (Verdun) to Gare du Nord (156 seconds)

After 1498 seconds, you have reached your destination!
```

---

### 4.  Miscellaneous (Errors for stations)

If for any of the stations, you press **ENTER** without typing any character, you will get:

```text
Stations error!
At least one station was not entered!
```

If you type the **same station** for arrival and departure:

```text
Stations error!
You entered the same station for departure and arrival!
```
