## Student Info
Name: Francisco Diaz
Email: fd86@nau.edu
Class: CS 430

## Usage
This program takes in 4 command line variables which are: The wdith, height, CSV file, and the name or your output ppm file. The width and height that you put in are the values for the resolution. The real world width and height will be in the CSV with the camera object. The image will be rendered and stored in a PPM file.

###Example command line input
./raycast 100 100 test.csv output.ppm

## Known Issues
There is an issue that I am having and it trying to render and image where the shpere is in front of the plane. I am not sure if it is a problem with the intersection test or if I am not placing the images in the right locations. 