#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 20
#define MAX_ROLL_LEN 20
#define TRUE 1
#define FALSE 0

/**
 * templr.c
 * A program to generate personalized messages for students
 * based on the template provided and the student details
 * provided in a csv file in the format:
 * name,rollno,marks
 * The program reads the template file and replaces the tags
 * with the corresponding values from the csv file and saves
 * the message to a file with the name rollno.txt
 *
 * The grade is calculated based on the marks as follows:
 * 90 - 100 : S
 * 80 - 89  : A
 * 70 - 79  : B
 * 60 - 69  : C
 * 50 - 59  : D
 * 40 - 49  : E
 * 0  - 39  : U
 *
 * Usage: ./templr templatefile csvfile
 */

// structure to store student details
typedef struct student {
  char name[MAX_NAME_LEN];
  char roll[MAX_ROLL_LEN];
  uint32_t marks;
  char grade;
} student;

void perr(char *msg);
char *trimWhitespace(char *str);
student *read_csv(char *filename);
char compute_grade(student s);
void write_file(student s, char *template);

int num_students = 0;

/**
 * function to print error message and exit
 * @param msg
 */
void perr(char *msg) {
  perror(msg);
  exit(1);
}

/**
 * function to trim leading and trailing whitespace from a string
 * @param str
 * @return char*
 */
char *trimWhitespace(char *str) {
  if (str == NULL)
    return NULL;
  char *start = str;
  while (isspace(*start))
    start++;
  char *end = str + strlen(str) - 1;
  while (end > start && isspace(*end))
    end--;
  *(end + 1) = '\0';
  return start;
}
/**
 * function to read from a given filename and return an array of students
 * @param filename
 * @return student*
 */
student *read_csv(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL)
    perr("Error opening file");
  // get the number of lines in the file
  int length = 0;
  char c, prev = '\n';
  while ((c = fgetc(fp)) != EOF) {
    if (c == '\n')
      length++;
    prev = c;
  }
  // if last line does not end with a newline
  if (prev != '\n')
    length++;
  fseek(fp, 0, SEEK_SET);

  num_students = length;

  // allocate memory for the array
  student *students = (student *)malloc(length * sizeof(student));
  if (students == NULL)
    perr("Error allocating memory");

  // read the file line by line
  char line[100];
  int i = 0;
  while (fgets(line, 100, fp) != NULL) {
    char DELIM[] = ",";
    // tokenize the line and store the values in the array
    char *token = strtok(line, DELIM);
    if (token == NULL)
      perr("Error parsing file");
    strcpy(students[i].name, token);
    token = strtok(NULL, DELIM);
    if (token == NULL)
      perr("Error parsing file");
    strcpy(students[i].roll, token);
    token = strtok(NULL, DELIM);
    if (token == NULL)
      perr("Error parsing file");
    students[i].marks = atoi(token);

    // calculate the grade
    students[i].grade = compute_grade(students[i]);
    i++;
  }
  fclose(fp);
  return students;
}

/**
 * function to compute the grade of a student
 * @param s
 * @return char
 */
char compute_grade(student s) {
  if (s.marks > 100)
    perr("Invalid marks");
  if (s.marks >= 90)
    return 'S';
  if (s.marks >= 80)
    return 'A';
  if (s.marks >= 70)
    return 'B';
  if (s.marks >= 60)
    return 'C';
  if (s.marks >= 50)
    return 'D';
  if (s.marks >= 40)
    return 'E';
  return 'U';
}

/**
 * function to write the student details to a file. Creates file with filename =
 * student.roll .txt
 * @param s
 * @param template
 */
void write_file(student s, char *template) {
  char filename[MAX_ROLL_LEN + 5];
  strcpy(filename, s.roll);
  strcat(filename, ".txt");
  FILE *fp = fopen(filename, "w");
  if (fp == NULL)
    perr("Error opening file for writing");
  // replace the template with the student details. {name} -> student.name
  char *p = template;
  int markup = FALSE;
  char varname[20];
  memset(varname, 0, sizeof(varname));
  while (*p != '\0') {
    if (*p == '{') {
      markup = TRUE;
      p++;
      continue;
    }
    if (*p == '}') {
      // trim the variable name
      char *trimmed = trimWhitespace(varname);
      if (strcmp(trimmed, "name") == 0)
        fprintf(fp, "%s", s.name);
      else if (strcmp(trimmed, "rollno") == 0)
        fprintf(fp, "%s", s.roll);
      else if (strcmp(trimmed, "marks") == 0)
        fprintf(fp, "%u", s.marks);
      else if (strcmp(trimmed, "grade") == 0)
        fputc(s.grade, fp);
      // should invalid tags be retained? or removed? or throw an error?
      // else fprintf(fp, "{%s}", varname); // retained
      // else printf("Invalid tag: {%s}\n", varname); // removed
      else
        perror("Invalid tag"); // error
      markup = FALSE;
      memset(varname, 0, sizeof(varname));
      p++;
      continue;
    }
    if (markup) {
      // append the character to the variable name
      int len = strlen(varname);
      varname[len] = *p;
      varname[len + 1] = '\0';
    } else
      fputc(*p, fp);
    p++;
  }
  fclose(fp);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: %s templatefile csvfile\n", argv[0]);
    exit(1);
  }
  // read the template file
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL)
    perr("Error opening template file");

  // get the length of the file
  fseek(fp, 0, SEEK_END);
  long length = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char *template = (char *)malloc((length + 1) * sizeof(char));
  if (template == NULL) {
    fclose(fp);
    perror("Error allocating memory");
  }
  size_t read = fread(template, sizeof(char), length, fp);
  if (read != (size_t)length) {
    fclose(fp);
    free(template);
    perror("Error reading template file");
  }
  fclose(fp);
  template[length] = '\0';

  // read the csv file
  student *students = read_csv(argv[2]);

  // save each student's message to disk
  for (int i = 0; i < num_students; i++) {
    write_file(students[i], template);
  }
}
