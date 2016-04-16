#include "abstractlevelsetspeedfucntion.h"
#include "LevelSetRoutines.h"

AbstractLevelSetSpeedFucntion::AbstractLevelSetSpeedFucntion(QObject *parent) :
    QObject(parent)
{
    externalForce = NULL;
    phi = NULL;
    alpha = 0;
    beta = 0;
    gamma = 0;
}
#ifdef USESHORTTYPELEVELSETFUNCTION
bool AbstractLevelSetSpeedFucntion::initialize( short* p, void* ext, int* d, float* s)
#else
bool AbstractLevelSetSpeedFucntion::initialize( float* p, void* ext, int* d, float* s)
#endif
{
    //set up the play ground
    phi = p;
    externalForce = ext;
    imInfo.totalSize = 1;
    for( int i = 0 ; i < 3 ; i++ )
    {
        imInfo.dimension[i] = d[i];
        imInfo.totalSize *= imInfo.dimension[i];
        imInfo.spacing[i] = s[i];
    }
    if(d[2] ==1 )
        imInfo.imDimension = 2;
    else
        imInfo.imDimension = 3;
    imInfo.sliceSize = imInfo.dimension[0]*imInfo.dimension[1];
    imInfo.dimoffset[0] = 1;
    imInfo.dimoffset[1] = imInfo.dimension[0];
    imInfo.dimoffset[2] = imInfo.sliceSize;
    return isValid();
}

//float AbstractLevelSetSpeedFucntion::getDetlaPhi(const int cor[3])
//{
//    // small loop code
//     int dx_phi[3];
//     int dx_phiplus[3], dx_phiminus[3];
//     float gradphi_upwind, gradphi_downwind;
//     int ind = cor[2]*imInfo.sliceSize + cor[1]*imInfo.dimension[0] + cor[0];
//     bool isOnBoundingBoxEdges = false;

//     gradphi_upwind = 0;
//     gradphi_downwind = 0;
//     for(int dim = 0; dim < imInfo.imDimension; dim++)
//     {
//         if(cor[dim]==0||cor[dim]==imInfo.dimension[dim]-1)
//         {
//             dx_phi[dim]=0;
//             isOnBoundingBoxEdges = true;
//         }
//         else
//             dx_phi[dim]=(phi[ind+imInfo.dimoffset[dim]]-phi[ind-imInfo.dimoffset[dim]])/2;

//         if(cor[dim]==0)
//             dx_phiminus[dim]=0;
//         else
//             dx_phiminus[dim]=phi[ind]-phi[ind-imInfo.dimoffset[dim]];

//         if(cor[dim]==imInfo.dimension[dim]-1)
//             dx_phiplus[dim]=0;
//         else
//             dx_phiplus[dim]=(phi[ind+imInfo.dimoffset[dim]]-phi[ind]);

//         int up_g = max(dx_phiplus[dim],0)*max(dx_phiplus[dim],0)+max(-dx_phiminus[dim],0)*max(-dx_phiminus[dim],0);
//         gradphi_upwind += (float)(up_g*up_g);
//         int down_g = min(dx_phiplus[dim],0)*min(dx_phiplus[dim],0)+min(-dx_phiminus[dim],0)*min(-dx_phiminus[dim],0);
//         gradphi_downwind += (float)(down_g*down_g);

//     }
//     gradphi_upwind = sqrt(gradphi_upwind)*0.0001;
//     gradphi_downwind = sqrt(gradphi_downwind)*0.0001;

////        double test_norm2;
//     float curvature= 0.0f;

//     for(int dim = 0; dim < imInfo.imDimension; dim++)
//     {
//         double normplus = 1.192092896e-07F;
//         normplus += dx_phiplus[dim]*dx_phiplus[dim];
//         double normminus = 1.192092896e-07F;
//         double normalplus,normalminus;
//         normminus += dx_phiminus[dim]*dx_phiminus[dim];

//         for(int dim2 = 0; dim2 < imInfo.imDimension; dim2 ++)
//         {
//             if(dim == dim2)
//                 continue;
//             int dx_phi_at_plus,dx_phi_at_minus;
//             if(cor[dim2]==0||cor[dim2]==imInfo.dimension[dim2]-1||cor[dim] == imInfo.dimension[dim]-1)
//             {
//                 dx_phi_at_plus = 0;
//             }
//             else
//             {
//                 dx_phi_at_plus = (phi[ind+imInfo.dimoffset[dim]+imInfo.dimoffset[dim2]]-phi[ind+imInfo.dimoffset[dim]-imInfo.dimoffset[dim2]])/2;
//             }

//             float temp_plus = (float)(dx_phi_at_plus + dx_phi[dim2])*0.0001;
//             normplus += (temp_plus)*(temp_plus)*0.25;

//             if(cor[dim2]==0||cor[dim2]==imInfo.dimension[dim2]-1||cor[dim] == 0)
//             {
//                 dx_phi_at_minus = 0;
//             }
//             else
//             {
//                 dx_phi_at_minus = (phi[ind-imInfo.dimoffset[dim]+imInfo.dimoffset[dim2]]-phi[ind-imInfo.dimoffset[dim]-imInfo.dimoffset[dim2]])/2.0f;
//             }

//             double temp_minus = (double)(dx_phi_at_minus + dx_phi[dim2])*0.0001;
//             normminus += (temp_minus)*(temp_minus)*0.25;

//         }
//         normplus = sqrt(normplus);
//         normalplus = dx_phiplus[dim]/normplus;

//         normminus = sqrt(normminus);
//         normalminus = dx_phiminus[dim]/normminus;

//         curvature += normalplus-normalminus;
//     }
//     curvature /= 2.0f;//may also be (float)imInfo.imDimension;

//     float delta_phi = speedFuction(ind, curvature, NULL);

//     if(delta_phi > 0)
//         delta_phi = delta_phi * gradphi_upwind;
//     else
//         delta_phi = delta_phi * gradphi_downwind;
//     //this is to prevent the shape to develope holes around the border of the volume
//     // the
//     if (delta_phi<0 && isOnBoundingBoxEdges) //which should be         else if (delta_phi<0 && (i==0||i==imInfo.dimension[0]-1||j==0||j==imInfo.dimension[1]-1||k==0||k==imInfo.dimension[2]-1))
//     {
//         delta_phi = 0;
//     }

//     return delta_phi;
//}
float AbstractLevelSetSpeedFucntion::getDetlaPhi(const int cor[3])
{
    // small loop code
     float dx_phi[3];
     float dx_phiplus[3], dx_phiminus[3];
     float gradphi_upwind, gradphi_downwind;
     int ind = cor[2]*imInfo.sliceSize + cor[1]*imInfo.dimension[0] + cor[0];
     bool isOnBoundingBoxEdges = false;

     gradphi_upwind = 0.0f;
     gradphi_downwind = 0.0f;
     for(int dim = 0; dim < imInfo.imDimension; dim++)
     {
         if(cor[dim]==0||cor[dim]==imInfo.dimension[dim]-1)
         {
             dx_phi[dim]=0;
             isOnBoundingBoxEdges = true;
         }
         else
             dx_phi[dim]=(phi[ind+imInfo.dimoffset[dim]]-phi[ind-imInfo.dimoffset[dim]])/2.0f;

         if(cor[dim]==0)
             dx_phiminus[dim]=0;
         else
             dx_phiminus[dim]=phi[ind]-phi[ind-imInfo.dimoffset[dim]];

         if(cor[dim]==imInfo.dimension[dim]-1)
             dx_phiplus[dim]=0;
         else
             dx_phiplus[dim]=(phi[ind+imInfo.dimoffset[dim]]-phi[ind]);

#ifdef USESHORTTYPELEVELSETFUNCTION
         dx_phi[dim] *= 0.0001f;
         dx_phiminus[dim] *= 0.0001f;
         dx_phiplus[dim] *= 0.0001f;
#endif
         float up_g = max(dx_phiplus[dim],0)*max(dx_phiplus[dim],0)+max(-dx_phiminus[dim],0)*max(-dx_phiminus[dim],0);
         gradphi_upwind += up_g*up_g;
         float down_g = min(dx_phiplus[dim],0)*min(dx_phiplus[dim],0)+min(-dx_phiminus[dim],0)*min(-dx_phiminus[dim],0);
         gradphi_downwind += down_g*down_g;

     }
     gradphi_upwind = sqrt(gradphi_upwind);
     gradphi_downwind = sqrt(gradphi_downwind);

//        double test_norm2;
     float curvature= 0.0f;

     for(int dim = 0; dim < imInfo.imDimension; dim++)
     {
         double normplus = 1.192092896e-07F;
         normplus += dx_phiplus[dim]*dx_phiplus[dim];
         double normminus = 1.192092896e-07F;
         double normalplus,normalminus;
         normminus += dx_phiminus[dim]*dx_phiminus[dim];

         for(int dim2 = 0; dim2 < imInfo.imDimension; dim2 ++)
         {
             if(dim == dim2)
                 continue;
             float dx_phi_at_plus,dx_phi_at_minus;
             if(cor[dim2]==0||cor[dim2]==imInfo.dimension[dim2]-1||cor[dim] == imInfo.dimension[dim]-1)
             {
                 dx_phi_at_plus = 0;
             }
             else
             {
                 dx_phi_at_plus = (phi[ind+imInfo.dimoffset[dim]+imInfo.dimoffset[dim2]]-phi[ind+imInfo.dimoffset[dim]-imInfo.dimoffset[dim2]])/2.0f;
             }

#ifdef USESHORTTYPELEVELSETFUNCTION
             dx_phi_at_plus *= 0.0001f;
#endif

             normplus += (dx_phi_at_plus + dx_phi[dim2])*(dx_phi_at_plus + dx_phi[dim2])*0.25;

             if(cor[dim2]==0||cor[dim2]==imInfo.dimension[dim2]-1||cor[dim] == 0)
             {
                 dx_phi_at_minus = 0;
             }
             else
             {
                 dx_phi_at_minus = (phi[ind-imInfo.dimoffset[dim]+imInfo.dimoffset[dim2]]-phi[ind-imInfo.dimoffset[dim]-imInfo.dimoffset[dim2]])/2.0f;
             }

#ifdef USESHORTTYPELEVELSETFUNCTION
             dx_phi_at_minus *= 0.0001f;
#endif
             normminus += (dx_phi_at_minus + dx_phi[dim2])*(dx_phi_at_minus + dx_phi[dim2])*0.25;

         }
         normplus = sqrt(normplus);
         normalplus = dx_phiplus[dim]/normplus;

         normminus = sqrt(normminus);
         normalminus = dx_phiminus[dim]/normminus;

         curvature += normalplus-normalminus;
     }
     curvature /= 2.0f;//may also be (float)imInfo.imDimension;

     float delta_phi = speedFuction(ind, curvature, dx_phi);

     if(delta_phi > 0)
         delta_phi = delta_phi * gradphi_upwind;
     else
         delta_phi = delta_phi * gradphi_downwind;
     //this is to prevent the shape to develope holes around the border of the volume
     // the
     if (delta_phi<0 && isOnBoundingBoxEdges) //which should be         else if (delta_phi<0 && (i==0||i==imInfo.dimension[0]-1||j==0||j==imInfo.dimension[1]-1||k==0||k==imInfo.dimension[2]-1))
     {
         delta_phi = 0;
     }

     return delta_phi;
}

//unwrapped version
//float AbstractLevelSetSpeedFucntion::getDetlaPhi(const int cor[3])
//{
////unwrapped cord
//        float dx,dy,dz;
//        float dxplus, dyplus, dzplus, dxminus, dyminus, dzminus;
//        float dxplusy, dxminusy, dxplusz, dxminusz, dyplusx, dyminusx, dyplusz, dyminusz, dzplusx, dzminusx, dzplusy, dzminusy;
//        float gradphimax, gradphimin, nplusx, nplusy, nplusz, nminusx, nminusy, nminusz;
//        int i,j,k;
//        k=cor[2];
//        j=cor[1];
//        i=cor[0];
//        int ind = cor[2]*imInfo.sliceSize + cor[1]*imInfo.dimension[0] + cor[0];

//         if(i==0||i==imInfo.dimension[0]-1){dx=0;} else {dx=(phi[ind+1]-phi[ind-1])/2;}
//         if(j==0||j==imInfo.dimension[1]-1){dy=0;} else {dy=(phi[ind-imInfo.dimension[0]]-phi[ind+imInfo.dimension[0]])/2;}
//         if(k==0||k==imInfo.dimension[2]-1){dz=0;} else {dz=(phi[ind+imInfo.sliceSize]-phi[ind-imInfo.sliceSize])/2;}

//         if(i==imInfo.dimension[0]-1){dxplus=0;} else {dxplus=(phi[ind+1]-phi[ind]);}
//         if(j==0){dyplus=0;} else {dyplus=(phi[ind-imInfo.dimension[0]]-phi[ind]);}
//         if(k==imInfo.dimension[2]-1){dzplus=0;} else {dzplus=(phi[ind+imInfo.sliceSize]-phi[ind]);}
//         if(i==0){dxminus=0;} else {dxminus=(phi[ind]-phi[ind-1]);}
//         if(j==imInfo.dimension[1]-1){dyminus=0;} else {dyminus=(phi[ind]-phi[ind+imInfo.dimension[0]]);}
//         if(k==0){dzminus=0;} else {dzminus=(phi[ind]-phi[ind-imInfo.sliceSize]);}

//         if(i==0||i==imInfo.dimension[0]-1||j==0){dxplusy=0;} else {dxplusy=(phi[ind-imInfo.dimension[0]+1]-phi[ind-imInfo.dimension[0]-1])/2;}
//         if(i==0||i==imInfo.dimension[0]-1||j==imInfo.dimension[1]-1){dxminusy=0;} else {dxminusy=(phi[ind+imInfo.dimension[0]+1]-phi[ind+imInfo.dimension[0]-1])/2;}
//         if(i==0||i==imInfo.dimension[0]-1||k==imInfo.dimension[2]-1) {dxplusz=0;} else {dxplusz=(phi[ind+imInfo.sliceSize+1]-phi[ind+imInfo.sliceSize-1])/2;}
//         if(i==0||i==imInfo.dimension[0]-1||k==0) {dxminusz=0;} else {dxminusz=(phi[ind-imInfo.sliceSize+1]-phi[ind-imInfo.sliceSize-1])/2;}
//         if(j==0||j==imInfo.dimension[1]-1||i==imInfo.dimension[0]-1){dyplusx=0;} else {dyplusx=(phi[ind-imInfo.dimension[0]+1]-phi[ind+imInfo.dimension[0]+1])/2;}
//         if(j==0||j==imInfo.dimension[1]-1||i==0){dyminusx=0;} else {dyminusx=(phi[ind-imInfo.dimension[0]-1]-phi[ind+imInfo.dimension[0]-1])/2;}
//         if(j==0||j==imInfo.dimension[1]-1||k==imInfo.dimension[2]-1) {dyplusz=0;} else {dyplusz=(phi[ind+imInfo.sliceSize-imInfo.dimension[0]]-phi[ind+imInfo.sliceSize+imInfo.dimension[0]])/2;}
//         if(j==0||j==imInfo.dimension[1]-1||k==0) {dyminusz=0;} else {dyminusz=(phi[ind-imInfo.sliceSize-imInfo.dimension[0]]-phi[ind-imInfo.sliceSize+imInfo.dimension[0]])/2;}
//         if(k==0||k==imInfo.dimension[2]-1||i==imInfo.dimension[0]-1) {dzplusx=0;} else {dzplusx=(phi[ind+1+imInfo.sliceSize]-phi[ind+1-imInfo.sliceSize])/2;}
//         if(k==0||k==imInfo.dimension[2]-1||i==0) {dzminusx=0;} else {dzminusx=(phi[ind-1+imInfo.sliceSize]-phi[ind-1-imInfo.sliceSize])/2;}
//         if(k==0||k==imInfo.dimension[2]-1||j==0) {dzplusy=0;} else {dzplusy=(phi[ind-imInfo.dimension[0]+imInfo.sliceSize]-phi[ind-imInfo.dimension[0]-imInfo.sliceSize])/2;}
//         if(k==0||k==imInfo.dimension[2]-1||j==imInfo.dimension[1]-1) {dzminusy=0;} else {dzminusy=(phi[ind+imInfo.dimension[0]+imInfo.sliceSize]-phi[ind+imInfo.dimension[0]-imInfo.sliceSize])/2;}


//         gradphimax=sqrt(max(dxplus,0)*max(dxplus,0)+max(-dxminus,0)*max(-dxminus,0)
//                        +max(dyplus,0)*max(dyplus,0)+max(-dyminus,0)*max(-dyminus,0)
//                        +max(dzplus,0)*max(dzplus,0)+max(-dzminus,0)*max(-dzminus,0));

//         gradphimin=sqrt(min(dxplus,0)*min(dxplus,0)+min(-dxminus,0)*min(-dxminus,0)
//                        +min(dyplus,0)*min(dyplus,0)+min(-dyminus,0)*min(-dyminus,0)
//                        +min(dzplus,0)*min(dzplus,0)+min(-dzminus,0)*min(-dzminus,0));

//         nplusx= dxplus / sqrt((double)(1.192092896e-07F) + (double)(dxplus*dxplus) + (double)((dyplusx + dy)*(dyplusx + dy)*0.25) + (double)((dzplusx + dz)*(dzplusx + dz)*0.25));
//         nplusy= dyplus / sqrt((double)(1.192092896e-07F) + (double)(dyplus*dyplus) + (double)((dxplusy + dx)*(dxplusy + dx)*0.25) + (double)((dzplusy + dz)*(dzplusy + dz)*0.25));
//         nplusz= dzplus / sqrt((double)(1.192092896e-07F) + (double)(dzplus*dzplus) + (double)((dxplusz + dx)*(dxplusz + dx)*0.25) + (double)((dyplusz + dy)*(dyplusz + dy)*0.25));

//         nminusx= dxminus / sqrt((double)(1.192092896e-07F) + (double)(dxminus*dxminus) + (double)((dyminusx + dy)*(dyminusx + dy)*0.25) + (double)((dzminusx + dz)*(dzminusx + dz)*0.25));
//         nminusy= dyminus / sqrt((double)(1.192092896e-07F) + (double)(dyminus*dyminus) + (double)((dxminusy + dx)*(dxminusy + dx)*0.25) + (double)((dzminusy + dz)*(dzminusy + dz)*0.25));
//         nminusz= dzminus / sqrt((double)(1.192092896e-07F) + (double)(dzminus*dzminus) + (double)((dxminusz + dx)*(dxminusz + dx)*0.25) + (double)((dyminusz + dy)*(dyminusz + dy)*0.25));

//        float curvature= ((nplusx-nminusx)+(nplusy-nminusy)+(nplusz-nminusz))/2;
//        float delta_phi = speedFuction(ind, curvature, NULL);

//        if(delta_phi > 0)
//            delta_phi = delta_phi * gradphimax;
//        else
//            delta_phi = delta_phi * gradphimin;
//}
