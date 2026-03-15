Discription:-MP3 Tag Reader and Editor is a command-line utility that allows users to view and modify metadata embedded in MP3 files.
Sample:
//VIEW    

./a.out -v song.mp3 

----------------------------------------------------------------------------------------------------------------------------
MP3 Tag Reader and Editor for ID3
----------------------------------------------------------------------------------------------------------------------------
     Title : Sunny Sunny - Yo Yo Honey Singh - [SongsPk.CC]
    Artist : Yo Yo Honey Singh - [SongsPk.CC]
     Album : Yaariyan
      year : 2013
   content : Bollywood Music - [SongsPk.CC]
  composer : eng


//EDIT

./a.out -e -y 2020 song.mp3

-------------------------------select Edit option----------------------------------------
-------------------------------------------select Year change option---------------------------------
Year : 2020
-----------------------------------Year changed succesfully-----------------------------------------------------------
 


 //HELP

 ./a.out --help song.mp3


 usage:
-t      Modifies the Title tag
-a      Modifies an Artist tag
-A      Modifies Album tag
-y      Modifies year tag
-c      Modifies the Content type
-m      Modifies the Composer tag
