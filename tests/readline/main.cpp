#include <cstdlib>
#include <iostream>
#include <readline/history.h>
#include <readline/readline.h>

// CAMBIATO: Usiamo char* invece di const char* per evitare il warning/errore di
// qualificazione
char *commands[] = {(char *)"hello", (char *)"world", (char *)"idk",
                    (char *)"bye",   (char *)"ciao",  NULL};

char *comandi_generator(const char *text, int state) {
  static int list_index, len;
  char *name;

  if (!state) {
    list_index = 0;
    len = strlen(text);
  }

  while ((name = commands[list_index++])) {
    if (strncmp(name, text, len) == 0) {
      return strdup(name);
    }
  }

  return NULL;
}

char **mia_cli_completion(const char *text, int start, int end) {
  rl_attempted_completion_over = 1;
  return rl_completion_matches(text, comandi_generator);
}

int main(void) {
  rl_attempted_completion_function = mia_cli_completion;

  char *input;
  for (int i = 0; i < 3; i++) {
    input = readline("Write something: ");

    if (!input) {
      break;
    }

    if (*input) {
      std::cout << "input = " << input << '\n';
      add_history(input);
    }

    free(input);
  }

  return 0;
}
