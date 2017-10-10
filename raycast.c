#include <stdlib.h>
#include <math.h>
#include "csv_parser.h"
#include "3dmath.h"

typedef struct Pixel{
  unsigned char r, g, b;
}Pixel;

typedef struct PPMImage{
  int width, height;
  Pixel *pix_info;
}PPM_Image;

int raycast(V3 Rd, int count, Object *array);
Pixel* render(int width, int height, Object camera, Object *array, int count);

int main (int argc, char *argv[]){
  int obj_count = 0;
  Object obj_array[128];
  int width = atoi(argv[1]);
  int height = atoi(argv[2]);
  FILE *csv_file;
  csv_file = fopen(argv[3], "r");
  
  while (1){
    if (feof(csv_file)){
	break;
    }
    
    Object obj;
    Object *obj_ptr;
    obj_ptr = &obj;

    parse_type(csv_file, obj_ptr);
    parse_field(csv_file, obj_ptr);

    obj_array[obj_count] = obj;

    obj_count++; 
  }

  for (int i = 0; i < obj_count; i++){
    if (obj_array[i].kind == 1){
      render(width, height, obj_array[i], obj_array, obj_count);
      break;
    }
  }
  return(0);
}

int raycast(V3 Rd, int count, Object *array){
  Object closest_obj;
  double closest_t = INFINITY;
  Object obj;
  
  for (int i = 0; i < count; i++){
    obj = array[i];
    double t;
    if (obj.kind == 2){
      //sphere intersection
      
    }
    if (obj.kind == 3){
      //plane intersection
     
    }
  }
  return(0);
}

Pixel* render(int width, int height, Object camera, Object *array, int count){
  PPM_Image image;
  double color;
  double grid_intervalx = (camera.width/width);
  double grid_intervaly = (camera.height/height); 
  double top_leftx = -(grid_intervalx * (width/2)) - grid_intervalx;
  double top_lefty;
  
  image.width = width;
  image.height = height;
  image.pix_info = (Pixel *)malloc(sizeof(Pixel) * image.width * image.height);
  double* Pij = malloc(sizeof(double) * 3);
  double* Rd = malloc(sizeof(double) * 3);
  
  for (int i = 0; i < width; i++){
    top_leftx += grid_intervalx;
    top_lefty = grid_intervaly * (height/2);
    for (int j = 0; j < height; j++){
      
      v3_assign(Pij,
		top_leftx + (i + 1) * grid_intervalx + (.5) * grid_intervalx,
		top_lefty + (j + 1) * grid_intervaly + (.5) * grid_intervaly,
		-1);
      
      double mag = sqrt(pow(Pij[0], 2) + pow(Pij[1], 2) + pow(Pij[2], 2));

     
      v3_assign(Rd,
		Pij[0]/mag,
		Pij[1]/mag,
		Pij[2]/mag);

      top_lefty -= grid_intervaly;
      raycast(Rd, count, array);
    }
  }
  return(image.pix_info);
}
