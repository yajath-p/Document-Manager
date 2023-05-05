#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include "document.h"

/*Yajath Parashar, 118042596, yajath*/

int main(int argc, char *argv[]) {
  char *substring;
  const char *doc_name = "main_document";
  Document doc;
  FILE *stream;
  char lines[MAX_STR_SIZE + 1] = "", command[MAX_STR_SIZE + 1] = "";
  int i;
  
  /*Initiliazes document depending on number of commands*/
  if(argc > 2) {
    printf("Usage: user_interface\n");
    printf("Usage: user_interface <filename>\n");
    exit(EX_USAGE);
    
    /*One command therefore stream is standard input*/
  } else if(argc == 1) {
    stream = stdin;
   
    /*Two commands so stream is the specified input*/
  } else if(argc == 2) {
    stream = fopen(argv[1], "r");

    /*If stream cant be opened program is exited*/
    if (stream == NULL) {
      fprintf(stderr, "%s cannot be opened.\n", argv[1]);
      exit(EX_OSERR);
    }

  }

  
  init_document(&doc, doc_name);
  if(argc == 1) {
    printf("> ");
  }
  fgets(lines, MAX_STR_SIZE + 1, stream);
  
  /*Computations of commands in the input stream*/
  while (stream) {
   
   
    /*If not comment*/
    if(strstr(lines, "#") == NULL) {

      /*Handles add_paragraph_after command*/
      if (strstr(lines, "add_paragraph_after") != NULL) {
	int para_num;
	char extra[MAX_STR_SIZE + 1] = "";
	/*Checks for correct amount of inputs*/
	if (sscanf(lines, " %s %d %s", command, &para_num, extra) == 2) {

	  /*Checks if the number inputed is positive and even a number*/
	  if(para_num >= 0) {
	    if(add_paragraph_after(&doc, para_num) == FAILURE){
	      printf("add_paragraph_after failed\n");
	    }
	  } else {
	    printf("Invalid Command\n");
	  }
	} else {
	  printf("Invalid Command\n");
	}

	/*Handles add_line_after*/
      } else if (strstr(lines, "add_line_after") != NULL) {
	/*Find instance of * which indicates line*/
	substring = strstr(lines, "*");

	/*Checks if input has line*/
	if (substring == NULL) {
	  printf("Invalid Command\n");
	} else {
	  char command_string[MAX_STR_SIZE + 1] = "", sentence_string[MAX_STR_SIZE + 1] = "";
	  int para_num, line_num;

	  /*Adds all paramater information besides actual line into compare string*/
	  for(i = 0; lines[i] != '*'; i++) {
	    command_string[i] = lines[i];
	  }
	  
	  /*Copies everything after * into other_compare_string*/
	  strcpy(sentence_string, substring + 1);
	  
	  /*Gets information from compare_string to separate commands*/
	  if((sscanf(command_string, " %s %d %d", command, &para_num, &line_num) == 3)  && para_num > 0 && line_num >= 0) {

	    /*Runs add_line_after*/
	    if(add_line_after(&doc, para_num,  line_num, sentence_string) == FAILURE) {
	      printf("add_line_after failed");
	    }
	  } else {
	    printf("Invalid Command\n");
	  }
	}

	/*Handles print_document*/
      } else if (strstr(lines, "print_document") != NULL) {
	char extra[MAX_STR_SIZE + 1] = "";

	/*If there is any extra information print doc doesn't run*/
	if(sscanf(lines, " %s %s", command, extra) == 1) {
	  print_document(&doc);
	} else {
	  printf("Invalid Command\n");
	}
	
	/*Handles append_line*/
      } else if (strstr(lines, "append_line") != NULL) {

	/*Checks if line input as indicated by **/
	substring = strstr(lines, "*");

	/*If * exists line exists*/
	if (substring == NULL) {
	  printf("Invalid Command\n");
	} else {
	  char command_string[MAX_STR_SIZE + 1] = "", line_string[MAX_STR_SIZE + 1] = "";
	  int para_num;

	  /*Adds commands/parameters into command_string*/
	  for(i = 0; lines[i] != '*'; i++) {
	    command_string[i] = lines[i];
	  }
	  
	  /*Adds actual line into other_compare_string*/
	  strcpy(line_string, substring + 1);
	  
	  /*Checks if correct number of input and num param given is a positive number*/
	  if((sscanf(command_string, " %s %d", command, &para_num) == 2) && para_num > 0) {
	    if(append_line(&doc, para_num, line_string)
		== FAILURE) {
	      printf("append_line failed");
	    }
	  } else {
	    printf("Invalid Command\n");
	  }
	} 

	/*Handles remove_line*/
      } else if(strstr(lines, "remove_line") != NULL) {
	int para_num, line_num;
	char extra[MAX_STR_SIZE +1] = "";
	int scan_amount = sscanf(lines, "%s %d %d %s", command, &para_num, &line_num, extra); /*the number stored is amount of variables given data*/

	/*Checks for correct amount of inputs*/
	if(scan_amount != 3) {
	  printf("Invalid Command\n");
	}  else {
	  /*Checks if correct parameters*/
	  if(para_num <= 0 || line_num <= 0) {
	    printf("Invalid Command\n");
	  } else {
	    if(remove_line(&doc, para_num, line_num) == FAILURE) {
	      printf("remove_line failed");
	    }
	  }
	}

	/*Handles load_file*/
      } else if(strstr(lines, "load_file") != NULL) {
	char filename[MAX_STR_SIZE + 1] = "", extra[MAX_STR_SIZE + 1] = "";
	int scan_amount = sscanf(lines, "%s %s %s", command, filename, extra);

	/*Checks if correct input amount*/
	if(scan_amount != 2) {
	  printf("Invalid Command\n");
	} else {
	  /*Runs load_file if fail prints whats given below*/
	  if(load_file(&doc, filename) == FAILURE) {
	    printf("Invalid Command\n");
	  }
	} 

	/*Handles into replace_text*/
      } else if(strstr(lines, "replace_text") != NULL) {
	char target[MAX_STR_SIZE + 1] = "", replacement[MAX_STR_SIZE + 1] = "";
	int scan_amount = sscanf(lines, "%s %s %s", command, target, replacement);
	char *str1,*str2,*str3,*str4; /*Points to each quotation mark*/

	if(scan_amount < 2 || strlen(target) == 0) {
	  printf("Invalid Command\n");
	} else { 
	  char target_no_quote[MAX_STR_SIZE + 1] = "", replace_no_quote[MAX_STR_SIZE + 1] = "";
	 
	  /*Pointers to each quote*/
	  str1 = strstr(lines, "\""); 
	  str2 = strstr(str1 + 1, "\"");
	  str3 = strstr(str2 + 1, "\"");
	  str4 = strstr(str3 + 1, "\"");

	  if(str1 == NULL || str2 == NULL) {
	    printf("Invalid Command\n");
	  }

	  /*Copies target without the quotes into target_no_quote*/
	  strncpy(target_no_quote, str1 +1, str2 - str1);
	  target_no_quote[str2 - (str1 +1)] = '\0';

	  /*Copes replacement without the quotes into replace_no_quote*/
	  strncpy(replace_no_quote, str3 +1, str4 - str3);
	  replace_no_quote[str4 - (str3 +1)] = '\0';

	  /*Runs replace text*/
	  if(replace_text(&doc, target_no_quote, replace_no_quote) == FAILURE) {
	    printf("replace_text failed");
	  }
	} 
	
	/*Runs highlight_text*/
      } else if(strstr(lines, "highlight_text") != NULL) {
	char target[MAX_STR_SIZE] = "", target_no_quote[MAX_STR_SIZE] = "";

	/*Checks for valid amount of commands*/
	if(sscanf(lines, "%s %s", command, target) != 2) {
	  printf("Invalid Command\n");
	} else {
	  char *str1,*str2;
	  str1 = strstr(lines, "\"");
	  str2 = strstr(str1 + 1,"\"");

	  if(str1 == NULL || str2 == NULL) {
	    printf("Invalid Command\n");
	  } else {
	    /*Removes quotation marks from target*/
	    for(i=1;i<strlen(target)-1;i++) {
	      target_no_quote[i-1] = target[i];
	    }
	    target_no_quote[i-1] = '\0';
	    /*Runs highlight_text*/
	    highlight_text(&doc, target_no_quote);
	  }
	  
	}
	  
	  /*Handles remove_text*/
      } else if(strstr(lines, "remove_text") != NULL) {
	char target[MAX_STR_SIZE] = "", target_no_quotes[MAX_STR_SIZE] = "";

	/*Checks for valid amount of commands*/
	if(sscanf(lines, "%s %s", command, target) != 2) {
	  printf("Invalid Command\n");
	} else {
	  char *str1,*str2;
	  str1 = strstr(lines, "\"");
	  str2 = strstr(str1 + 1,"\"");

	  if(str1 == NULL || str2 == NULL) {
	    printf("Invalid Command\n");
	  } else {
	    /*Removes quotation marks from target*/
	    for(i = 1; i < strlen(target) - 1; i++) {
	      target_no_quotes[i-1] = target[i];
	    }
	    target_no_quotes[i-1] = '\0';
	    /*Runs remove_text*/
	    remove_text(&doc, target_no_quotes);
	  }
	}

	/*Handles save_document*/
      } else if(strstr(lines, "save_document") != NULL) {
	char filename[MAX_STR_SIZE + 1] = "", extra[MAX_STR_SIZE + 1] = "";
	int scan_amount = sscanf(lines, "%s %s %s", command, filename, extra);

	/*Checks for valid amount of commands*/
	if(scan_amount != 2) {
	  printf("Invalid Command\n");
	} else {
	  /*Runs save_document*/
	  if(save_document(&doc, filename) == FAILURE) {
	    printf("Invalid Command\n");
	  }
	}

	/*Handles reset_document*/
      } else if(strstr(lines, "reset_document") != NULL) {
	char extra[MAX_STR_SIZE + 1] = "";

	/*Checks for valid amount of commands*/
	if(sscanf(lines, "%s %s", command, extra) != 1) {
	  printf("Invalid Command\n");
	} else {
	  /*Resets document*/
	  reset_document(&doc);
	}
	
	/*Handles if input wants to stop*/
      } else if(strstr(lines, "exit") != NULL || strstr(lines, "quit") != NULL) {
	char extra[MAX_STR_SIZE + 1] = "";

	/*Checks for valid amount of commands if extra information says invalid command*/
	if (sscanf(lines, " %s %s", command, extra) == 1) {
	  exit(EXIT_SUCCESS);
	} else {
	  printf("Invalid Command\n");
	}

	/*Handles blank spaces and blank lines*/
      }  else if (strspn(lines, " ") == strlen(lines) - 1 || strcmp(lines, "") == 0 || *lines == '\n') {
       
	/*Can't handle any other unknown commands*/
      } else {
	printf("Invalid Command\n");

      }

    }
    
    if(argc == 1) {
      printf("> ");
    }

    /*Gets updated stream and lines arr*/
    if(fgets(lines, MAX_STR_SIZE + 1, stream)==NULL) {
      break;
    }
  }
  
  /*Closes stream and exits program*/
  fclose(stream);
  exit(EXIT_SUCCESS);
}
