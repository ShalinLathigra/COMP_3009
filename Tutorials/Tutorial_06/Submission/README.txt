This tutorial we learned about setting up vertex/index buffers to allow vertices to be reused across triangles. This mean that any triangles which
share a side can share the vertices which make up that side rather than storing duplicates. I knew about the process prior to this tutorial, but the 
opportunity to dig in a bit to the OpenGL guts of the code was interesting.

I ran into an issue with binding the colour attributes initially, I did not find that part of the tutorial very clear. However after working on it for 
a bit I was able to get it no problem.