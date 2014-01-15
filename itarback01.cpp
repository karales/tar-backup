#include <stdio.h>
#include <dirent.h>		// header for directory listing

#include <iostream>
#include <fstream>

#include <algorithm>
#include <vector>
#include <string>

using namespace std;

int main (int c, char *v[]) {
  
  	int i = 0;
  	int n = 0;
  
    	//
    	// VAR 001 ORIGINAL DIRECTORY SCAN
    	//
    	int len;
    	struct dirent *pDirent;
    	DIR *pDir;
    
    	//
    	// VAR 002 OUTPUT FILE
    	//
    	ofstream myfile;
    
    	//
    	// VAR 003 FILE LIST VECTOR
   	//
    	string img_filename;
    	vector<string> filename_vector;
    	//
    	// Ram Drive Commands
    	//
    	string ram_command_create;
    	string ram_command_close;
    	string ram_command_delete;
    	//
    	//Copy Files Commands
    	//
    	string ram_command_dest_create;
    	string ram_command_dest_close;
    
    	//
    	//Tar Commands
    	//tar -cv -b 1024 -f
    	//
    	string tar_command_create;
    	string tar_command_create_go;
    
    	//
    	//copy tar files commands
    	//
    	string tar_copy;
    	tar_copy="sudo mv ";
    
    	// sudo tar -cv -b 1024 -f 
   	// tar_copy="sudo tar -cv -b 1024 -f ";
    
    
    
    	string outimagefilefolder;
    	string outimagetarfilename;
    	string outimagetarfilenametar;
    
    	string outpath;
    
    	string ramcache;
    	ramcache = "/ramcache/";
    	
	string ramcache_command;
     
    	string space;
    	space = " ";
     
    	string tarend;
    	tarend = ".tar";
    
    	outimagefilefolder = "/imagein/";   // Placee Input Path Here
    
    	outpath = "/imageout/";    // Placee Output Path Here
    	//outpath = "/imageout";    // Placee Output Path Here
    
    	ram_command_create = "sudo mkdir -p /ramcache; sudo mkfs -q /dev/ram0 60000; sudo mount /dev/ram0 /ramcache; sudo df -H | grep ramcache";
    
    	ram_command_close = "sudo umount /dev/ram0 /ramcache; sudo df -H | grep ramcache";
    
    	ram_command_delete = "sudo rm -r /ramcache;";
    
    	ram_command_dest_create = "sudo mkdir /imageout";
    	ram_command_dest_close = "sudo rm -r /imageout";    // check that file is deleted before copying.
    
    	tar_command_create = "sudo tar -cv -b 1024 -f ";
    
    
    	/////////////////////////////////////////////////////////
    	// 001 ORIGINAL DIRECTORY SCAN
    	/////////////////////////////////////////////////////////

    	pDir = opendir (outimagefilefolder.c_str());
    
    	if (pDir == NULL)
	{
        	cout << endl << "Cannot open directory: " << v[1] << endl;
        	return 1;
    	}

    	while ((pDirent = readdir(pDir)) != NULL)
	{  
      
		img_filename = pDirent->d_name;
	
		if ((img_filename != ".directory")&&(img_filename != ".")&&(img_filename != ".."))
		{
	  		filename_vector.push_back(img_filename);
		}
    	}
    	
	closedir (pDir);
    
    	sort(filename_vector.begin(), filename_vector.end());

    	for (i = 0; i < filename_vector.size(); i++)
	{
		cout << endl << "filename: " << filename_vector[i] << endl;
	}
    
    	i = 0;
    	n = 1;
    
    	//delete previous image output folder
    	system(ram_command_dest_close.c_str());
    
    
    	//create ram disk  
    	system(ram_command_create.c_str());
    
    	//create image output folder
    	system(ram_command_dest_create.c_str());
    

    	for (i = 0; i < filename_vector.size(); i++, n++) 
    	{
      		cout << i << "++" << n << endl;
       
      		outimagetarfilename = filename_vector[i];
      		outimagetarfilenametar = filename_vector[i];
      		outimagetarfilenametar.replace(outimagetarfilenametar.find(".file"), 5, tarend);
      
      		cout << "OUTIMAGETARFILENAME::"<<outimagetarfilename << endl;
      		cout << "OUTIMAGETARFILENAMETAR::"<<outimagetarfilenametar << endl;

      		tar_command_create_go = tar_command_create + ramcache + outimagetarfilenametar + space + outimagefilefolder + outimagetarfilename;
      		//tar_command_create_go = tar_command_create + ramcache + space + outimagefilefolder + outimagetarfilename;
      
      		cout <<tar_command_create_go << endl;
      
      		//tar file in memory space
     		system(tar_command_create_go.c_str());
      
      		//copy file to folder location (TAPE)
      		ramcache_command = tar_copy+ramcache+outimagetarfilenametar+space+outpath;
      		cout << ramcache_command << endl;
      		system(ramcache_command.c_str());
    	}
    
    	//close ram disk
    	system(ram_command_close.c_str());
    
    	//delete folder
    	system(ram_command_delete.c_str());

    	return 0;
}
