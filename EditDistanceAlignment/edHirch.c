#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define MIN(a,b) ((a) < (b) ? a : b)
#define M(i, j, N, M) ((i) * (M) + (j))

typedef int myindex;
typedef char mychar;

mychar *strrev(mychar *s, int slen) {
   mychar *res = calloc((slen + 1), sizeof *res);
   int i;
   for (i=0; i<slen; i++) {
     res[i] = s[slen - i - 1];
   }
   return res;
}

mychar *strsub(mychar *s, int start, int end) {
    mychar* sub = calloc(end - start + 1, sizeof *sub);
    memcpy(sub, s, end - start);
    return sub;
}

void print_arr(int *m, int N, int M, mychar *a, mychar *b)
{
  int i, j;
  for(i = -1; i<N; i++)
  {
    for(j = -1; j<M; j++)
    {
      if(i==-1) // print the first row of letters
      {
        if(j <= 0)
          printf("%3c",' ');
        else
          printf("%3c",b[j-1]);
      }
      else if(j==-1) // print the first column of letters
      {
        if(i <= 0)
          printf("%3c",' ');
        else
          printf("%3c", a[i-1]);
      }
      else{ // print the matrix
        printf("%3d", m[M(i, j, N, M)]);
      }
    }
    printf("\n");
  }
}

void fill_cost_table(int *m, int N, int M, mychar *a, mychar *b)
{
  int i,j;
  for (i = 0; i < N; i++)
      m[M(i, 0, N, M)] = i;

  for (j = 0; j < M; j++)
      m[M(0, j, N, M)] = j;

#ifndef CODEJUDGE
  printf("\nFirst row and column\n");
  print_arr(m, N, M, a, b);
#endif

  for (i = 1; i < N; i++)
  {
      for (j = 1; j < M; j++)
      {
          m[M(i, j, N, M)] = MIN(
            m[M(i-1, j, N, M)] + 1, MIN(
            m[M(i, j-1, N, M)] + 1,
            m[M(i-1, j-1, N, M)] + ((a[i-1] == b[j-1]) ? 0 : 1))
          );
      }
  }
}

char *levenshtein_distance(mychar *a, mychar *b)
{
  //swap a and b : vi kan også bare bytte om på parameter rækkefølgen - but for now this is it
  mychar *swap = b;
  b = a;
  a = swap;

  int alen = strlen(a);
  int blen = strlen(b);
  const int N = alen + 1;
  const int M = blen + 1;
#ifndef CODEJUDGE
  printf("\nalen: %d", alen);
  printf("\nblen: %d", blen);
#endif

  char *out = calloc(alen + blen, sizeof *out);

  if (strcmp(a, b) == 0) {
       memset(out, '|', alen);
       return out;
  }

  if (alen == 0) {
       memset(out, 'b', blen);
       return out;
  }
  if (blen == 0) {
       memset(out, 'a', alen);
       return out;
  }

  int *m = (int*) calloc (N * M, sizeof *m);
  //int *m = malloc((alen + 1) * (blen + 1) * sizeof *m);

#ifndef CODEJUDGE
  printf("\nEmpty matrix\n");
  print_arr(m, N, M, a, b);
#endif

  fill_cost_table(m, N, M, a, b);

#ifndef CODEJUDGE
  printf("\nMatrix filled out\n");
  print_arr(m, N, M, a, b);
#endif

  // backtrace
  int i = alen, j = blen;
  myindex resultindex = 0;
  while (i > 0 && j > 0) {
    int current = m[M(i, j, N, M)],
        up      = m[M(i-1, j, N, M)],
        diag    = m[M(i-1, j-1, N, M)],
        left    = m[M(i, j-1, N, M)];
    if (up + 1 == current) {
      out[resultindex++] = 'b';
      i--;
    } else if (diag + 1 == current || (diag == current && a[i-1] == b[j-1])) {
      out[resultindex++] = '|';
      i--;
      j--;
    } else if (left + 1 == current) {
      out[resultindex++] = 'a';
      j--;
    } else {
      printf("You done goofed!\n");
      exit(45);
    }
  }

  if (j!=0) {
      // a is empty, so fill rest of length with 'b'.
      memset(out + resultindex, 'a', j);
      resultindex += j;
      j = 0;
  } else if (i!=0) {
      // see above.
      memset(out + resultindex, 'b', i);
      resultindex += i;
      i = 0;
  }

  return strrev(out, resultindex);
}

int *nw_score(mychar *x, mychar *y){
    int xlen = strlen(x), ylen = strlen(y), i, j, N = xlen+1, M = ylen+1;
    int *v0 = calloc(ylen + 1, sizeof *v0);
    int *v1 = calloc(ylen + 1, sizeof *v1);

#ifndef CODEJUDGE
    printf("x: %s\n", x);
    printf("y: %s\n", y);
#endif

    for(j=1;j<M;j++) v0[j] = v0[j-1] + 1;

#ifndef CODEJUDGE
        for(j=0; j<M; j++) {
            printf("%3d", v0[j]);
        }
        printf("\n");
#endif

    for(i=1;i<N;i++) {
        v1[0] = v0[0] + 1;
        for(j=1;j<M;j++) {
            int sub = v0[j-1] + (x[i-1] == y[j-1] ? 0 : 1),
                del = v0[j] + 1,
                ins = v1[j-1] + 1;
            v1[j] = MIN(sub, MIN(del, ins));
        }
        int *swap = v1;
        v1 = v0;
        v0 = swap;
#ifndef CODEJUDGE
        for(j=0; j<M; j++) {
            printf("%3d", v0[j]);
        }
        printf("\n");
#endif
    } 

    free(v1);

#ifndef CODEJUDGE
    printf("\n\n");
#endif

    return v0;
}


char *hirchbergs_align(mychar *x, mychar *y)
{
    int xlen = strlen(x);
    int ylen = strlen(y);
    // const int N = xlen + 1;
    // const int M = ylen + 1;
    
    if (strcmp(x, y) == 0) {
       mychar *out = calloc(xlen + 1, sizeof *out);
       memset(out, '|', xlen);
       return out;
    }

    if (xlen == 0) {
        mychar *out = calloc(ylen + 1, sizeof *out);
        memset(out, 'b', ylen);
        return out;
    }
    if (ylen == 0) {
        mychar *out = calloc(xlen + 1, sizeof *out);
        memset(out, 'a', xlen);
        return out;
    }

    if (xlen <= 2 || ylen <= 2) return levenshtein_distance(x, y);

    int xmid = xlen/2;
    mychar *left = strsub(x, 0, xmid + 1); // TODO: Check if +1
    mychar *rev_right = strrev(x+xmid, xlen-xmid);
    mychar *rev_y = strrev(y, ylen);

#ifndef CODEJUDGE
    printf("left: %s\n", left);
    printf("rev_right: %s\n", rev_right);
    printf("rev_y: %s\n", rev_y);
#endif

    int *scoreL = nw_score(left, y);
    int *scoreR = nw_score(rev_right, rev_y);

    int i,
        current_min=scoreL[0] + scoreR[ylen],
        min_index = 0;

#ifndef CODEJUDGE
        printf("%3d", current_min);
#endif

    for(i = 1; i <= ylen; i++)
    {
        int score = scoreL[i] + scoreR[ylen-i];

        // Hard > is used to get the topmost 'best' value.
        if(current_min > score)
        {
            current_min = score;
            min_index = i;
        } 
#ifndef CODEJUDGE
        printf("%3d", score);
#endif
    }
    int ymid = min_index;
#ifndef CODEJUDGE
    printf("\n(xmid,ymid): (%d,%d)\n", xmid, ymid);
#endif
    mychar *ytop = strsub(y, 0, ymid + 1); // TODO: Check if +1
    mychar *out = hirchbergs_align(left, ytop); // left result
    mychar *right_result = hirchbergs_align(x + xmid, y + ymid);

#ifndef CODEJUDGE
    printf("(x,y): (%s,%s)\n", x, y);
    printf("left: (x,y,result) = (%s, %s, %s)\n", left, ytop, out);
    printf("right: (x,y,result) = (%s, %s, %s)\n", x + xmid, y + ymid, right_result);
#endif

    strcat(out, right_result + 1);

#ifndef CODEJUDGE
    printf("result: %s\n", out);
#endif

    free(left);
    free(ytop);
    free(right_result);
    return out;
}


// Needed for seed method
mychar *a, *b;

void edFullArray(myindex lena, myindex lenb, long seed) {
  srand48(seed);
  a = malloc((lena + 1) * sizeof a);
  a[lena] = 0;
  b = malloc((lenb + 1) * sizeof b);
  b[lenb] = 0;
  int i,j;
  for(i=0;i<lena;i++) a[i] = (lrand48() & 15) + 'a';
  for(j=0;j<lenb;j++) b[j] = (lrand48() & 15) + 'a';
}

int main(int argc, char **argv) {
  if (argc == 3) { // Two strings
    a = argv[1];
    b = argv[2];
  } else if (argc == 4) {
    myindex slen = atoi(argv[1]);
    myindex tlen = atoi(argv[2]);
    long seed = atol(argv[3]);

    edFullArray(slen, tlen, seed);
  } else {
    printf("Wrong amount of arguments!\n");
    exit(2);
  }

  char *result = hirchbergs_align(a, b);
  printf("%s\n", result);
  return 0;
}
