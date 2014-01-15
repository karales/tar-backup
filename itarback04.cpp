#include <stdio.h>
#include <dirent.h>		// header for directory listing

#include <iostream>
#include <fstream>

#include <algorithm>
#include <vector>
#include <string>



#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>








using namespace std;

int main (int c, char *v[]) {


	char block[100000];
    	int in, out;
    	int nread;
 	// default block size
    	int blen = 1024;
	int betterLen = 1 << 10;

	assert(blen == betterLen);

    	int temp;
	
  
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
    	//string tar_copy;
    	//tar_copy="sudo mv ";
    	// sudo tar -cv -b 1024 -f 
   	// tar_copy="sudo tar -cv -b 1024 -f ";
    
    	string ioinputfile;
	string iooutputfile;
    
    	string inpath;
    	string filenameoriginal;
    	string filenametar;
    
    	string outpath;

	string oneoutfile;
	oneoutfile = "out.tar";
    
    	string ramcache;
    	ramcache = "/ramcache/";
    	
	//string ramcache_command;
     	string ramfiledelete;
	ramfiledelete = "sudo rm -r /ramcache/*";

    	string space;
    	space = " ";
     
    	string tarend;
    	tarend = ".tar";
    
    	inpath = "/imagein/";   // Placee Input Path Here
    
    	outpath = "/imageout/";    // Placee Output Path Here
    	//outpath = "/imageout";    // Placee Output Path Here
    
    	ram_command_create = "sudo mkdir -p /ramcache; sudo mkfs -q /dev/ram0 60000; sudo mount /dev/ram0 /ramcache; sudo df -H | grep ramcache";
    
    	ram_command_close = "sudo umount /dev/ram0 /ramcache; sudo df -H | grep ramcache";
    
    	ram_command_delete = "sudo rm -r /ramcache;";
    
    	ram_command_dest_create = "sudo mkdir /imageout";
    	ram_command_dest_close = "sudo rm -r /imageout";    // check that file is deleted before copying.
    
    	tar_command_create = "sudo tar -cv -b 1024 -f ";



	//printf("Using a block size of %d\n", len);
    	cout << endl <<"Using a block size of " << blen << endl;
    
    	/////////////////////////////////////////////////////////
    	// 001 ORIGINAL DIRECTORY SCAN
    	/////////////////////////////////////////////////////////

    	pDir = opendir (inpath.c_str());
    
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


	const char* iofilename = "data.tar"; 

	iooutputfile = outpath + oneoutfile;
	cout << endl << "RAMCACHE OUTPUT TAR FILE: " << iooutputfile << endl;



	// Open out for writing Only.  If the file doesn't exist,
  	// the we will create the file with User Read/Write permission
 
    	//out = open(iofilename, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
    	out = open("test2.tar", O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
    	if (in < 0 || out < 0)
    	{
        	cout << endl << "ERROR OPENING FILE" << endl;
		return -1;
    	}
    

    	for (i = 0; i < filename_vector.size(); i++, n++) 
    	{
      		cout << i << "++" << n << endl;
       
      		filenameoriginal = filename_vector[i];
      		filenametar = filenameoriginal;
      		filenametar.replace(filenametar.find(".file"), 5, tarend);
      
      		cout << "ORIGINAL FILE NAME: "<<filenameoriginal << endl;
      		cout << "TAR FILE NAME: "<<filenametar << endl;

      		tar_command_create_go = tar_command_create + ramcache + filenametar + space + inpath + filenameoriginal;
      
      		cout << endl << tar_command_create_go << endl;

      		ioinputfile = ramcache + filenametar;
		cout << endl << "RAMCACHE INPUT TAR FILE: " << ioinputfile << endl;
      		
		

      		//tar file in memory space
     		system(tar_command_create_go.c_str());

		//OPEN INPUT FILE AND COPY FILE TO TAPE
		// Open in for Reading Only
    		in = open(ioinputfile.c_str(), O_RDONLY);

		// read in blocks of length len and write out the same blocks to 
   		// the output file 
    		while((nread = read(in,block,blen)) > 0)
        	write(out,block,nread);


      
      		//copy file to folder location (TAPE)
      		//ramcache_command = tar_copy+ramcache+filenametar+space+outpath;
      		//cout << endl << ramfiledelete << endl;
      		//system(ramfiledelete.c_str());
		
      		system(ramfiledelete.c_str());

		

		//CLOSE INPUT FILE
		close(in);
		
    	}

    	//CLOSE OUTPUT FILE (TAPE DEVICE)
	close(out);

    	//close ram disk
    	system(ram_command_close.c_str());
    
    	//delete folder
    	system(ram_command_delete.c_str());

    	return 0;
}
