This tutorial taught me about the specifics behind setting up shaders to run. In previous courses we were handed similar code without
particular attention paid to why each step was important or what it accomplished, it was just important to have all of the steps present.
The further context is interesting and could prove useful when debugging or setting up shaders in the future.

The only problem I faced was with finding the world_matrix error and that is simply because I missed it while scanning the first few times.


**NOTE: I realize my implementation of shifting colours is overcomplicated and uses if statements which is typically a taboo when doing
shaders. That being said, I figured that this particular application wouldn't suffer unduly from adding them and I wanted to see what the
easeInOutElastic function would look like in practice (function found here: https://easings.net/)