# Advent-of-Code-2023
https://adventofcode.com/2023 done entirely in C.
Code compiled using Microsoft Visual Studio 2022.\
Note that input.txt is the same for both parts of a problem.
"Table of contents":\
\
**Day 1 - Trebuchet (calibration value searching).** Cool substring matching in searchForNums().\
**Day 2 - Cube Conundrum (cubes and colors).** Not very interesting.
**Day 3 - Gear Ratios (text parsing torture).** Part 2 had some weird diagonal parsing and replacement of chars. Took ages due to edge cases.\
**Day 4 - Scratchcards (winning numbers).** Both parts worked out pretty elegantly. Nothing special, just nice/efficient code.\
**Day 5 - If You Give a Seed a Fertilizer (maps).** Very large numbers and ranges. In part 2, findNextDifferentSeed() allowed to get the correct answer and simultaneously avoid processing *billions* of inputs, which would have been computationally unfeasible. Also took quite a lot of debugging.\
**Day 6 - Wait For It (boat races, quadratic roots).** The naive solution would be to loop and "brute force" whether or not a certain amount of buttonTime would work to get the required distance, but I realized the equation for distance could be represented as a quadratic equation. I then used the quadratic formula, along with some ceil() and floor() functions, to count the number of integers between the two roots. Part 2 likely would have been computationally unfeasible without this approach as well.\
**Day 7 - Camel Cards (poker)**. Overall I felt like this was coded very well, without any ugly hacks. I used a lot of functions I wrote in first-year CS classes, like insertion sort, functions to validate 5 or 4 of a kind (when coding Yahtzee for homework), etc. Very readable, but still clever.
