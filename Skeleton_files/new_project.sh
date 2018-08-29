#!/bin/bash
###############################################################################
#
#   new_project.sh
#   Creates new project in hacks repository. Creates folder structure and
#   copies default Makefile and README.md files.
#
#   by JuR
#
###############################################################################

# Check if input arguments are correct
if [ ! $1 ] || [ ! $2 ] || [ ! $3 ] || [ $# -gt 3 ]
then
  exit 1                # ERR CODE 1 -> invalid arguments
fi

# Read arguments
hackName=$1
hacksDir=$2
skeletonDir=$3

# Check if hacksDir exists
if [ ! -d "$hacksDir" ]; then
  exit 2                # ERR CODE 2 -> can't open HacksDir
fi

# Create a new folder
mkdir "$hacksDir/$hackName"
if [ $? -ne 0 ]         # Check if successfull
then
    exit 3              # ERR CODE 3 -> can't create folder
fi

# Create subfolders and empty source files
mkdir "$hacksDir/$hackName/builds"
mkdir "$hacksDir/$hackName/src"
touch "$hacksDir/$hackName/src/$hackName.asm"

# Copy default README.md and Makefile
cp "$skeletonDir/README.md" "$hacksDir/$hackName"
cp "$skeletonDir/Makefile" "$hacksDir/$hackName"

# Edit $FILENAME in Makefile
sed -i "/^FILENAME=/c\FILENAME=${hackName}" "$hacksDir/$hackName/Makefile"

exit 0