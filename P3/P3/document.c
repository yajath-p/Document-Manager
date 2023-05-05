#include <stdio.h>
#include <string.h>
#include "document.h"

/*Yajath Parashar, 118042596, yajath*/

int load_file(Document *doc, const char *filename) {

  if(doc == NULL || filename == NULL) { 
    return FAILURE;
  } else {
    char buffer[MAX_STR_SIZE + 1] = "";
    FILE *stream = fopen(filename, "r");
    int values, para_num = 1;
    char detector, line[MAX_STR_SIZE + 1] = "";

    if(stream == NULL) {
      return FAILURE;
    }

    if(add_paragraph_after(doc, 0) == FAILURE) {
      return FAILURE;
    }

    while(fgets(buffer, MAX_STR_SIZE + 1, stream) != NULL) {
      values = sscanf(buffer, "%c%s", &detector, line);

      if(values < 2) {
	if(add_paragraph_after(doc, para_num) == FAILURE) {
	  return FAILURE;
	}
	para_num++;
      } else {
	if(append_line(doc, para_num, buffer) == FAILURE) {
	  return FAILURE;
	}
      }
      
      if(para_num > MAX_PARAGRAPHS) {
	return FAILURE;
      }
    }

    fclose(stream);
    return SUCCESS;
  } 
  
}

int save_document(Document *doc, const char *filename) {
  if(doc == NULL || filename == NULL) {
    return FAILURE;
  } else {
    FILE *stream = fopen(filename, "w"); 
    int i, z;

    if(stream == NULL) {
      return FAILURE;
    }
    for(i = 0; i < doc->number_of_paragraphs; i++) {
      for(z = 0; z < doc->paragraphs[i].number_of_lines; z++) {
	fputs(doc->paragraphs[i].lines[z], stream);
	if(z < doc->paragraphs[i].number_of_lines) {
	  fputs("\n", stream);
	}
      }
      if(i+1 < doc->number_of_paragraphs) {
	fputs("\n", stream);
      }
    }

    fclose(stream);
    return SUCCESS;
  }
}

/*Initializes document by setting new document name */
int init_document(Document *doc, const char *name) {
  if(doc == NULL || name == NULL || strlen(name) > MAX_STR_SIZE) {
    return FAILURE;
  }
  
  strcpy(doc->name, name);
  doc->number_of_paragraphs = 0;

  return SUCCESS;
}

/*Resets document*/
int reset_document(Document *doc) {
  if(doc==NULL) {
    return FAILURE;
  }

  doc->number_of_paragraphs = 0;

  return SUCCESS;
} 

/*Prints document*/
int print_document(Document *doc) {
  int i, z;
  
  if(doc==NULL) {
    return FAILURE;
  }

  printf("Document name: \"%s\"\n", doc->name);
  printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);
  /*Prints each paragraph and line.*/
  for(i = 0; i < doc->number_of_paragraphs; i++) {

    /*If number of lines is empty doesn't go through loop*/
    if(doc->paragraphs[i].number_of_lines > 0) {

      for(z = 0; z < doc->paragraphs[i].number_of_lines; z++) {
	printf("%s\n", doc->paragraphs[i].lines[z]);
      } 

      /*To add spaces between paragraphs*/
      if(i < doc->number_of_paragraphs - 1) {
	printf("\n");	
      }

    }

  }
  
  return SUCCESS;
}

/*Adds paragraph*/
int add_paragraph_after(Document *doc, int paragraph_number) {
  if(doc==NULL || doc->number_of_paragraphs>=MAX_PARAGRAPHS || paragraph_number >= MAX_PARAGRAPHS || paragraph_number > doc->number_of_paragraphs) { 
    return FAILURE;
  } else {
    int para_amount =  (doc->number_of_paragraphs);
    int i;

    /*If input is 0 new paragraph is at beginning of array*/
    if(paragraph_number == 0 && para_amount<1) {
      doc->paragraphs[0].number_of_lines = 0;
      
      /*If input is putting new paragraph at the end of array*/
    } else if(paragraph_number == para_amount) {
      doc->paragraphs[para_amount].number_of_lines = 0;
      
      /*Every other case*/ 
    } else {
      
      for(i = para_amount; i > paragraph_number - 1; i--) {
	doc->paragraphs[i + 1] = doc->paragraphs[i];
      } 
      
      doc->paragraphs[paragraph_number].number_of_lines = 0;
    }
    
    /*Increments number of paragraphs*/
    doc->number_of_paragraphs++;
    
    return SUCCESS;
  }
}

/*Adds new line into paragraph*/
int add_line_after(Document *doc, int paragraph_number, int line_number, const char *new_line) {
  int i, r; 
  char lines_copy[MAX_PARAGRAPH_LINES][MAX_STR_SIZE + 1]; /*Temporary array to hold new iteration of lines array*/
  int lines_num = doc->paragraphs[paragraph_number-1].number_of_lines; 
  
  if(doc==NULL || paragraph_number > MAX_PARAGRAPHS || doc->number_of_paragraphs == MAX_PARAGRAPH_LINES || line_number >= MAX_PARAGRAPH_LINES || new_line == NULL) { 
    return FAILURE; 
  } 
  
  /*Puts line in temporary array*/
  
  /*Zero case*/
  if(line_number == 0) { 
    /*Copies all elements besides first into temporary array*/
    for(i = 1; i < lines_num + 1; i++) { 
      strcpy(lines_copy[i], doc->paragraphs[paragraph_number-1].lines[i-1]); 
    } 
    
    /*Puts new line as first element*/
    strcpy(lines_copy[0], new_line); 
    
    /*Puts element into last element*/
  } else if(line_number == lines_num) { 
    /*Copies element directly into lines array at the end and increment num lines*/
    strcpy(doc->paragraphs[paragraph_number-1].lines[line_number], new_line); 
    doc->paragraphs[paragraph_number-1].number_of_lines++; 
    
    return SUCCESS; 
    
    /*If element is requested to go somewhere in the middle */
  } else { 
    /*Moves everything down 1 after new line spot*/
    for(i = 0; i < lines_num + 1; i++) { 
      if(i < line_number) { 
	strcpy(lines_copy[i], doc->paragraphs[paragraph_number-1].lines[i]); 
      } else if(i > line_number) {  
	strcpy(lines_copy[i], doc->paragraphs[paragraph_number-1].lines[i-1]); 
      } 
    } 
    
    /*Copies new line into temporary array*/
    strcpy(lines_copy[line_number], new_line); 
  }  
  
  /*Copies temporary array of lines into actual lines array*/
  for(r = 0; r < lines_num + 1; r++) { 
    strcpy(doc->paragraphs[paragraph_number-1].lines[r], lines_copy[r]); 
  }  
  
  doc->paragraphs[paragraph_number-1].number_of_lines++; 
  
  return SUCCESS; 
}

/*Gets number of lines in a paragraph*/
int get_number_lines_paragraph(Document *doc, int paragraph_number, int *number_of_lines) {
  if(doc==NULL || number_of_lines==NULL || paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  } 

  *number_of_lines = doc->paragraphs[paragraph_number-1].number_of_lines;

  return SUCCESS;
}

/*Adds a line into a paragraph*/
int append_line(Document *doc, int paragraph_number, const char *new_line) { 
  if(doc==NULL || paragraph_number > MAX_PARAGRAPHS || doc->number_of_paragraphs == MAX_PARAGRAPH_LINES || new_line == NULL) {
    return FAILURE;
  }
  
  /*Uses add_line_after function to add line as the last sentence in paragraph*/
  add_line_after(doc, paragraph_number, doc->paragraphs[paragraph_number - 1].number_of_lines, new_line);

  return SUCCESS;
}

/*Removes a line*/
int remove_line(Document *doc, int paragraph_number, int line_number) {
  int i;

  if(doc == NULL || paragraph_number > MAX_PARAGRAPHS || line_number > doc->paragraphs[paragraph_number-1].number_of_lines) {
    return FAILURE;
  }
  
  /*Starting at line that should be removed, adds next line into removed line*/
  for(i = line_number-1; i < doc->paragraphs[paragraph_number-1].number_of_lines; i++) {
    strcpy(doc->paragraphs[paragraph_number-1].lines[i], doc->paragraphs[paragraph_number-1].lines[i + 1]);
  }
  
  /*Decrements number of lines*/
  doc->paragraphs[paragraph_number-1].number_of_lines--;

  return SUCCESS;
}

/*Adds lines and paragraphs from data set*/
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines) {
  int i;

  if(doc == NULL || data == NULL || data_lines == 0) {
    return FAILURE;
  }
  /*Creates new paragraph*/
  add_paragraph_after(doc, doc->number_of_paragraphs); 

  /*Goes through data array*/
  for(i = 0; i < data_lines; i++) {
    /*If element is blank adds new paragraph*/
    if(strlen(data[i]) == 0) {
      add_paragraph_after(doc, doc->number_of_paragraphs);
    } else {
      /*Otherwise adds line after last sentence*/
      append_line(doc, doc->number_of_paragraphs, data[i]);
    }
  }

  return SUCCESS;
}

/*Replaces targetted text with replacement text*/
int replace_text(Document *doc, const char *target, const char *replacement) {
  int i, z, pos;
  char temporary[MAX_STR_SIZE + 1] = "", *str;
  
  if (doc == NULL || target == NULL || replacement == NULL) {
    return FAILURE;
  }
  /*Traverses doc*/
  for (i = 0; i < doc->number_of_paragraphs; i++) {
    for (z = 0; z < doc->paragraphs[i].number_of_lines; z++) {
      /*Goes to target*/
      str = strcpy(temporary, doc->paragraphs[i].lines[z]);
      str = strstr(str, target);
      /*Checks if it went to anything*/
      if (str != NULL) {
	pos = str - temporary;
      }
      /*Actual replacement*/
      while (str != NULL) {
	/*Puts replacement into target*/
	doc->paragraphs[i].lines[z][pos] = '\0';
	strcat(doc->paragraphs[i].lines[z], replacement);
	str += strlen(target);
	strcat(doc->paragraphs[i].lines[z], str);
	/*Goes to next target*/
	str = strstr(str, target);
	/*Checks if it exists or not*/
	if (!(str == NULL)) {
	  pos = strlen(doc->paragraphs[i].lines[z]) - strlen(str);
	}
      }
    }
  }
  return SUCCESS;
 
}

/*Highlights specified text*/
int highlight_text(Document *doc, const char *target) {
  char highlight[MAX_STR_SIZE + 1] = "";

  if(doc == NULL || target == NULL) {
    return FAILURE;
  }

  /*Creates what we want the highlighted text to be*/
  strcat(highlight, HIGHLIGHT_START_STR);
  strcat(highlight, target);
  strcat(highlight, HIGHLIGHT_END_STR);
  /*Replaces all target strings with what we want the 
   highlighted text to be by using replace_text function*/
  replace_text(doc, target, highlight); 
  
  return SUCCESS;
}

/*Removes specified text throughout document*/
int remove_text(Document *doc, const char *target) { 
  if(doc == NULL || target == NULL) {
    return FAILURE;
  }
  
  /*Sets targetted string to empty string*/
  replace_text(doc, target, "");
	
  return SUCCESS;
}
