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

double* raycast(V3 Rd, V3 Ro, int count, Object *array, double* background);
Pixel* render(int width, int height, Object camera, Object *array, int count);
double sphere_intersection(V3 Rd, V3 Ro, Object obj);
double plane_intersection(V3 Rd, V3 Ro, Object obj);

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
  Object camera_ptr;
  for (int i = 0; i < obj_count; i++){
    if (obj_array[i].kind == 1){
      camera_ptr = obj_array[i];
      break;
    }
  }

  pix_image = render(width, height, camera_ptr, obj_array, obj_count);
  //Writing P3 to file
  fprintf(output_file, "%s\n", "P3");

  //Writing width and height to file
  fprintf(output_file, "%s "  "%s\n", argv[1], argv[2]);

  //Writing max color value to file
  fprintf(output_file, "%s\n", "255");

  //Writing image data to file
  for (int i = 0; i < width * height; i++){
    fprintf(output_file, "%d\n", pix_image[i].r);
    fprintf(output_file, "%d\n", pix_image[i].g);
    fprintf(output_file, "%d\n", pix_image[i].b);
  }
  fclose(output_file);
  fclose(csv_file);
  return(0);
}

double* raycast (V3 Rd, V3 Ro, int count, Object *array, double* background){
  Object closest_obj;
  double* background_color = background; 
  double* color_array;
  double closest_t = INFINITY;
  double t;
  Object obj;
  
  for (int i = 0; i < count; i++){
    obj = array[i];
    if (obj.kind == 2){
      //sphere intersection
      t = sphere_intersection(Rd, Ro, obj);
      if (t < closest_t){
	closest_t = t;
	closest_obj = array[i];
      }
    }
    
    if (obj.kind == 3){
      //plane intersection
      t = plane_intersection(Rd, Ro, obj);
      if (t < closest_t){
	closest_t = t;
	closest_obj = array[i];
      }
    }
  }

  if (closest_t == INFINITY){
    return(background);
  }else{
    color_array = closest_obj.color;
  }
  return(color_array);
}

Pixel* render (int width, int height, Object camera, Object *array, int count){
  Pixel *pix_info;
  double *color;
  double background[3] = {0, 1, 0};
  double dx = (camera.width/width);
  double dy = (camera.height/height);
  int current_pix = 0;
  
  pix_info = (Pixel *)malloc(sizeof(Pixel) * width * height);
  double* Pij = malloc(sizeof(double) * 3);
  double* Rd = malloc(sizeof(double) * 3);
  double* Ro = malloc(sizeof(double) * 3);
  v3_assign(Ro, 0 , 0, -1);
  for (int i = 0; i < height; i++){
    double Py = 0 - (camera.height/2) + dy * (i + 0.5);
    for (int j = 0; j < width; j++){
      double Px = 0 - (camera.width/2) + dx * (j + 0.5);
     
      v3_assign(Pij,
		Px,
		Py,
		1);
      
      v3_sub(Rd, Pij, Ro);
      
      double mag = sqrt(pow(Rd[0], 2) + pow(Rd[1], 2) + pow(Rd[2], 2));

     
      v3_assign(Rd,
		Rd[0]/mag,
		Rd[1]/mag,
		Rd[2]/mag);

      
      color = raycast(Rd, Ro, count, array, background);
      
      pix_info[current_pix].r = (color[0] * 255);
      pix_info[current_pix].g = (color[1] * 255);
      pix_info[current_pix].b = (color[2] * 255);
      current_pix++;
    }
  }
  return(pix_info);
}

double sphere_intersection (V3 Rd, V3 Ro, Object obj){
  //Rd values for x y z
  double Rdx = Rd[0];
  double Rdy = Rd[1];
  double Rdz = Rd[2];

  //R0 values
  double Rox = Ro[0];
  double Roy = Ro[1];
  double Roz = Ro[2];
  
  //Sphere values for x y z and r
  double Cx = obj.position[0];
  double Cy = obj.position[1];
  double Cz = obj.position[2];
  double r = obj.radius;

  //Calculating A B C for quadratic equation
  double A = pow(Rdx, 2) + pow(Rdy, 2) + pow(Rdz, 2);
  double B = 2 * (Rdx * (Rox - Cx) + Rdy * (Roy - Cy) + Rdz * (Roz - Cz));
  double C = ((Rox - Cx) * (Rox - Cx)) + ((Roy - Cy) * (Roy - Cy)) + ((Roz - Cz) * (Roz - Cz)) - (r * r);

  //Initializing t0 t1
  double t0;
  double t1;

  //Calculating discriminant
  double dis;
  dis = (B * B) - (4 * C); 
  if (dis < 0){
    return(INFINITY);
  }
  
  //Calculating for t0 and t1
  t0 = (-B - sqrt(dis))/2;
  if (t0 > 0){
    return(t0);
  }else{
    t1 = (-B + sqrt(dis))/2;
    return(t1);
  }
}

double plane_intersection (V3 Rd, V3 Ro, Object obj){
  //value for dot product result
  double Vd;
  
  //Normal values
  double A = obj.normal[0];
  double B = obj.normal[1];
  double C = obj.normal[2];

  //Allocating space for vector
  double* Pn = malloc(sizeof(double) * 3);
  
  
  //Making normal a unit normal
  double mag = sqrt(pow(A, 2) + pow(B, 2) + pow(C, 2));
  v3_assign(Pn, A/mag, B/mag, C/mag);

  
  //Dot product of Pn and Rd to see if there is an intersection
  Vd = v3_dot(Pn, Rd);
  if (Vd == 0){
    return(INFINITY);
  }
  if (Vd > 0){
    double* temp = malloc(sizeof(double) * 3);
    v3_sub(temp, Ro, obj.position);
    double dot_temp1 = v3_dot(Pn, temp);
    double t = -(dot_temp1/Vd);
    if ( t < 0){
      return(INFINITY);
    }else{
      return(t);
    }
  }
}
