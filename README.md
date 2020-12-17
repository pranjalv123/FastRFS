# FastRFS
Fast Robinson Foulds Supertrees

To build FastRFS you will need the [Bazel build system](https://bazel.build). From the FastRFS folder, run

    bazel build //src:FastRFS
    
to build FastRFS. Your executable will be located at `blaze-bin/src/FastRFS`. 
   
To run FastRFS, you must have a directory containing the Astral distribution in the folder with FastRFS. That is the folder named `Astral` contained inside the ASTRAL distribution. For example, you could copy that folder to the same location as the FastRFS executable, or you could copy FastRFS to the same location as the ASTRAL executable.

Then, run 

    FastRFS -i <input files> -o <output file> 
   
To run FastRFS*, calculate an extra tree with MRL (or extra trees with whatever method), and run
    
    FastRFS -i <input files> -o <output file> -e <extra tree files>
   
By default, FastRFS uses [SIESTA](https://link.springer.com/chapter/10.1007/978-3-319-67979-2_13) to generate strict, greedy, and majority consensus trees of all optimal trees.
If for some reason this is a problem, you can disable these with --nogreedy, --nostrict, and --nomajority options.

The other output files are <output file>.count, which contains the number of optimal trees, <output file>.score, which contains the optimal score (RF distances to input trees),  and <output file>.single, which contains a single optimal tree.

Other options you can use include:
    
    --noastral to not use ASTRAL to compute bipartition sests
    --clades <newick file> to give an explicit list of clades that should be used for the analysis 
         (by contrast, -e adds the files as a command line argument to ASTRAL; --clades takes exactly the clades from the trees in the provided file
