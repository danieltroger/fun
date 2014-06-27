/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2013, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
#include <stdio.h>
#include <curl/curl.h>
#include <libgen.h>
#include <stdlib.h>

int prog(void *clientp, double dltotal,   double dlnow,   double ultotal,   double ulnow)
{
  printf("dltotal: %f, dlnow: %f\n",dltotal,dlnow);
  return 1;
}

int main(int argc, char *argv[])
{
  if(argv[1] == NULL)
  {
    printf("Usage: %s url [filename]\n",argv[0]);
    return -1;
  }
  FILE *outf;
  if(argv[2] == NULL)
  {
    outf=fopen(basename(argv[1]),"w+");
  }
  else
  {
    outf=fopen(argv[2],"w+");
  }
  if(!outf) {
    perror("Couldn't output file\n");
    return 1;
  }
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, outf) ;
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, prog);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
    {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    return 1;
    }
    fclose(outf);
    curl_easy_cleanup(curl);
  }
  return 0;
}
