# eyelight
EyeLight is a localtime based CLI+GUI(kinda) tool to set temperature of displays for X11 clients.


## Installation

### Dependecies
It requires sct to be installed.
Install it with
* ```sudo apt install sct``` for debian/ubuntu/mint
* ```sudo dnf install sct``` for fedora distros

### Application installation 
There happens to be no application installation yet. I plan on adding it very soon.
To run the file with all it's requirements, download the latest release, extract the zip archive.
1. cd into the extracted archive.
2. Set the bash script as executable with  -   ```sudo chmod +x ./mkdirfile.sh```
3. Execute the script with                 -   ```sudo ./mkdirfile.sh ```
4. Run the binary with                     -   ```./EyeLight.out ```
5. Change values of files at /etc/.EyeLight/ to edit temperature values.
* Accepted range of temperature is between 1000 - 10000
* Needs root/admin access to be edited
   
## Possible issues
1. No change in screen colour on starting program - Can be fixed by editing any of the files at /etc/.EyeLight/ once
2. Delayed change in colour [last cached values being executed] - My fault. Code is low quality and homemade hence such issues.
3. Tiresome procedure - It is what it is. Will fix when I can code better.
