
Binary:
The best way the use it is download the binary file.
Three kinds of binary are provided:
1 32bit linux(test on ubuntu 12.04)
2 64bit linux(test on ubuntu 12.04)
3 32bit windows(test on win8-64bit and xp-32bit)

You should use windows version as much as possible,because I will update it first.
other version will be updated if requested.
Source build:


Make sure you have install mingw,wxWidgets and boost library.

windows:
	type cmake -G "MinGW Makefiles" under windows
	then type mingw32-make
linux:
	type cmake 
	type make



This software is develop by limeng at Harbin Engeerning University China

any suggestion or bug report is welcome.Conncet:331060295@qq.com

How to use:
1 Use open command to open *.bam *.fa *.fasta *.gtf *.gtf.gz *.bed.gz *.bed *.vcf *.vcf.gz *.gff.gz *.gff.
  If you use file end with .gz,you must sort them and index them properly with samtools/tabix.See tabix for details,
  and please use -0 option to ensure a 0 based coordinate is used when indexing the .gz file.In the case that your 
  file is too large(mostly >800mb) you must use .gz file instead.Important:Make sure the bam file have the Corresponding bai file.
  ie. the bam must be sort and index first.
  
2 Double click the bam file name in the left tree control.Then select a chrosome in the toolbar.

3 When you want to get statistic information, just left click the bam filename in the tree control,and then click the Do statistic menu.

4 Use left arrow and right arrow to move right or left or use drag to move the viewer.You can use scroll + shift to move faster.
  Use up arrow and down arrow or scroll+ctrl to zoom in or out.

5 When you zoom in to largest view,you can see some detail information by hovering the mouse on the bam-read or on the top black track.

