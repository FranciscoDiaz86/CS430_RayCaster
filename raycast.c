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

double* raycast(V3 Rd, int count, Object *array, double* background);
Pixel* render(int width, int height, Object camera, Object *array, int count);
double sphere_intersection(V3 Rd, Object obj);

int main (int argc, char *argv[]){
  int obj_count = 0;
  Object obj_array[128];
  int width = atoi(argv[1]);
  int height = atoi(argv[2]);
  Pixel *pix_image;
  FILE *csv_file;
  FILE *output_file;
  
  csv_file = fopen(argv[3], "r");
  output_file = fopen(argv[4], "w");
  
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
      pix_image = render(width, height, obj_array[i], obj_array, obj_count);
      break;
    }
  }
  //Writing P3 to file
  fprintf(output_file, "%s\n", "P3");

  //Writing width and height to file
  fprintf(output_file, "%s "  "%s\n", argv[1], argv[2]);

  //Writing max color value to file
  fprintf(output_file, "%s\n", "255");

  //Writing image data to file
  for (int i = 0; i < width * height; i++){
    fprintf(output_file, " %d %d %d ", pix_image[i].r, pix_image[i].g, pix_image[i].b);
  }
  fclose(output_file);
  fclose(csv_file);
  return(0);
}

double* raycast (V3 Rd, int count, Object *array, double* background){
  Object closest_obj;
  double* background_color = background;
  double closest_t = INFINITY;
  double t;
  Object obj;
  
  for (int i = 0; i < count; i++){
    obj = array[i];
    double t;
    if (obj.kind == 2){
      t = sphere_intersection(Rd, obj);
      if (t == -1){
	return (background);
      }
      
      if (t < closest_t){
	printf("Im here\n");
	closest_t = t;
	closest_obj = array[i];
      }
    }
    if (obj.kind == 3){
      //plane intersection
     
    }
  }
  
  double* color_array = closest_obj.color; 
  return(color_array);
}

Pixel* render (int width, int height, Object camera, Object *array, int count){
  Pixel *pix_info;
  double *color;
  double background[3] = {1, 1, 1};
  double dx = (camera.width/width);
  double dy = (camera.height/height); 
  double top_lefty = -(camera.width/dx) - dy;
  double top_leftx;
  int current_pix = 0;
  
  pix_info = (Pixel *)malloc(sizeof(Pixel) * width * height);
  double* Pij = malloc(sizeof(double) * 3);
  double* Rd = malloc(sizeof(double) * 3);
  
  for (int i = 0; i < height; i++){
    top_lefty += dy;
    double Py = top_lefty + (i * dy) + (.5 * dy); 
    top_leftx = (camera.width/width);
    for (int j = 0; j < width; j++){
      double Px = top_leftx + (j * dx) + (.5 * dx);
      v3_assign(Pij,
		Px,
		Py,
		-10);
      
      double mag = sqrt(pow(Pij[0], 2) + pow(Pij[1], 2) + pow(Pij[2], 2));

     
      v3_assign(Rd,
		Pij[0]/mag,
		Pij[1]/mag,
		Pij[2]/mag);

      top_leftx -= dx;
      
      color = raycast(Rd, count, array, background);
      
      pix_info[current_pix].r = (color[0] * 255);
      pix_info[current_pix].g = (color[1] * 255);
      pix_info[current_pix].b = (color[2] * 255);
      current_pix++;
    }
  }
  return(pix_info);
}

double sphere_intersection (V3 Rd, Object obj){
  //Rd values for x y z
  double Rdx = Rd[0];
  double Rdy = Rd[1];
  double Rdz = Rd[2];

  //Sphere values for x y z and r
  double Cx = obj.position[0];
  double Cy = obj.position[1];
  double Cz = obj.position[2];
  double r = obj.radius;

  //Calculating A B C for quadratic equation
  double A = pow(Rdx, 2) + pow(Rdy, 2) + pow(Rdz, 2);
  double B = 2 * ((Rdx * (0 - Cx)) + (Rdy * (0 - Cy)) + (Rdz * (0 - Cz)));
  double C = pow(-Cx, 2) + pow(-Cy, 2) + pow(-Cz, 2) - pow(r, 2);

  printf("The value of A is: %f\n", A);
  printf("The value of B is: %f\n", B);
  printf("The value of C is: %f\n", C);
  //Initializing t0 t1
  double t0;
  double t1;

  //Calculating (B^2-4*C)
  double dis_part;
  dis_part = (pow(B, 2) - 4 * C);

  //Calculating discriminat for t0
  t0 = (-B - pow(dis_part, .5))/2;
  if (t0 < 0 ){
    return(INFINITY);
  }

  if (t0 >= 0){
    return(t0);
  }
  else{
    t1 = (-B - pow(dis_part, .5))/2;
    return(t1);
  }
}

