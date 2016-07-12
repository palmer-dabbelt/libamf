/* Copyright 2016 Palmer Dabbelt <palmer@dabbelt.com> */

#ifndef AMF_H
#define AMF_H

static __inline__ int isspace(int c)
{
  if (c == ' ')  return 1;
  if (c == '\t') return 1;
  if (c == '\n') return 1;
  if (c == '\r') return 1;
  return 0;
}

const char *advance_until_slash(const char *path)
{
  while ((*path != '\0') && (*path != '/'))
    path++;
  if (*path == '/')
    path++;

  return path;
}

const char *advance_until_entered(const char *config_string)
{
  while ((*config_string != '\0') && (*config_string != '{'))
    config_string++;

  if (*config_string == '{')
    config_string++;

  return config_string;
}

const char *advance_until_over(const char *config_string)
{
  int open;
  config_string = advance_until_entered(config_string);
  open = 1;
  while ((*config_string != '\0') && (open > 0)) {
    if (*config_string == '{') open++;
    if (*config_string == '}') open--;
    config_string++;
  }
  if (*config_string == ';')
    config_string++;

  return config_string;
}

int compare_until_slash(const char *config_string, const char *path)
{
  while (isspace(*config_string)) config_string++;
  while ((*path != '\0') && (*path != '/') && (*config_string != '\0')) {
    if (*path != *config_string)
      return 0;
    path++;
    config_string++;
  }

  if (!isspace(*config_string))
    return 0;

  return 1;
}


const char *find_in_config_string(const char *config_string, const char *path)
{
  while (*path != '\0' && *config_string != '\0') {
    if (compare_until_slash(config_string, path)) {
      path = advance_until_slash(path);
      if (*path != '\0')
        config_string = advance_until_entered(config_string);
    } else {
      config_string = advance_until_over(config_string);
    }
  }

  /* Remove any whitespace from the config string, then strip off the key. */
  while (*config_string != '\0' &&  isspace(*config_string)) config_string++;
  while (*config_string != '\0' && !isspace(*config_string)) config_string++;
  while (*config_string != '\0' &&  isspace(*config_string)) config_string++;

  return config_string;
}

#endif
