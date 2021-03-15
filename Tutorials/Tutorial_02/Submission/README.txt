Shalin Lathigra, Tutorial 02

This tutorial was primarily review for me, basic manipulation of primitives in OpenGL was covered pretty extensively through 2501 and 3501, however I found the last section on
Hermite curves very interesting. Specifically I learned a lot about the mechanics behind creating a Hermite curve, although we did cover the topic in lecture as well, I have been
finding the various technical difficulties somewhat distracting. Reading through the code for myself was very helpful.


I briefly ran into an issue when I decided to use matrices rather than vectors to apply translations and add the ability to scale shapes, it seemed like no matter what I did 
the translation would always be applied before scaling. Turns out I added the vertex position to the matrix BEFORE scaling, and then applied whatever additional offset I wanted
which broke ISROT. i realized that after finishing the tutorial without scaling so I did not go back and fix it, but I know how I should have done it.