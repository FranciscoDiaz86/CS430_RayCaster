#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct{
  int kind;
  double position[3], color[3], normal[3];
  double width, height, radius;
}Object;

int parse_type(FILE *fh, Object *obj);
int parse_field(FILE *fh, Object *obj);
int parse_width(FILE *fh, Object *obj);
int parse_height(FILE *fh, Object *obj);
int parse_color(FILE *fh, Object *obj);
int parse_position(FILE *fh, Object *obj);
int parse_normal(FILE *fh, Object *obj);
int parse_radius(FILE *fh, Object *obj);

int main(int argc, char *argv[]){
  
  Object test_obj;
  Object *obj_ptr;
  obj_ptr = &test_obj;
  
  FILE *input_file = fopen(argv[1], "r");
  parse_type(input_file, obj_ptr);
  parse_field(input_file, obj_ptr);

  printf("%d\n", obj_ptr -> kind);
  
  printf("%.2f\n", obj_ptr -> color[0]);
  printf("%.2f\n", obj_ptr -> color[1]);
  printf("%.2f\n", obj_ptr -> color[2]);

  printf("\n");
  
  printf("%.2f\n", obj_ptr -> position[0]);
  printf("%.2f\n", obj_ptr -> position[1]);
  printf("%.2f\n", obj_ptr -> position[2]);

  printf("\n");

  printf("%.2f\n", obj_ptr -> normal[0]);
  printf("%.2f\n", obj_ptr -> normal[1]);
  printf("%.2f\n", obj_ptr -> normal[2]);
  
  fclose(input_file);
  return(0);
}

int parse_type(FILE *fh, Object *obj){
  int chr;
  char buff[255];
  int buff_pos = 0;
  int file_ptr;
  
  while(1){
    chr = fgetc(fh);
    if(chr == ','){
      break;
    }
    buff[buff_pos] = chr;
    buff[buff_pos + 1] = '\0';
    buff_pos++;
  }
  
  if((strcmp(buff, "camera")) == 0){
    obj -> kind = 1;
    
  }

  if((strcmp(buff, "sphere")) == 0){
    obj -> kind = 2;
  }

  if((strcmp(buff, "plane")) == 0){
    obj -> kind = 3;
  }
  return(0);
}

int parse_field(FILE *fh, Object *obj){
  int chr;
  char buff[255];
  int buff_pos = 0;

  chr = fgetc(fh);
  while(chr != EOF){
    if(chr == '\n'){
      break;
    }
    
    if(chr == ' '){
      chr = fgetc(fh);
    }
    
    if(chr != ' ' && chr != ':'){
      buff[buff_pos] = chr;
      buff[buff_pos + 1] = '\0';
      buff_pos++;
      chr = fgetc(fh);
    }
    
    if(chr == ':'){
      if((strcmp(buff, "width")) == 0){
	parse_width(fh, obj);
	chr = fgetc(fh);
	buff_pos = 0;
      }
      
      if((strcmp(buff, "height")) == 0){
	parse_height(fh, obj);
	chr = fgetc(fh);
	buff_pos = 0;
      }
      
      if((strcmp(buff, "color")) == 0){
	parse_color(fh, obj);
	chr = fgetc(fh);
	buff_pos = 0;
      }
      
      if((strcmp(buff, "position")) == 0){
	parse_position(fh, obj);
	chr = fgetc(fh);
	buff_pos = 0;
      }

      if((strcmp(buff, "radius")) == 0){
	parse_radius(fh, obj);
	chr = fgetc(fh);
	buff_pos = 0;
      }

      if((strcmp(buff, "normal")) == 0){
	parse_normal(fh, obj);
	chr = fgetc(fh);
	buff_pos = 0;
      }
    }
  }
  return(0);
}

int parse_width(FILE *fh, Object *obj){
  double obj_width;
  char buff[255];
  int buff_pos = 0;
  int chr;

  chr = fgetc(fh);
  while(chr != EOF){
    if(chr == ',' || chr == '\n'){
      break;
    }
    
    if(chr == ' '){
      chr = fgetc(fh);
    }

    if(chr != ' ' && chr != ','){
      buff[buff_pos] = chr;
      buff[buff_pos + 1] = '\0';
      buff_pos++;
      chr = fgetc(fh);
    }
  }
  obj_width = atof(buff);
  obj -> width = obj_width;
  return(0);
}

int parse_height(FILE *fh, Object *obj){
  double obj_height;
  char buff[255];
  int buff_pos = 0;
  int chr;

  chr = fgetc(fh);
  while(chr != EOF){
    if(chr == ',' || chr == '\n'){
      break;
    }
    
    if(chr == ' '){
      chr = fgetc(fh);
    }

    if(chr != ' ' && chr != ','){
      buff[buff_pos] = chr;
      buff[buff_pos + 1] = '\0';
      buff_pos++;
      chr = fgetc(fh);
    }
  }
  obj_height = atof(buff);
  obj -> height = obj_height;
  return(0);
}

int parse_color(FILE *fh, Object *obj){
  double obj_color;
  char buff[255];
  int buff_pos = 0;
  int clr_pos = 0;
  int chr;

  chr = fgetc(fh);
  while(chr != EOF){
    if(chr == ',' || chr == '\n'){
      break;
    }

    if(chr == ' '){
      chr = fgetc(fh);
    }

    if(chr == '['){
      chr = fgetc(fh);
      while(1){
	if(chr == ']'){
	  obj_color = atof(buff);
	  obj -> color[clr_pos] = obj_color;
	  chr = fgetc(fh);
	  break;
	}
	if(chr == ' '){
	  chr = fgetc(fh);
	}

	if(chr == ','){
	  obj_color = atof(buff);
	  obj -> color[clr_pos] = obj_color;
	  clr_pos++;
	  buff_pos = 0;
	  chr = fgetc(fh);
	}

	if(chr != ' ' && chr != ','){
	  buff[buff_pos] = chr;
	  buff[buff_pos + 1] = '\0';
	  buff_pos++;
	  chr = fgetc(fh);
	}
      }
      break;
    }
  }
  return(0);
}

int parse_position(FILE *fh, Object *obj){
  double obj_position;
  char buff[255];
  int buff_pos = 0;
  int pos_ptr = 0;
  int chr;

  chr = fgetc(fh);
  while(chr != EOF){
    if(chr == ',' || chr == '\n'){
      break;
    }

    if(chr == ' '){
      chr = fgetc(fh);
    }

    if(chr == '['){
      chr = fgetc(fh);
      while(1){
	if(chr == ']'){
	  obj_position = atof(buff);
	  obj -> position[pos_ptr] = obj_position;
	  chr = fgetc(fh);
	  break;
	}
	if(chr == ' '){
	  chr = fgetc(fh);
	}

	if(chr == ','){
	  obj_position = atof(buff);
	  obj -> position[pos_ptr] = obj_position;
	  pos_ptr++;
	  buff_pos = 0;
	  chr = fgetc(fh);
	}

	if(chr != ' ' && chr != ','){
	  buff[buff_pos] = chr;
	  buff[buff_pos + 1] = '\0';
	  buff_pos++;
	  chr = fgetc(fh);
	}
      }
      break;
    }
  }
  return(0);
}

int parse_normal(FILE *fh, Object *obj){
  double obj_normal;
  char buff[255];
  int buff_pos = 0;
  int norm_ptr = 0;
  int chr;

  chr = fgetc(fh);
  while(chr != EOF){
    if(chr == ',' || chr == '\n'){
      break;
    }

    if(chr == ' '){
      chr = fgetc(fh);
    }

    if(chr == '['){
      chr = fgetc(fh);
      while(1){
	if(chr == ']'){
	  obj_normal = atof(buff);
	  obj -> normal[norm_ptr] = obj_normal;
	  chr = fgetc(fh);
	  break;
	}
	if(chr == ' '){
	  chr = fgetc(fh);
	}

	if(chr == ','){
	  obj_normal = atof(buff);
	  obj -> normal[norm_ptr] = obj_normal;
	  norm_ptr++;
	  buff_pos = 0;
	  chr = fgetc(fh);
	}

	if(chr != ' ' && chr != ','){
	  buff[buff_pos] = chr;
	  buff[buff_pos + 1] = '\0';
	  buff_pos++;
	  chr = fgetc(fh);
	}
      }
      break;
    }
  }
  return(0);
}

int parse_radius(FILE *fh, Object *obj){
  double obj_radius;
  char buff[255];
  int buff_pos = 0;
  int chr;

  chr = fgetc(fh);
  while(chr != EOF){
    if(chr == ',' || chr == '\n'){
      break;
    }
    
    if(chr == ' '){
      chr = fgetc(fh);
    }

    if(chr != ' ' && chr != ','){
      buff[buff_pos] = chr;
      buff[buff_pos + 1] = '\0';
      buff_pos++;
      chr = fgetc(fh);
    }
  }
  obj_radius = atof(buff);
  obj -> radius = obj_radius;
  return(0);
}
