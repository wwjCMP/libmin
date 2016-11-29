/*  This is my library */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "libmin.h"


libmin_plan * libmin_init(int ndim, int history_record, double tolerance)
{
  int i;
  int nwork;
  libmin_plan *p;
  p = (libmin_plan*)malloc(sizeof(libmin_plan));

  p->status = 0;
  p->iter   = 0;
  p->ndim = ndim;
  p->history_record = history_record;
  p->tolerance = tolerance;
  p->diag = malloc(ndim*sizeof(double));

  nwork = ndim * ( 2 * history_record + 1 ) + 2 * history_record;
  p->work = malloc(nwork*sizeof(double));

  p->gtol = 0.9;
  p->stpmin = 1.0e-20;
  p->stpmax = 1.0e+20;
  p->stp    = 1.0;
  
  for(i=0; i<ndim; i++) p->diag[i] = 1.0;

  assert(p->ndim > 0);
  assert(p->history_record > 0);

  return p;
}


libmin_plan * libmin_init_diag(int ndim, int history_record, double tolerance, double *diag)
{
  int i;
  int nwork;
  libmin_plan *p;
  p = (libmin_plan*)malloc(sizeof(libmin_plan));

  p->status = 0;
  p->iter   = 0;
  p->ndim = ndim;
  p->history_record = history_record;
  p->tolerance = tolerance;
  p->diag = malloc(ndim*sizeof(double));

  nwork = ndim * ( 2 * history_record + 1 ) + 2 * history_record;
  p->work = malloc(nwork*sizeof(double));

  p->gtol = 0.9;
  p->stpmin = 1.0e-20;
  p->stpmax = 1.0e+20;
  p->stp    = 1.0;
  
  for(i=0; i<ndim; i++) p->diag[i] = diag[i];

  assert(p->ndim > 0);
  assert(p->history_record > 0);

  return p;
}





void libmin_destroy(libmin_plan * p)
{
  free(p->work);
  free(p->diag);
  free(p);
}

int libmin_execute(libmin_plan *p, double *x, double f, double *gradf)
{
 bool diagco   = false;
 double stpmin = 1.0e-20;
 double stpmax = 1.0e+20;

 lbfgs(p->ndim, p->history_record, x, f, gradf, diagco, p->diag, p->tolerance, p->work, &(p->status),
       &(p->gtol), stpmin, stpmax, &(p->stp), &(p->iter), &(p->line_info),
       &(p->line_dginit), &(p->line_finit),
       &(p->line_stx),  &(p->line_fx),  &(p->line_dgx),
       &(p->line_sty),  &(p->line_fy),  &(p->line_dgy),
       &(p->line_stmin),  &(p->line_stmax), 
       &(p->line_bracket), &(p->line_stage1), &(p->line_infoc));


 return p->status;

}
