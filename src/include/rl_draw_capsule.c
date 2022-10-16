#include "rl_draw_capsule.h"

#warning "C Preprocessor got here!"

void DrawCapsule(Vector3 startPos, Vector3 endPos, float radius, int slices, int rings, Color color)
{
    if (slices < 3) slices = 3;

    Vector3 direction = { endPos.x - startPos.x, endPos.y - startPos.y, endPos.z - startPos.z };
    
    // draw a sphere if start and end points are the same
    bool sphereCase = (direction.x == 0) && (direction.y == 0) && (direction.z == 0);
    if (sphereCase) direction = (Vector3){0.0f, 1.0f, 0.0f};

    // Construct a basis of the base and the caps:
    Vector3 b0 = Vector3Normalize(direction);
    Vector3 b1 = Vector3Normalize(Vector3Perpendicular(direction));
    Vector3 b2 = Vector3Normalize(Vector3CrossProduct(b1, direction));
    Vector3 capCenter = endPos;

    float baseSliceAngle = (2.0f*PI)/slices;
    float baseRingAngle  = PI * 0.5 / rings; 

    rlBegin(RL_TRIANGLES);
        rlColor4ub(color.r, color.g, color.b, color.a);

        // render both caps
        for (int c = 0; c < 2; c++)
        {
            for (int i = 0; i < rings; i++)
            {
                for (int j = 0; j < slices; j++) 
                {

                    // we build up the rings from capCenter in the direction of the 'direction' vector we computed earlier

                    // as we iterate through the rings they must be placed higher above the center, the height we need is sin(angle(i))
                    // as we iterate through the rings they must get smaller by the cos(angle(i))

                    // compute the four vertices
                    float ringSin1 = sinf(baseSliceAngle*(j + 0))*cosf(baseRingAngle * ( i + 0 ));
                    float ringCos1 = cosf(baseSliceAngle*(j + 0))*cosf(baseRingAngle * ( i + 0 ));
                    Vector3 w1 = (Vector3){ 
                        capCenter.x + (sinf(baseRingAngle * ( i + 0 ))*b0.x + ringSin1*b1.x + ringCos1*b2.x) * radius,
                        capCenter.y + (sinf(baseRingAngle * ( i + 0 ))*b0.y + ringSin1*b1.y + ringCos1*b2.y) * radius, 
                        capCenter.z + (sinf(baseRingAngle * ( i + 0 ))*b0.z + ringSin1*b1.z + ringCos1*b2.z) * radius
                    };
                    float ringSin2 = sinf(baseSliceAngle*(j + 1))*cosf(baseRingAngle * ( i + 0 ));
                    float ringCos2 = cosf(baseSliceAngle*(j + 1))*cosf(baseRingAngle * ( i + 0 ));
                    Vector3 w2 = (Vector3){ 
                        capCenter.x + (sinf(baseRingAngle * ( i + 0 ))*b0.x + ringSin2*b1.x + ringCos2*b2.x) * radius, 
                        capCenter.y + (sinf(baseRingAngle * ( i + 0 ))*b0.y + ringSin2*b1.y + ringCos2*b2.y) * radius, 
                        capCenter.z + (sinf(baseRingAngle * ( i + 0 ))*b0.z + ringSin2*b1.z + ringCos2*b2.z) * radius 
                    };

                    float ringSin3 = sinf(baseSliceAngle*(j + 0))*cosf(baseRingAngle * ( i + 1 ));
                    float ringCos3 = cosf(baseSliceAngle*(j + 0))*cosf(baseRingAngle * ( i + 1 ));
                    Vector3 w3 = (Vector3){ 
                        capCenter.x + (sinf(baseRingAngle * ( i + 1 ))*b0.x + ringSin3*b1.x + ringCos3*b2.x) * radius, 
                        capCenter.y + (sinf(baseRingAngle * ( i + 1 ))*b0.y + ringSin3*b1.y + ringCos3*b2.y) * radius, 
                        capCenter.z + (sinf(baseRingAngle * ( i + 1 ))*b0.z + ringSin3*b1.z + ringCos3*b2.z) * radius 
                    };
                    float ringSin4 = sinf(baseSliceAngle*(j + 1))*cosf(baseRingAngle * ( i + 1 ));
                    float ringCos4 = cosf(baseSliceAngle*(j + 1))*cosf(baseRingAngle * ( i + 1 ));
                    Vector3 w4 = (Vector3){ 
                        capCenter.x + (sinf(baseRingAngle * ( i + 1 ))*b0.x + ringSin4*b1.x + ringCos4*b2.x) * radius, 
                        capCenter.y + (sinf(baseRingAngle * ( i + 1 ))*b0.y + ringSin4*b1.y + ringCos4*b2.y) * radius, 
                        capCenter.z + (sinf(baseRingAngle * ( i + 1 ))*b0.z + ringSin4*b1.z + ringCos4*b2.z) * radius 
                    };

                    // make sure cap triangle normals are facing outwards
                    if(c == 0)
                    {
                        rlVertex3f(w1.x, w1.y, w1.z);
                        rlVertex3f(w2.x, w2.y, w2.z);
                        rlVertex3f(w3.x, w3.y, w3.z);
                        
                        rlVertex3f(w2.x, w2.y, w2.z);   
                        rlVertex3f(w4.x, w4.y, w4.z);  
                        rlVertex3f(w3.x, w3.y, w3.z); 
                    }
                    else
                    {
                        rlVertex3f(w1.x, w1.y, w1.z);
                        rlVertex3f(w3.x, w3.y, w3.z);
                        rlVertex3f(w2.x, w2.y, w2.z);

                        rlVertex3f(w2.x, w2.y, w2.z);  
                        rlVertex3f(w3.x, w3.y, w3.z);  
                        rlVertex3f(w4.x, w4.y, w4.z);     
                    }
                }
            }
            capCenter = startPos;
            b0 = Vector3Scale(b0, -1.0f);
        }
        // render middle
        if (!sphereCase)
        {
            for (int j = 0; j < slices; j++) 
            {
                // compute the four vertices
                float ringSin1 = sinf(baseSliceAngle*(j + 0))*radius;
                float ringCos1 = cosf(baseSliceAngle*(j + 0))*radius;
                Vector3 w1 = { 
                    startPos.x + ringSin1*b1.x + ringCos1*b2.x,
                    startPos.y + ringSin1*b1.y + ringCos1*b2.y, 
                    startPos.z + ringSin1*b1.z + ringCos1*b2.z 
                };
                float ringSin2 = sinf(baseSliceAngle*(j + 1))*radius;
                float ringCos2 = cosf(baseSliceAngle*(j + 1))*radius;
                Vector3 w2 = { 
                    startPos.x + ringSin2*b1.x + ringCos2*b2.x, 
                    startPos.y + ringSin2*b1.y + ringCos2*b2.y, 
                    startPos.z + ringSin2*b1.z + ringCos2*b2.z 
                };

                float ringSin3 = sinf(baseSliceAngle*(j + 0))*radius;
                float ringCos3 = cosf(baseSliceAngle*(j + 0))*radius;
                Vector3 w3 = { 
                    endPos.x + ringSin3*b1.x + ringCos3*b2.x, 
                    endPos.y + ringSin3*b1.y + ringCos3*b2.y, 
                    endPos.z + ringSin3*b1.z + ringCos3*b2.z 
                };
                float ringSin4 = sinf(baseSliceAngle*(j + 1))*radius;
                float ringCos4 = cosf(baseSliceAngle*(j + 1))*radius;
                Vector3 w4 = { 
                    endPos.x + ringSin4*b1.x + ringCos4*b2.x, 
                    endPos.y + ringSin4*b1.y + ringCos4*b2.y, 
                    endPos.z + ringSin4*b1.z + ringCos4*b2.z 
                };
                                                                        //          w2 x.-----------x startPos
                rlVertex3f(w1.x, w1.y, w1.z);                         // |           |\'.  T0    /
                rlVertex3f(w2.x, w2.y, w2.z);                         // T1          | \ '.     /
                rlVertex3f(w3.x, w3.y, w3.z);                         // |           |T \  '.  /
                                                                        //             | 2 \ T 'x w1
                rlVertex3f(w2.x, w2.y, w2.z);                         // |        w4 x.---\-1-|---x endPos
                rlVertex3f(w4.x, w4.y, w4.z);                         // T2            '.  \  |T3/
                rlVertex3f(w3.x, w3.y, w3.z);                         // |               '. \ | /
                                                                        //                   '.\|/
                                                                        //                   'x w3
            }
        }
    rlEnd();
}


void DrawCapsuleWires(Vector3 startPos, Vector3 endPos, float radius, int slices, int rings, Color color)
{
    if (slices < 3) slices = 3;

    Vector3 direction = { endPos.x - startPos.x, endPos.y - startPos.y, endPos.z - startPos.z };

    // draw a sphere if start and end points are the same
    bool sphereCase = (direction.x == 0) && (direction.y == 0) && (direction.z == 0);
    if (sphereCase) direction = (Vector3){0.0f, 1.0f, 0.0f};

    // Construct a basis of the base and the caps:
    Vector3 b0 = Vector3Normalize(direction);
    Vector3 b1 = Vector3Normalize(Vector3Perpendicular(direction));
    Vector3 b2 = Vector3Normalize(Vector3CrossProduct(b1, direction));
    Vector3 capCenter = endPos;

    float baseSliceAngle = (2.0f*PI)/slices;
    float baseRingAngle  = PI * 0.5 / rings; 

    rlBegin(RL_LINES);
        rlColor4ub(color.r, color.g, color.b, color.a);

        // render both caps
        for (int c = 0; c < 2; c++)
        {
            for (int i = 0; i < rings; i++)
            {
                for (int j = 0; j < slices; j++) 
                {

                    // we build up the rings from capCenter in the direction of the 'direction' vector we computed earlier

                    // as we iterate through the rings they must be placed higher above the center, the height we need is sin(angle(i))
                    // as we iterate through the rings they must get smaller by the cos(angle(i))

                    // compute the four vertices
                    float ringSin1 = sinf(baseSliceAngle*(j + 0))*cosf(baseRingAngle * ( i + 0 ));
                    float ringCos1 = cosf(baseSliceAngle*(j + 0))*cosf(baseRingAngle * ( i + 0 ));
                    Vector3 w1 = (Vector3){ 
                        capCenter.x + (sinf(baseRingAngle * ( i + 0 ))*b0.x + ringSin1*b1.x + ringCos1*b2.x) * radius,
                        capCenter.y + (sinf(baseRingAngle * ( i + 0 ))*b0.y + ringSin1*b1.y + ringCos1*b2.y) * radius, 
                        capCenter.z + (sinf(baseRingAngle * ( i + 0 ))*b0.z + ringSin1*b1.z + ringCos1*b2.z) * radius
                    };
                    float ringSin2 = sinf(baseSliceAngle*(j + 1))*cosf(baseRingAngle * ( i + 0 ));
                    float ringCos2 = cosf(baseSliceAngle*(j + 1))*cosf(baseRingAngle * ( i + 0 ));
                    Vector3 w2 = (Vector3){ 
                        capCenter.x + (sinf(baseRingAngle * ( i + 0 ))*b0.x + ringSin2*b1.x + ringCos2*b2.x) * radius, 
                        capCenter.y + (sinf(baseRingAngle * ( i + 0 ))*b0.y + ringSin2*b1.y + ringCos2*b2.y) * radius, 
                        capCenter.z + (sinf(baseRingAngle * ( i + 0 ))*b0.z + ringSin2*b1.z + ringCos2*b2.z) * radius 
                    };

                    float ringSin3 = sinf(baseSliceAngle*(j + 0))*cosf(baseRingAngle * ( i + 1 ));
                    float ringCos3 = cosf(baseSliceAngle*(j + 0))*cosf(baseRingAngle * ( i + 1 ));
                    Vector3 w3 = (Vector3){ 
                        capCenter.x + (sinf(baseRingAngle * ( i + 1 ))*b0.x + ringSin3*b1.x + ringCos3*b2.x) * radius, 
                        capCenter.y + (sinf(baseRingAngle * ( i + 1 ))*b0.y + ringSin3*b1.y + ringCos3*b2.y) * radius, 
                        capCenter.z + (sinf(baseRingAngle * ( i + 1 ))*b0.z + ringSin3*b1.z + ringCos3*b2.z) * radius 
                    };
                    float ringSin4 = sinf(baseSliceAngle*(j + 1))*cosf(baseRingAngle * ( i + 1 ));
                    float ringCos4 = cosf(baseSliceAngle*(j + 1))*cosf(baseRingAngle * ( i + 1 ));
                    Vector3 w4 = (Vector3){ 
                        capCenter.x + (sinf(baseRingAngle * ( i + 1 ))*b0.x + ringSin4*b1.x + ringCos4*b2.x) * radius, 
                        capCenter.y + (sinf(baseRingAngle * ( i + 1 ))*b0.y + ringSin4*b1.y + ringCos4*b2.y) * radius, 
                        capCenter.z + (sinf(baseRingAngle * ( i + 1 ))*b0.z + ringSin4*b1.z + ringCos4*b2.z) * radius 
                    };

                    rlVertex3f(w1.x, w1.y, w1.z);
                    rlVertex3f(w2.x, w2.y, w2.z);

                    rlVertex3f(w2.x, w2.y, w2.z);
                    rlVertex3f(w3.x, w3.y, w3.z);

                    rlVertex3f(w1.x, w1.y, w1.z);
                    rlVertex3f(w3.x, w3.y, w3.z);
                    
                    rlVertex3f(w2.x, w2.y, w2.z);   
                    rlVertex3f(w4.x, w4.y, w4.z); 

                    rlVertex3f(w3.x, w3.y, w3.z);
                    rlVertex3f(w4.x, w4.y, w4.z); 
                }
            }
            capCenter = startPos;
            b0 = Vector3Scale(b0, -1.0f);
        }
        // render middle
        if (!sphereCase)
        {
            for (int j = 0; j < slices; j++) 
            {
                // compute the four vertices
                float ringSin1 = sinf(baseSliceAngle*(j + 0))*radius;
                float ringCos1 = cosf(baseSliceAngle*(j + 0))*radius;
                Vector3 w1 = { 
                    startPos.x + ringSin1*b1.x + ringCos1*b2.x,
                    startPos.y + ringSin1*b1.y + ringCos1*b2.y, 
                    startPos.z + ringSin1*b1.z + ringCos1*b2.z 
                };
                float ringSin2 = sinf(baseSliceAngle*(j + 1))*radius;
                float ringCos2 = cosf(baseSliceAngle*(j + 1))*radius;
                Vector3 w2 = { 
                    startPos.x + ringSin2*b1.x + ringCos2*b2.x, 
                    startPos.y + ringSin2*b1.y + ringCos2*b2.y, 
                    startPos.z + ringSin2*b1.z + ringCos2*b2.z 
                };

                float ringSin3 = sinf(baseSliceAngle*(j + 0))*radius;
                float ringCos3 = cosf(baseSliceAngle*(j + 0))*radius;
                Vector3 w3 = { 
                    endPos.x + ringSin3*b1.x + ringCos3*b2.x, 
                    endPos.y + ringSin3*b1.y + ringCos3*b2.y, 
                    endPos.z + ringSin3*b1.z + ringCos3*b2.z 
                };
                float ringSin4 = sinf(baseSliceAngle*(j + 1))*radius;
                float ringCos4 = cosf(baseSliceAngle*(j + 1))*radius;
                Vector3 w4 = { 
                    endPos.x + ringSin4*b1.x + ringCos4*b2.x, 
                    endPos.y + ringSin4*b1.y + ringCos4*b2.y, 
                    endPos.z + ringSin4*b1.z + ringCos4*b2.z 
                };

                rlVertex3f(w1.x, w1.y, w1.z); 
                rlVertex3f(w3.x, w3.y, w3.z);

                rlVertex3f(w2.x, w2.y, w2.z); 
                rlVertex3f(w4.x, w4.y, w4.z); 

                rlVertex3f(w2.x, w2.y, w2.z); 
                rlVertex3f(w3.x, w3.y, w3.z);
            }
        }
    rlEnd();
}
