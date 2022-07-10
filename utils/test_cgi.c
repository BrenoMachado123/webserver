#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

int main ()
{
   char *env_value;
   char *save_env;
   time_t current_time;

   printf("Content-type: text/html\n\n");
   printf("<html>\n");
   printf("<head>\n");
   printf("<title>Improved Simple Sample</title>\n");
   printf("</head> <body>\n");
   printf("<h1>This is a Heading</h1>\n");
   printf("<p> <pre>\n");
   printf("C Program Version\n");
   current_time = time(NULL);                       /* get current time */
   printf("It is now %s\n", ctime(&current_time));
   save_env = getenv("QUERY_STRING");   /* get environment variable */
   if (save_env == NULL)
   {
      printf("Major failure Wrong Envs; please notify the webmaster\n");
      exit(2);
   }
   env_value = malloc(strlen(save_env) + 1);
   if (env_value == NULL)
   {
      printf("Major failure; please notify the webmaster\n");
      exit (2);
   }
   strcpy(env_value, save_env);   /* save environment variable */
   printf("The query is %s\n", env_value);         /* and print it */
   printf("You are signed onto %s\n", getenv("REMOTE_HOST"));
   printf("Your IP Address is %s\n", getenv("REMOTE_ADDR"));
   printf("</pre> </body> </html>\n");
   int cont_len = atoi(getenv("CONTENT_LENGTH"));
   char * buff = calloc(sizeof(char), cont_len + 1);
   int valread = read(STDIN_FILENO, buff, cont_len);
   if (valread < 0)
   {
      printf("Major failure couldn't read; please notify the webmaster\n");
      exit(2);
   }
   printf("<h2>Content: %s\n</h2>", buff);
   free(buff);
   fflush(stdout);              /* force physical write */
   exit (0);
}

