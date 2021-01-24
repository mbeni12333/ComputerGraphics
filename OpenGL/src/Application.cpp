#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string>
#include <iostream>

static GLuint compileShader(GLuint type, const std::string& source){

    // create a shader 
    GLuint id = glCreateShader(type);
    // get the adress of the array from the string
    const char* src = source.c_str();
    // set the shader source
    glShaderSource(id, 1, &src, nullptr);
    // compile that source for the shader created
    glCompileShader(id);

    // Error handeling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS ,&result);

    if(result==GL_FALSE){
        
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        // allocate memory on the stack
        char* message = (char*)alloca(length*sizeof(char));
        // get the message
        glGetShaderInfoLog(id, length, &length, message);

        // print the message
        std::cout<<"Failed to compile : "<<(type==GL_VERTEX_SHADER ? "Vertex" : "Fragment")<<std::endl;
        std::cout<<message<<std::endl;

        // remove the useless shader
        glDeleteShader(id);

        // return an error
        return 0;
    }

    // return the created shader
    return id;
}



static GLuint createShader(const std::string& vertexShader, const std::string& fragmentShader){

    // Create a program
    GLuint program = glCreateProgram();

    // Compile shaders
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Attach shaders to the program
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    // Link the program
    glLinkProgram(program);
    glValidateProgram(program);

    // Delete intermediates
    glDeleteShader(vs);
    glDeleteShader(fs);


    return program;
}



float positions[] = {
    -0.5f, -0.5f,
    +0.5f, -0.5f,
    +0.0f, 0.5f
};

void initBuffers(){

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
    


    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location=0) in vec4 position;\n"
        "\n"
        "void main(){\n"
        "   gl_position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location=0) out vec4 color;\n"
        "\n"
        "void main(){\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";


    GLuint program = createShader(vertexShader, fragmentShader);
    glUseProgram(program);
}

void drawScene(){
    glDrawArrays(GL_TRIANGLES, 0, sizeof(positions));
}




int main(void){
    GLFWwindow* window;

    /* Initialize the library */
    if(!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if(!window){
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    GLenum err = glewInit();
    if(GLEW_OK!=err){
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));


    initBuffers();
    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window)){
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        drawScene();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}