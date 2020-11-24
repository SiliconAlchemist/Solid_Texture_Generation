//Assignment 02: Trandformations, viewing and projection

#include "utils.h"

int width = 640, height=640;
#define DIVISIONS 100
#define PI 3.14

#define DELTA_ANGLE 3
#define VERTEX_OFFSET_Z 3

int size;
int main(int, char**)
{
    // Setup window
    GLFWwindow *window = setupWindow(width, height);
    ImGuiIO& io = ImGui::GetIO(); // Create IO object

    ImVec4 clearColor = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);


    unsigned int shaderProgram = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");
    glUseProgram(shaderProgram);
    
    // ##################
    GLuint tex;

    if (load_texture("./2d_exemplar.jpeg", &tex)){
        std::cout << "texture loaded successfully !!!!" << std::endl;
    }

    glEnable( GL_CULL_FACE ); // cull face
    glCullFace( GL_BACK );      // cull back face
    glFrontFace( GL_CCW );      // GL_CCW for counter clock-wise



    setupModelTransformation(shaderProgram);
    setupViewTransformation(shaderProgram);
    setupProjectionTransformation(shaderProgram, width , height);
    
    int vVertex_attrib = glGetAttribLocation(shaderProgram, "vVertex");
    if(vVertex_attrib == -1) {
        std::cout << "Could not bind location: vVertex\n" ;
        exit(0);
    }else{
        std::cout << "vVertex found at location " << vVertex_attrib << std::endl;
    }

   
/////////////////////// Create Sphere

    GLuint sphere_VAO, indices_IBO;
    int nTheta = 180/DELTA_ANGLE + 1;
    int nPhi = 360/DELTA_ANGLE + 1;

    float *vertices = new float[nTheta*nPhi*3];
    float *normals = new float[nTheta*nPhi*3];
    float *uv = new float[nTheta*nPhi*2];
    // float *v = new float[nTheta*nPhi];
    float theta, phi, x, y, z;
    float radius = 0.5f;
    for (int j = 0; j<nTheta; j++)
        for(int i=0; i<nPhi; i++)
        {
            theta = float(j*DELTA_ANGLE)*M_PI/180.0;
            phi = float(i*DELTA_ANGLE)*M_PI/180.0;
            x = radius * sinf(theta)*cosf(phi);
            y = radius * sinf(theta)*sinf(phi);
            z = radius * cos(theta);
            normals[(i + j*nPhi)*3 + 0] = x; normals[(i + j*nPhi)*3 + 1] = y; normals[(i + j*nPhi)*3 + 2] = z; 
            vertices[(i + j*nPhi)*3 + 0] = radius*x; vertices[(i + j*nPhi)*3 + 1] = radius*y; vertices[(i + j*nPhi)*3 + 2] = radius*z - VERTEX_OFFSET_Z; 
        }
        
    //Generate index array
    GLushort *indices = new GLushort[2*(nTheta-1)*(nPhi-1)*3];
    for(int j=0; j<(nTheta-1); j++)
        for(int i=0; i<(nPhi-1); i++)
        {
            //Upper triangle
            indices[(i + j*(nPhi-1))*6 + 0] = i + j*nPhi;
            indices[(i + j*(nPhi-1))*6 + 1] = i + (j+1)*nPhi;
            indices[(i + j*(nPhi-1))*6 + 2] = i + 1 + j*nPhi;

            //Lower triangle
            indices[(i + j*(nPhi-1))*6 + 3] = i + 1 + j*nPhi;
            indices[(i + j*(nPhi-1))*6 + 4] = i + (j+1)*nPhi;
            indices[(i + j*(nPhi-1))*6 + 5] = i + 1 + (j+1)*nPhi;
        }

    //Generate vertex buffer and index buffer

    glGenVertexArrays(1, &sphere_VAO);
    glBindVertexArray(sphere_VAO);

    GLuint vertex_VBO, normal_VBO;
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, nTheta*nPhi*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vVertex_attrib);
    glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0); 


    
    glGenBuffers(1, &indices_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (nTheta-1)*(nPhi-1)*6*sizeof(GLushort), indices, GL_STATIC_DRAW);



    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_IBO);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // printf("doing \n");

    for (int j = 0;j<(nTheta); j++)
        for(int i=0; i<(nPhi); i++)
        {
            // compute uv coordinates
            theta = float(j*DELTA_ANGLE)*M_PI/180.0;
            phi = float(i*DELTA_ANGLE)*M_PI/180.0;            
            uv[(i+j*nPhi)*2+0] = ((phi))/(2*M_PI);
            uv[(i+j*nPhi)*2+1] = 1-(theta/M_PI);

            //std::cout<<"theta: "<<theta<<" phi: "<<phi<<" and in deg:"<<float(i*DELTA_ANGLE)<<" u: "<<uv[(i+j*nPhi)*2+0]<<" v: "<<uv[(i+j*nPhi)*2+1]<<std::endl;
        }

    GLuint tex_VBO; //bind texture buffer
    glGenBuffers(1, &tex_VBO);
    int vt = glGetAttribLocation(shaderProgram, "vt");
    if(vt == -1) {
        std::cout << "Could not bind location: vt\n" ;
        exit(0);
    }else{
        std::cout << "vt found at location " << vt << std::endl;
    }
    glBindBuffer(GL_ARRAY_BUFFER, tex_VBO);
    glBufferData(GL_ARRAY_BUFFER, nPhi*nTheta*2*sizeof(float), uv, GL_STATIC_DRAW);
    glVertexAttribPointer(vt, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vt);

    float angle = 0.0f;
    
    GLfloat matrix[] = {
    1.0f, 0.0f, 0.0f, 0.0f, // first column
    0.0f, 1.0f, 0.0f, 0.0f, // second column
    0.0f, 0.0f, 1.0f, 0.0f,// third column
     0.0f, 0.0f, 0.0f, 1.0f
    };

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glUseProgram(shaderProgram);
        showOptionsDialog(shaderProgram, matrix,  angle);

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Information");                          
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(sphere_VAO);
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, tex );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_IBO);
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
         

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }

    // Cleanup
    cleanup(window);

    return 0;
}

