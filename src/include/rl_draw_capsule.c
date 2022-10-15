#include "rl_draw_capsule.h"

#warning "C Preprocessor got here!"

void DrawCapsuleEx(Vector3 startPos, Vector3 endPos, float radius, int slices, int rings, Color color){

}

// functions included for now so I can comment and modify them freely

// Draw sphere with extended parameters
void DrawSphereEx_(Vector3 centerPos, float radius, int rings, int slices, Color color)
{
    rlPushMatrix();
        // NOTE: Transformation is applied in inverse order (scale -> translate)
        rlTranslatef(centerPos.x, centerPos.y, centerPos.z);
        rlScalef(radius, radius, radius);

        rlBegin(RL_TRIANGLES);
            rlColor4ub(color.r, color.g, color.b, color.a);

            for (int i = 0; i < (rings + 2); i++)
            {
                for (int j = 0; j < slices; j++)
                {

                    // 6 verts total, 3 pepr tri, together they make a quad (I think lmao)

                    // break down sphere math into slice angle and "taper angle"

                    // this just makes a unit sphere and scales it
                    
                    // compute the four vertices
                    Vector3 w1 = {                                           //s1
                        cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 0))) * sinf(DEG2RAD*(360.0f*(j + 0)/slices)),
                        sinf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 0))),
                        cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 0))) * cosf(DEG2RAD*(360.0f*(j + 0)/slices))
                    };                                                       //c1

                    Vector3 w2 = {
                        cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 1))) * sinf(DEG2RAD*(360.0f*(j + 1)/slices)),
                        sinf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 1))),
                        cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 1))) * cosf(DEG2RAD*(360.0f*(j + 1)/slices))
                    };

                    Vector3 w3 = {
                        cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 1))) * sinf(DEG2RAD*(360.0f*(j + 0)/slices)),
                        sinf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 1))),
                        cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 1))) * cosf(DEG2RAD*(360.0f*(j + 0)/slices))
                    };

                    Vector3 w4 = {
                        cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 0))) * sinf(DEG2RAD*(360.0f*(j + 1)/slices)),
                        sinf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 0))),
                        cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 0))) * cosf(DEG2RAD*(360.0f*(j + 1)/slices))
                    };

                    rlVertex3f(w1.x, w1.y, w1.z);
                    rlVertex3f(w2.x, w2.y, w2.z);
                    rlVertex3f(w3.x, w3.y, w3.z);

                    rlVertex3f(w1.x, w1.y, w1.z);
                    rlVertex3f(w4.x, w4.y, w4.z);
                    rlVertex3f(w2.x, w2.y, w2.z);
                }
            }
        rlEnd();
    rlPopMatrix();
}


// Draw a cylinder with base at startPos and top at endPos
// NOTE: It could be also used for pyramid and cone
void DrawCylinderEx_(Vector3 startPos, Vector3 endPos, float radius, int slices, Color color)
{
    if (slices < 3) slices = 3;

    Vector3 direction = { endPos.x - startPos.x, endPos.y - startPos.y, endPos.z - startPos.z };
    if ((direction.x == 0) && (direction.y == 0) && (direction.z == 0)) return;

    // Construct a basis of the base and the top face:
    Vector3 b0 = Vector3Scale(Vector3Normalize(direction), 1.0f);
    Vector3 b1 = Vector3Normalize(Vector3Perpendicular(direction));
    Vector3 b2 = Vector3Normalize(Vector3CrossProduct(b1, direction));



    // we force the basis to be on the axis (note: does not update position)
    //Vector3 b1 = (Vector3){1.0f, 0.0f, 0.0f}; 
    //Vector3 b2 = (Vector3){0.0f, 0.0f, 1.0f};

    // concerning the basis vectors: for the sphere, 
    // the assumed direction for the sphere is always straight up, i.e. the direction is {0,1,0}


    // TODO: make an arg
    int rings = 10;

    float baseSliceAngle = (2.0f*PI)/slices;
    float baseRingAngle  = PI * 0.5 / rings; 

    static float PI3_2 = PI * 3.0f/2.0f;

    rlBegin(RL_TRIANGLES);
        rlColor4ub(color.r, color.g, color.b, color.a);
        // start cap
        for (int i = 0; i < rings; i++)
        {
            for (int j = 0; j < slices; j++) 
            {

                // as we iterate through the rings they must be places higher above the center (i -> angle growing on unit circle) height we need is sin(angle)
                // the rings must get smaller by the cos(angle)
                // compute the four vertices
                float s1 = sinf(baseSliceAngle*(j + 0))*radius*cosf(baseRingAngle * ( i + 0 ));
                float c1 = cosf(baseSliceAngle*(j + 0))*radius*cosf(baseRingAngle * ( i + 0 ));
                Vector3 w1 = { 
                    startPos.x + radius * sinf(PI3_2 = PI + baseRingAngle * ( i + 0 )) * b0.x /*/ radius * (i + 0)*/ + s1*b1.x + c1*b2.x, // s1*b1.x 
                    startPos.y + radius * sinf(PI3_2 = PI + baseRingAngle * ( i + 0 )) * b0.y  + s1*b1.y + c1*b2.y, 
                    startPos.z + radius * sinf(PI3_2 = PI + baseRingAngle * ( i + 0 )) * b0.z  + s1*b1.z + c1*b2.z  //         c1*b2.z
                };
                float s2 = sinf(baseSliceAngle*(j + 1))*radius*cosf(baseRingAngle * ( i + 0 ));
                float c2 = cosf(baseSliceAngle*(j + 1))*radius*cosf(baseRingAngle * ( i + 0 ));
                Vector3 w2 = { 
                    startPos.x + radius * sinf(PI3_2 = PI + baseRingAngle * ( i + 0 )) * b0.x + s2*b1.x + c2*b2.x, 
                    startPos.y + radius * sinf(PI3_2 = PI + baseRingAngle * ( i + 0 )) * b0.y + s2*b1.y + c2*b2.y, 
                    startPos.z + radius * sinf(PI3_2 = PI + baseRingAngle * ( i + 0 )) * b0.z + s2*b1.z + c2*b2.z 
                };

                float s3 = sinf(baseSliceAngle*(j + 0))*radius*cosf(baseRingAngle * ( i + 1 ));
                float c3 = cosf(baseSliceAngle*(j + 0))*radius*cosf(baseRingAngle * ( i + 1 ));
                Vector3 w3 = { 
                    startPos.x + radius * sinf(PI3_2 = PI + baseRingAngle * ( i + 1 )) * b0.x + s3*b1.x + c3*b2.x, 
                    startPos.y + radius * sinf(PI3_2 = PI + baseRingAngle * ( i + 1 )) * b0.y + s3*b1.y + c3*b2.y, 
                    startPos.z + radius * sinf(PI3_2 = PI + baseRingAngle * ( i + 1 )) * b0.z + s3*b1.z + c3*b2.z 
                };
                float s4 = sinf(baseSliceAngle*(j + 1))*radius*cosf(baseRingAngle * ( i + 1 ));
                float c4 = cosf(baseSliceAngle*(j + 1))*radius*cosf(baseRingAngle * ( i + 1 ));
                Vector3 w4 = { 
                    startPos.x + radius * sinf(PI3_2 = PI + baseRingAngle * ( i + 1 )) * b0.x + s4*b1.x + c4*b2.x, 
                    startPos.y + radius * sinf(PI3_2 = PI + baseRingAngle * ( i + 1 )) * b0.y + s4*b1.y + c4*b2.y, 
                    startPos.z + radius * sinf(PI3_2 = PI + baseRingAngle * ( i + 1 )) * b0.z + s4*b1.z + c4*b2.z 
                };

                rlVertex3f(w1.x, w1.y, w1.z);
                rlVertex3f(w2.x, w2.y, w2.z);
                rlVertex3f(w3.x, w3.y, w3.z);
                
                rlVertex3f(w2.x, w2.y, w2.z);   
                rlVertex3f(w4.x, w4.y, w4.z);  
                rlVertex3f(w3.x, w3.y, w3.z);    

                DrawLine3D(w1, w2, BLACK);
                DrawLine3D(w2, w3, BLACK);
                DrawLine3D(w3, w1, BLACK);

                DrawLine3D(w2, w4, BLACK);
                DrawLine3D(w4, w3, BLACK);
                DrawLine3D(w3, w2, BLACK);
                
            }
        }
        // end cap
        // ...

        // middle
        /*for (int j = 0; j < slices; j++) 
        {
            // compute the four vertices
            float s1 = sinf(baseSliceAngle*(j + 0))*radius;
            float c1 = cosf(baseSliceAngle*(j + 0))*radius;
            
            // just applying a rotation or change of basis matrix? TODO: look more closely into the math here 
            Vector3 w1 = { 
                startPos.x + s1*b1.x + c1*b2.x, // s1*b1.x 
                startPos.y + s1*b1.y + c1*b2.y, 
                startPos.z + s1*b1.z + c1*b2.z  //         c1*b2.z
            };
            float s2 = sinf(baseSliceAngle*(j + 1))*radius;
            float c2 = cosf(baseSliceAngle*(j + 1))*radius;
            Vector3 w2 = { 
                startPos.x + s2*b1.x + c2*b2.x, 
                startPos.y + s2*b1.y + c2*b2.y, 
                startPos.z + s2*b1.z + c2*b2.z 
            };
            float s3 = sinf(baseSliceAngle*(j + 0))*radius;
            float c3 = cosf(baseSliceAngle*(j + 0))*radius;
            Vector3 w3 = { 
                endPos.x + s3*b1.x + c3*b2.x, 
                endPos.y + s3*b1.y + c3*b2.y, 
                endPos.z + s3*b1.z + c3*b2.z 
            };
            float s4 = sinf(baseSliceAngle*(j + 1))*radius;
            float c4 = cosf(baseSliceAngle*(j + 1))*radius;
            Vector3 w4 = { 
                endPos.x + s4*b1.x + c4*b2.x, 
                endPos.y + s4*b1.y + c4*b2.y, 
                endPos.z + s4*b1.z + c4*b2.z 
            };

            // don't need T0 and T3?
            
            //rlVertex3f(startPos.x, startPos.y, startPos.z);     // |
            //rlVertex3f(w2.x, w2.y, w2.z);                       // T0
            //rlVertex3f(w1.x, w1.y, w1.z);                       // |
                                                                    //          w2 x.-----------x startPos
            rlVertex3f(w1.x, w1.y, w1.z);                         // |           |\'.  T0    /
            rlVertex3f(w2.x, w2.y, w2.z);                         // T1          | \ '.     /
            rlVertex3f(w3.x, w3.y, w3.z);                         // |           |T \  '.  /
                                                                    //             | 2 \ T 'x w1
            rlVertex3f(w2.x, w2.y, w2.z);                         // |        w4 x.---\-1-|---x endPos
            rlVertex3f(w4.x, w4.y, w4.z);                         // T2            '.  \  |T3/
            rlVertex3f(w3.x, w3.y, w3.z);                         // |               '. \ | /
                                                                    //                   '.\|/
            //rlVertex3f(endPos.x, endPos.y, endPos.z);           // |                   'x w3
            //rlVertex3f(w3.x, w3.y, w3.z);                       // T3
            //rlVertex3f(w4.x, w4.y, w4.z);                       // |
                                                                    //
        }
        */
    rlEnd();
}
