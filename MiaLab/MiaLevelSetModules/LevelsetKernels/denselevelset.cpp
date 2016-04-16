#include "denselevelset.h"
#define MAXSTEP 0.8

DenseLevelSet::DenseLevelSet(QObject *parent) :
    QThread(parent)
{
    phi = NULL;
    externalForce = NULL;
    statusmap = NULL;
    for( int i = 0 ; i < 3 ; i++ )
    {
        dimension[i] = 0;
        spacing[i] = 0;
        dimoffset[0] = 0;
    }
    totalSize = 0;
    sliceSize = 0;
    imageW = 0;
    imageH = 0;
    imageD = 0;
    updateInterval = 5;

    its = 0;
    periods = 0;
    dampingFactor = 0.5;
    currenttrend = -1;
    maxiumStepLength = MAXSTEP;
    externalWeightFactor = 0.5;

    maxPeriod = 4;
    maxIteration = 400;

//    currentAlgorithm = MIA_ThresholdBased;
//    currentAlgorithm = MIA_GradientBased;

    isValid = false;
    isAbort = false;
    imDimension = 3;

    connect(this,SIGNAL(levelsetFunctionUpdated()), parent, SLOT(forwardLevelSetFunctionUpdated()));
    connect(this,SIGNAL(levelsetFunctionCompleted()), parent, SLOT(forwardLevelsetFunctionCompleted()));
    connect(this,SIGNAL(levelsetEnterNewPeriod(int)), parent, SLOT(forwardLevelsetEnterNewPeriod(int)));
    setPriority(QThread::HighestPriority);
}

void DenseLevelSet::run()
{
    its = 0;
    virtualtime = 0.0;
    backwardPropagation = 0;
    totalComputation = 0;
    backwardAverageForce = 0;
    averageForce = 0;
    wigglemap = (float*) malloc(512*512*sizeof(float));
    memset(wigglemap, 0,512*512*sizeof(float));

    for( ; its < maxIteration ; its ++ )
    {
        if(isAbort)
            break;
//        QTime t;
//         t.start();
//        reinitialize();
//        qDebug() << "level set reinit: time elapsed: " << t.elapsed();
//        t.start();

        if(imDimension == 2)
            update_phi();


//        qDebug() << "level set update: time elapsed: " << t.elapsed();

//        if(changed==0)
//        {
//            currenttrend=-currenttrend;
//            periods++;
//            maxiumStepLength *= dampingFactor;
//            virtualtime = 0;

//            if(periods >= maxPeriod )
//                    its=maxIteration;
//            else {
//                    its=1; // to avoid sent update signal
//                    emit levelsetEnterNewPeriod(periods);
////                     qDebug() << "LevelSet Changing period";
//                    recreateActiveBand();

//            }


//        }

        if(its%updateInterval==0)
        {
            emit levelsetFunctionUpdated();
//            qDebug() << "LevelSet Updated";
        }

    }
    memcpy(phi,wigglemap,512*512*sizeof(float));

    emit levelsetFunctionUpdated();
    emit levelsetFunctionCompleted();
    qDebug() << "LevelSet Finished" << virtualtime<< " wiggled " <<backwardPropagation << totalComputation << "force" << backwardAverageForce/its << averageForce/its;

    its = 0;
    periods = 0;
    currenttrend = -1;
    maxiumStepLength = MAXSTEP;
    isAbort = false;

}
void DenseLevelSet::abort()
{

}

void DenseLevelSet::resume()
{

}
void DenseLevelSet::recreateActiveBand()
{

}

// p is the phi
// ext is the external force
// smap is the status map, no effects in the conventional levelset algorithms
// d is the dimension of input data
// s is the spacing between voxel.
bool DenseLevelSet::initialize( float* p, float* ext, uchar* smap, int* d, float* s)
{
    phi = p;
    externalForce = ext;
    statusmap = smap;
    totalSize = 1;
    for( int i = 0 ; i < 3 ; i++ )
    {
        dimension[i] = d[i];
        totalSize *= dimension[i];
        spacing[i] = s[i];
    }
    if(d[2] ==1 )
        imDimension = 2;
    else
        imDimension = 3;
    its = 0;
    periods = 0;
    maxiumStepLength = MAXSTEP;
    currenttrend = -1;

    imageW = dimension[0];
    imageH = dimension[1];
    imageD = dimension[2];
    sliceSize = imageW*imageH;

    dimoffset[0] = 1;
    dimoffset[1] = imageW;
    dimoffset[2] = sliceSize = imageW*imageH;;

    if((phi1=(float *)malloc(totalSize*sizeof(float)))==NULL)
        return false;



    int *init;
    init = (int*)phi1;

    for(int r=0;r<imageH;r++){
            for(int c=0;c<imageW;c++){
                if(phi[r*imageW+c] < 0 )
                    smap[r*imageW+c] = 1;
                else
                    smap[r*imageW+c] = 0;
                    //printf("%3d ", mask[r*imageW+c]);
            }
            //printf("\n");
    }

    distanceTransform2D(init,smap,imageH,imageW);

    //printf("sdf of init mask\n");
    for(int r=0;r<imageH;r++){
            for(int c=0;c<imageW;c++){
                    phi[r*imageW+c]=(float)init[r*imageW+c];
                    float value = fabs(phi[r*imageW+c]);
                    if(phi[r*imageW+c]>0 ){
                        if( value < imageW*imageW + imageH*imageH)
                            phi[r*imageW+c]=0.5*sqrt(fabs(phi[r*imageW+c]));
                        else
                            phi[r*imageW+c]= 0.5*sqrt((float)(imageW*imageW + imageH*imageH));

                    } else {
                        if( value < imageW*imageW + imageH*imageH)
                            phi[r*imageW+c]=-0.5*sqrt(fabs(phi[r*imageW+c]));
                        else
                            phi[r*imageW+c]= -0.5*sqrt((float)(imageW*imageW + imageH*imageH));
                    }
                    value = phi[r*imageW+c];
//                    printf("%6.3f ", phi[r*imageW+c]);
            }
            //printf("\n");
    }

    return true;
}


void DenseLevelSet::update_phi(){
        float maxPropogating=0;
        float maxCurvature =0;
        float dx;
        float dxplus;
        float dxminus;
        float dxplusy;
        float dxminusy;
        float maxdxplus;
        float maxminusdxminus;
        float mindxplus;
        float minminusdxminus;

        float dy;
        float dyplus;
        float dyminus;
        float dyplusx;
        float dyminusx;
        float maxdyplus;
        float maxminusdyminus;
        float mindyplus;
        float minminusdyminus;

        float gradphimax;
        float gradphimin;

        float nplusx;
        float nplusy;
        float nminusx;
        float nminusy;
        float curvature;

        float F;
        float gradphi;


        int ind;


        for(int r=0;r<imageH;r++){
                for( int c=0;c<imageW;c++){

                        ind=r*imageW+c;

                        if(c==0||c==imageW-1){dx=0;} else {dx=(phi[ind+1]-phi[ind-1])/2;}
                        if(c==imageW-1){dxplus=0;} else {dxplus=(phi[ind+1]-phi[ind]);}
                        if(c==0){dxminus=0;} else {dxminus=(phi[ind]-phi[ind-1]);}
                        if(r==0||c==0||c==imageW-1){dxplusy=0;} else {dxplusy=(phi[ind-imageW+1]-phi[ind-imageW-1])/2;}
                        if(r==imageH-1||c==0||c==imageW-1){dxminusy=0;} else {dxminusy=(phi[ind+imageW+1]-phi[ind+imageW-1])/2;}
                        if(dxplus<0){maxdxplus=0;} else { maxdxplus= dxplus*dxplus; }
                        if(-dxminus<0){maxminusdxminus=0;} else { maxminusdxminus= dxminus*dxminus; }
                        if(dxplus>0){mindxplus=0;} else { mindxplus= dxplus*dxplus; }
                        if(-dxminus>0){minminusdxminus=0;} else { minminusdxminus= dxminus*dxminus; }

                        if(r==0||r==imageH-1){dy=0;} else {dy=(phi[ind-imageW]-phi[ind+imageW])/2;}
                        if(r==0){dyplus=0;} else {dyplus=(phi[ind-imageW]-phi[ind]);}
                        if(r==imageH-1){dyminus=0;} else {dyminus=(phi[ind]-phi[ind+imageW]);}
                        if(r==0||c==imageW-1||r==imageH-1){dyplusx=0;} else {dyplusx=(phi[ind-imageW+1]-phi[ind+imageW+1])/2;}
                        if(r==0||c==0||r==imageH-1){dyminusx=0;} else {dyminusx=(phi[ind-imageW-1]-phi[ind+imageW-1])/2;}
                        if(dyplus<0){maxdyplus=0;} else { maxdyplus= dyplus*dyplus; }
                        if(-dyminus<0){maxminusdyminus=0;} else { maxminusdyminus= dyminus*dyminus; }
                        if(dyplus>0){mindyplus=0;} else { mindyplus= dyplus*dyplus; }
                        if(-dyminus>0){minminusdyminus=0;} else { minminusdyminus= dyminus*dyminus; }

                        gradphimax=sqrt((sqrt(maxdxplus+maxminusdxminus))*(sqrt(maxdxplus+maxminusdxminus))+(sqrt(maxdyplus+maxminusdyminus))*(sqrt(maxdyplus+maxminusdyminus)));
                        gradphimin=sqrt((sqrt(mindxplus+minminusdxminus))*(sqrt(mindxplus+minminusdxminus))+(sqrt(mindyplus+minminusdyminus))*(sqrt(mindyplus+minminusdyminus)));
                        nplusx= dxplus / sqrt(1.192092896e-07F + (dxplus*dxplus) + ((dyplusx + dy)*(dyplusx + dy)*0.25) );
                        nplusy= dyplus / sqrt(1.192092896e-07F + (dyplus*dyplus) + ((dxplusy + dx)*(dxplusy + dx)*0.25) );
                        nminusx= dxminus / sqrt(1.192092896e-07F + (dxminus*dxminus) + ((dyminusx + dy)*(dyminusx + dy)*0.25) );
                        nminusy= dyminus / sqrt(1.192092896e-07F + (dyminus*dyminus) + ((dxminusy + dx)*(dxminusy + dx)*0.25) );
                        curvature= ((nplusx-nminusx)+(nplusy-nminusy))/2;

                        F = (-externalWeightFactor * externalForce[ind]) + ((1 - externalWeightFactor) * curvature);
                        if(F>0) {gradphi=gradphimax;} else {gradphi=gradphimin;}
                        phi1[ind]= F * gradphi;
                        if(fabs(phi1[ind])>maxPropogating)
                        {
                                maxPropogating=fabs(phi1[ind]);
                                maxCurvature=ind;
                        }
                }
        }

//	float dt = fmin(0.9/maxPropogating,0.45/maxCurvature);

        float dt = 0.8/maxPropogating;

        dt=0.25;
        for(int i=0;i<totalSize;i++){
                phi[i]+=dt*phi1[i];
                float force = externalForce[i];
                if(phi1[i]*externalForce[i]>0)
                {
                    backwardPropagation++;

                    wigglemap[i]++;
                }
        }

}
void DenseLevelSet::distanceTransform2D(int *_d,unsigned char *_bimg,int _h,int _w)
{
  int *dd;
  int *f;
  int *v;
  int *z;
  int  i;
  int  j;
  int  k;
  int  n;
  /*We use an adaptation of the O(n) algorithm from \cite{FH04}.
    Our adaptation computes the positive and negative transform values
     simultaneously in a single pass.
    We could use the orignal algorithm on a doubled image as described in our
     documentation, but choose instead to operate directly on the original
     image, which makes for more complex code, but vastly reduced memory usage.
  @TECHREPORT{
    author="Pedro F. Felzenszwalb and Daniel P. Huttenlocher",
    title="Distance Transforms of Sampled Functions",
    number="TR2004-1963",
    institution="Cornell Computing and Information Science",
    year=2004
  }*/
  if(_h<=0||_w<=0)return;
  /*We do not strictly need this temporary image, but using it allows us to
     read out of it untransposed in the second pass, which is much nicer to the
     cache.
    We still have to write a transposed image in each pass, but writing
     introduces fewer stalls than reading.*/
  dd=(int *)malloc(_h*_w*sizeof(*dd));
  n=_h>_w?_h:_w;
  v=(int *)malloc(n*sizeof(*v));
  z=(int *)malloc((n+1)*sizeof(*z));
  f=(int *)malloc(_h*sizeof(*f));
  /*First compute the signed distance transform along the X axis.*/
  for(i=0;i<_h;i++){
    k=-1;
    /*At this stage, every transition contributes a parabola to the final
       envelope, and the intersection point _must_ lie between the vertices, so
       there's no need to worry about deletion or bounds checking.*/
    for(j=1;j<_w;j++)if(!_bimg[i*_w+j-1]!=!_bimg[i*_w+j]){
      int q;
      int s;
      q=(j<<1)-1;
      s=k<0?0:(v[k]+q>>2)+1;
      v[++k]=q;
      z[k]=s;
    }
    /*Now, go back and compute the distances to each parabola.
      If there were _no_ parabolas, then fill the row with +/- infinity.*/
    /*This is equivalent to dd[j*_h+i]=_bimg[i*_w+j]?INT_MIN:INT_MAX;*/
    if(k<0)for(j=0;j<_w;j++)dd[j*_h+i]=INT_MAX+!!_bimg[i*_w+j];
    else{
      int zk;
      z[k+1]=_w;
      j=k=0;
      do{
        int d1;
        int d2;
        d1=(j<<1)-v[k];
        d2=d1*d1;
        d1=d1+1<<2;
        zk=z[++k];
        for(;;){
          /*This is equivalent to dd[j*_h+i]=_bimg[i*_w+j]?-d2:d2;*/
          dd[j*_h+i]=d2-(d2<<1&-!!_bimg[i*_w+j]);
          if(++j>=zk)break;
          d2+=d1;
          d1+=8;
        }
      }
      while(zk<_w);
    }
  }
  /*Now extend the signed distance transform along the Y axis.
    This part of the code heavily depends on good branch prediction to be
     fast.*/
  for(j=0;j<_w;j++){
    int psign;
    /*v2 is not used uninitialzed, despite what your compiler may think.*/
    int v2;
    int q2;
    k=-1;
    /*Choose an initial value of psign that ensures there's no transition.
      This is the reason for the _h<=0 test at the start of the function.*/
    psign=dd[j*_h+0]<0;
    for(i=0,q2=1;i<_h;i++){
      int sign;
      int d;
      d=dd[j*_h+i];
      sign=d<0;
      /*If the sign changes, we've found a boundary point, and place a parabola
         of height 0 there.*/
      if(sign!=psign){
        int q;
        int s;
        q=(i<<1)-1;
        if(k<0)s=0;
        else for(;;){
          s=q2-v2-f[k];
          /*This is subtle: if q==v[k], then the parabolas never intersect, but
             our test is correct anyway, because f[k] is always positive.*/
          if(s>0){
            s=s/(q-v[k]<<2)+1;
            if(s>z[k])break;
          }
          else s=0;
          if(--k<0)break;
          v2=v[k]*v[k];
        }
        v[++k]=q;
        f[k]=0;
        z[k]=s;
        v2=q2;
      }
      /*We test for INT_MIN and INT_MAX together by adding +1 or -1 depending
         on the sign of d and checking if it retains that sign.
        If we have a finite point, we place up to three parabolas around it at
         height abs(d).
        There's no need to distinguish between the envelope inside and outside
         the shape, as a parabola of height 0 will always lie between them.*/
      if(sign==d-sign+!sign<0){
        int fq;
        int q;
        int s;
        int t;
        fq=abs(d);
        q=(i<<1)-1;
        if(k<0){
          s=0;
          t=1;
        }
        else for(;;){
          t=(q+1-v[k])*(q+1-v[k])+f[k]-fq;
          /*If the intersection point occurs to the left of the vertex, we will
             add all three parabolas, so we compute the intersection with the
             left-most parabola here.*/
          if(t>0){
            /*This is again subtle: if q==v[k], then we will take this branch
               whenever f[k]>=fq.
              The parabolas then intersect everywhere (when f[k]==fq) or
               nowhere (when f[k]>fq).
              However, in either case s<=0, and so we skip the division by zero
               below and delete the previous parabola.
              This relies on the fact that we ensure z[k] is never negative.*/
            s=q2-v2+fq-f[k];
            s=s<=0?0:s/(q-v[k]<<2)+1;
          }
          /*Otherwise we only add the right-most, so we compute that
             intersection point.
            (q+1)'s intersection point must lie even farther to the right than
             q's, so there is no needs to boundary check against 0.*/
          else s=(q2+(i<<3)-v2+fq-f[k])/(q+2-v[k]<<2)+1;
          if(s>z[k]||--k<0)break;
          v2=v[k]*v[k];
        }
        if(t>0){
          /*We only add the left-most parabola if it affects at least one
             pixel to prevent overrunning our arrays (e.g., consider the case
             _h==1).*/
          if(s<i){
            v[++k]=q;
            f[k]=fq;
            z[k]=s;
          }
          /*The center parabola will always span the interval [i,i+1), since
             the left and right parabolas are better outside of it.*/
          v[++k]=q+1;
          f[k]=fq;
          z[k]=i;
          s=i+1;
        }
        /*We only add the right-most parabola if it affects at least one pixel,
           to prevent overrunning our arrays (e.g., consider the case _h==1).
          This also conveniently ensures that z[k] is never larger than _h.*/
        if(s<_h){
          v[++k]=q+2;
          f[k]=fq;
          z[k]=s;
          v2=q2+(i<<3);
        }
      }
      psign=sign;
      q2+=i<<3;
    }
    /*Now, go back and compute the distances to each parabola.*/
    if(k<0){
      /*If there were _no_ parabolas, then the shape is uniform, and we've
         already filled it with +/- infinity in the X pass, so there's no need
         to examine the rest of the columns.
        Just copy the whole thing to the output image.*/
      memcpy(_d,dd,_w*_h*sizeof(*_d));
      break;
    }
    else{
      int zk;
      z[k+1]=_h;
      i=k=0;
      do{
        int d2;
        int d1;
        d1=(i<<1)-v[k];
        d2=d1*d1+f[k];
        d1=d1+1<<2;
        zk=z[++k];
        for(;;){
          /*This is equivalent to _d[i*_w+j]=dd[j*_h+i]<0?-d2:d2;*/
          _d[i*_w+j]=d2-(d2<<1&-(dd[j*_h+i]<0));
          if(++i>=zk)break;
          d2+=d1;
          d1+=8;
        }
      }
      while(zk<_h);
    }
  }
  free(f);
  free(z);
  free(v);
  free(dd);
}
