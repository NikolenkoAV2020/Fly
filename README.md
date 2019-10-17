# Fly
Spacecraft motion model forcing &amp; solving applied flight dynamics problems.

Now I realy wanna rewright my project for more correct multi thread computing and also for implementation 
script-injections written with LUA. Such LUA injections will allow further
to create program moduls which will represent the applied ballistic applications themselves.
For example, in this way possible to write program solve problem of spacecraft flight dynamic.
LUA injection may be builded and executed without compile of Fly-project kernel, as well as
may be changed by software customer on stage of software using.

The second feature that is especially relevant is that it is necessary to rewrite the code 
taking into account possible cross-compilation for both Windows and Linux. It's mean that 
need use POSIX.

Walter Artyomenko, october 2019.
