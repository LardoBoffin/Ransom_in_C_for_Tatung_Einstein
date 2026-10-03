# Ransom in C for the Tatung Einstein
This is an implementation of the short adventure game Ransom (an example from GAC) written about 10 years ago in C on a BBC Master using Twin as an editor and Norcroft C as a compiler. 

I have since started porting it to run under the Tatung Einstein using Z88DK. 

This has not been as simple as I had hoped, mostly due to the differences between DFS and CPM and the C compilers. 

Two major changes to support this have been:
1) Not relying on file size to determine the number of records in a file (due to CPM padding the files to the nearest 128 bytes).
2) Not using char to hold numerical bytes - in Norcroft C these were treated as unsigned by default but in Z88DK these are treated as signed which caused major issues when relying on these as number from 0 to 255 (higher numbers are treated as negatives)

I have added code in this version for the oil to run out after a number of turns so don't forget to turn it off if you are in the daylight as there are nasty things lurking on the dark!


If you want to try this in MAME download RANSOM.mfi.

If you want to try this on real hardware using a gotek download RANSOM.dsk


Plenty left to do in this version!
