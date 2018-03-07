# FastRFS
Fast Robinson Foulds Supertrees

To build FastRFS you will first need [phylokit](https://github.com/pranjalv123/phylokit) and [phylonaut](https://github.com/pranjalv123/phylonaut),

   mkdir build
   cd build
   cmake ../src
   make
   
   
To run FastRFS, you must have a directory containing the Astral distribution in the folder with FastRFS. Then, run 

   FastRFS -i <input files> -o <output file> 
   
To run FastRFS*, calculate an extra tree with MRL (or extra trees with whatever method), and run
   FastRFS -i <input files> -o <output file> -e <extra tree files>
   
By default, FastRFS uses [SIESTA](https://link.springer.com/chapter/10.1007/978-3-319-67979-2_13) to generate strict, greedy, and majority consensus trees of all optimal trees.
If for some reason this is a problem, you can disable these with --nogreedy, --nostrict, and --nomajority options.

The other output files are <output file>.count, which contains the number of optimal trees, <output file>.score, which contains the optimal score (RF distances to input trees),  and <output file>.single, which contains a single optimal tree.
