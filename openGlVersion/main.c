#include "main.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void processInput(GLFWwindow *window, float deltaTime){
    DataWrapper *dataWrapper = glfwGetWindowUserPointer(window);
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        dataWrapper->isClick = 1;
    }
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
        dataWrapper->isClick = 0;
    }
}


unsigned int genTexture(char *fullPath){

    stbi_set_flip_vertically_on_load(true);

    unsigned int texture;

    int width, height, nChannels;
    unsigned char *data = stbi_load(fullPath, &width, &height, &nChannels, 0);
    if(data == NULL){
        printf("ERROR: Failed to load texture %s\n", fullPath);
        return 0;
    }

    glGenTextures(1, &texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, texture);

    GLenum format = GL_RGB;
    if(nChannels == 1) format = GL_RED;
    else if(nChannels == 4) format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return texture;
}


void framebufferSizeCallback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}


char *readShaderFile(const char *fileName){
    FILE* file = fopen(fileName, "rb");
    if(file == NULL){
        printf("uhhhhhhhhhhhhhhhhhhhhh he's right behind isn't he?\n");
    }
    fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
    char *fileText = malloc(sizeof(char) * (size + 1));

    fread(fileText, 1, size,file);
    fileText[size] = '\0';

    fclose(file);
    return fileText;
}


unsigned int linkShaders(const char *vertexFileName, const char *fragmentFileName){
    const GLchar *vertexFileText = readShaderFile(vertexFileName);
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexFileText, NULL);
    glCompileShader(vertexShader);

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n",infoLog);
    }
    free((char *)vertexFileText);

    unsigned int fragmentShader;
    const GLchar *fragmentFileText = readShaderFile(fragmentFileName);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentFileText, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    free((char *)fragmentFileText);

    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n",infoLog);
        return 0;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::LINKING_FAILED\n%s\n",infoLog);
        return 0;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


unsigned int setupCard(unsigned int cardProgram){
    float cardVertices[] = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f
    };

    unsigned int cardVAO;
    glGenVertexArrays(1, &cardVAO);
    unsigned int cardVBO;
    glGenBuffers(1, &cardVBO);  //Creates buffer for array data
    glBindVertexArray(cardVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cardVBO);  //binds buffer to the GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(cardVertices), cardVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);  //telling the openGL state box how to read the inputVector
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));  //telling the openGL state box how to read the inputVector
    glEnableVertexAttribArray(1);

    return cardVAO;
}


void drawCard(unsigned int cardProgram, unsigned int cardVAO, float x, float y){
    mat4 model;
    vec3 translate = {x, y, 0.0f};
    glm_mat4_identity(model);
    glm_translate(model, translate);
    glm_scale(model, (vec3){200.0f, 300.0f, 1.0f});

    glUniformMatrix4fv(glGetUniformLocation(cardProgram, "model"), 1, GL_FALSE, (float *)model);

    glBindVertexArray(cardVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LoR Clone", NULL, NULL);
    if(window == NULL){
        printf("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("GLAD failed\n");
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    DataWrapper dataWrapper = {
        .isClick = 0
    };

    glfwSetWindowUserPointer(window, &dataWrapper);

    float backGroundVertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f
    };
    unsigned int backGroundVAO;
    glGenVertexArrays(1, &backGroundVAO);
    unsigned int backGroundVBO;
    glGenBuffers(1, &backGroundVBO);  //Creates buffer for array data
    glBindVertexArray(backGroundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, backGroundVBO);  //binds buffer to the GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(backGroundVertices), backGroundVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);  //telling the openGL state box how to read the inputVector
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));  //telling the openGL state box how to read the inputVector
    glEnableVertexAttribArray(1);

    unsigned int backGroundProgram = linkShaders("shaders/backGroundVertex.glsl", "shaders/backGroundFragments.glsl");
    glUseProgram(backGroundProgram);

    unsigned int backGroundTexture = genTexture("textures/Board1.png");
    glUniform1i(glGetUniformLocation(backGroundProgram, "backGroundTexture"), 0);
    glBindTexture(GL_TEXTURE_2D, backGroundTexture);

    unsigned int cardProgram = linkShaders("shaders/cardVertex.glsl", "shaders/cardFragments.glsl");
    glUseProgram(cardProgram);
    mat4 projection;
    glm_ortho(0.0f, WIDTH, 0.0f, HEIGHT, -1.0f, 1.0f, projection);
    unsigned int cardVAO = setupCard(cardProgram);
    unsigned int cardTexture = genTexture("textures/Orca.png");
    glUniformMatrix4fv(glGetUniformLocation(cardProgram, "projection"), 1, GL_FALSE, (float*) projection);
    glUniform1i(glGetUniformLocation(cardProgram, "cardTexture"), 0);
    glBindTexture(GL_TEXTURE_2D, cardTexture);

    double xpos, ypos;

    while(!glfwWindowShouldClose(window)){
        //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window, 0);
        glfwGetCursorPos(window, &xpos, &ypos);
        printf("dataWrapper->isClick: %d\n", dataWrapper.isClick);

        glUseProgram(backGroundProgram);
        glBindTexture(GL_TEXTURE_2D, backGroundTexture);
        glBindVertexArray(backGroundVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glUseProgram(cardProgram);
        glBindTexture(GL_TEXTURE_2D, cardTexture);
        drawCard(cardProgram, cardVAO, 200.0f, 240.0f);
        drawCard(cardProgram, cardVAO, 400.0f, 240.0f);
        drawCard(cardProgram, cardVAO, 600.0f, 240.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}
