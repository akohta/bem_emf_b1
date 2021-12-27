// calculation example of far-field intensity distributions.
// radar chart is output for a distant scattering field. 
#include "bem3_emf_b1.h"

int main(int argc,char *argv[]) 
{
  DOMD md;
  FILE *fp1,*fp2;
  double complex e[3],h[3];
  double th,dthd,dthr,ra,r[3],*ie,*ih,mf,iemax,ihmax;
  int i,sn,ty;

  if(argc!=2 && argc!=5){
    printf("Usage : %s datafile_name [sampling_number multplier_factor type](optional)\n",argv[0]);
    printf("default sampling number 360, multiplier factor 2000 (radius = 2000*lambda0), type 1 (9 or 7 point Gauss-Legendre)\n");
    exit(0);
  }
  else if(argc==5){
    sn=atoi(argv[2]);
    mf=atof(argv[3]);
    ty=atoi(argv[4]);
  }
  else{
    sn=360;
    mf=2000.0;
    ty=1;
  }
  
  dat_read_domd(argv[1],&md); // read data file
  print_domd(&md);            // print data
  
  ra=mf*md.mw.lambda_0;      // radius for calculation point
  dthd=360.0/(double)sn;     // delta theta [degree]
  dthr=2.0*M_PI/(double)sn;  // delta theta [radian]
  
  ie=(double *)m_alloc2(sn,sizeof(double),"example4.c,ie");
  ih=(double *)m_alloc2(sn,sizeof(double),"example4.c,ih");
  
  // x=0 plane, th=0 : +z-axis, th=270 : +y-axis
  if((fp1=fopen("fsIe_yz.txt","wt"))==NULL){    printf("Can not open the file.\n");    exit(1);  }
  fprintf(fp1,"%s\n","## x=0 plane, theta=0 : +z-axis, theta=270 : +y-axis ");
  fprintf(fp1,"%s %d, %s %g\n","## sampling number",sn,"multiplier factor",mf);
  fprintf(fp1,"%s\n","# theta electric_field_intensity normalized_intensity");
  if((fp2=fopen("fsIh_yz.txt","wt"))==NULL){    printf("Can not open the file.\n");    exit(1);  }
  fprintf(fp2,"%s\n","## x=0 plane, theta=0 : +z-axis, theta=270 : +y-axis ");
  fprintf(fp2,"%s %d, %s %g\n","## sampling number",sn,"multiplier factor",mf);
  fprintf(fp2,"%s\n","# theta magnetic_field_intensity normalized_intensity");
  iemax=0.0;
  ihmax=0.0;
  #pragma omp parallel for schedule(dynamic) private(th,r,e,h)
  for(i=0;i<sn;i++){
    th=0.5*dthr+(double)i*dthr;
    r[0]=0.0;
    r[1]=-ra*sin(th);
    r[2]= ra*cos(th);
    EH_mEMP_s(e,h,r,ty,&md); // scattered field
    ie[i]=creal(e[0]*conj(e[0]))+creal(e[1]*conj(e[1]))+creal(e[2]*conj(e[2]));
    ih[i]=creal(h[0]*conj(h[0]))+creal(h[1]*conj(h[1]))+creal(h[2]*conj(h[2]));
    #pragma omp critical
    if(ie[i]>iemax) iemax=ie[i];
    #pragma omp critical
    if(ih[i]>ihmax) ihmax=ih[i];
  }
  for(i=0;i<sn;i++){
    th=0.5*dthd+(double)i*dthd;
    fprintf(fp1,"%g %15.14e %15.14e\n",th,ie[i],ie[i]/iemax);
    fprintf(fp2,"%g %15.14e %15.14e\n",th,ih[i],ih[i]/ihmax);
  }
  fclose(fp1);
  fclose(fp2);

  // y=0 plane, th=0 : +z-axis, th=90 : +x-axis
  if((fp1=fopen("fsIe_xz.txt","wt"))==NULL){    printf("Can not open the file.\n");    exit(1);  }
  fprintf(fp1,"%s\n","## y=0 plane, theta=0 : +z-axis, theta=90 : +x-axis ");
  fprintf(fp1,"%s %d, %s %g\n","## sampling number",sn,"multiplier factor",mf);
  fprintf(fp1,"%s\n","# theta electric_field_intensity normalized_intensity");
  if((fp2=fopen("fsIh_xz.txt","wt"))==NULL){    printf("Can not open the file.\n");    exit(1);  }
  fprintf(fp2,"%s\n","## x=0 plane, theta=0 : +z-axis, theta=90 : +x-axis ");
  fprintf(fp2,"%s %d, %s %g\n","## sampling number",sn,"multiplier factor",mf);
  fprintf(fp2,"%s\n","# theta magnetic_field_intensity normalized_intensity");
  iemax=0.0;
  ihmax=0.0;
  #pragma omp parallel for schedule(dynamic) private(th,r,e,h)
  for(i=0;i<sn;i++){
    th=0.5*dthr+(double)i*dthr;
    r[0]=ra*sin(th);
    r[1]=0.0;
    r[2]=ra*cos(th);
    EH_mEMP_s(e,h,r,ty,&md); // scattered field
    ie[i]=creal(e[0]*conj(e[0]))+creal(e[1]*conj(e[1]))+creal(e[2]*conj(e[2]));
    ih[i]=creal(h[0]*conj(h[0]))+creal(h[1]*conj(h[1]))+creal(h[2]*conj(h[2]));
    #pragma omp critical
    if(ie[i]>iemax) iemax=ie[i];
    #pragma omp critical
    if(ih[i]>ihmax) ihmax=ih[i];
  }
  for(i=0;i<sn;i++){
    th=0.5*dthd+(double)i*dthd;
    fprintf(fp1,"%g %15.14e %15.14e\n",th,ie[i],ie[i]/iemax);
    fprintf(fp2,"%g %15.14e %15.14e\n",th,ih[i],ih[i]/ihmax);
  }
  fclose(fp1);
  fclose(fp2);
  
  // z=0 plane, th=0 : +x-axis, th=90 : +y-axis
  if((fp1=fopen("fsIe_xy.txt","wt"))==NULL){    printf("Can not open the file.\n");    exit(1);  }
  fprintf(fp1,"%s\n","## z=0 plane, theta=0 : +x-axis, theta=90 : +y-axis ");
  fprintf(fp1,"%s %d, %s %g\n","## sampling number",sn,"multiplier factor",mf);
  fprintf(fp1,"%s\n","# theta electric_field_intensity normalized_intensity");
  if((fp2=fopen("fsIh_xy.txt","wt"))==NULL){    printf("Can not open the file.\n");    exit(1);  }
  fprintf(fp2,"%s\n","## z=0 plane, theta=0 : +x-axis, theta=90 : +y-axis ");
  fprintf(fp2,"%s %d, %s %g\n","## sampling number",sn,"multiplier factor",mf);
  fprintf(fp2,"%s\n","# theta magnetic_field_intensity normalized_intensity");
  iemax=0.0;
  ihmax=0.0;
  #pragma omp parallel for schedule(dynamic) private(th,r,e,h)
  for(i=0;i<sn;i++){
    th=0.5*dthr+(double)i*dthr;
    r[0]=ra*cos(th);
    r[1]=ra*sin(th);
    r[2]=0.0;
    EH_mEMP_s(e,h,r,ty,&md); // scattered field
    ie[i]=creal(e[0]*conj(e[0]))+creal(e[1]*conj(e[1]))+creal(e[2]*conj(e[2]));
    ih[i]=creal(h[0]*conj(h[0]))+creal(h[1]*conj(h[1]))+creal(h[2]*conj(h[2]));
    #pragma omp critical
    if(ie[i]>iemax) iemax=ie[i];
    #pragma omp critical
    if(ih[i]>ihmax) ihmax=ih[i];
  }
  for(i=0;i<sn;i++){
    th=0.5*dthd+(double)i*dthd;
    fprintf(fp1,"%g %15.14e %15.14e\n",th,ie[i],ie[i]/iemax);
    fprintf(fp2,"%g %15.14e %15.14e\n",th,ih[i],ih[i]/ihmax);
  }
  fclose(fp1);
  fclose(fp2);

  free(ie);
  free(ih);
  finalize_domd(&md);
   
  return 0;
}
