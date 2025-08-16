#include "main.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void processInput(GLFWwindow *window, float deltaTime, ecs_world_t *world){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    double xpos, ypos;
    MousePosition *pos = ecs_singleton_get_mut(world, MousePosition);

    glfwGetCursorPos(window, &xpos, &ypos);
    int winWidth, winHeight;
    glfwGetWindowSize(window, &winWidth, &winHeight);
    pos->x = xpos * (double)WIDTH / winWidth;
    pos->y = ypos * (double)HEIGHT / winHeight;
    pos->y = HEIGHT - pos->y;

    MouseButtonState *mb = ecs_singleton_get_mut(world, MouseButtonState);
    mb->leftDown = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
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


void layoutGameStateSystem(ecs_iter_t *it){
    Position *pos = ecs_field(it, Position, 0);
    Size *s = ecs_field(it, Size, 1);
    Rotation *r = ecs_field(it, Rotation, 2);
    Owner *owner = ecs_field(it, Owner, 3);
    Index *index = ecs_field(it, Index, 4);
    Zone *zone = ecs_field(it, Zone, 5);
    const HandSizes *handSizes = ecs_singleton_get(it->world, HandSizes);
    const MousePosition *mousePos = ecs_singleton_get(it->world, MousePosition);
    const BoardSizes *boardSizes = ecs_singleton_get(it->world, BoardSizes);

    for(int i = 0; i < it->count; i++){
        if(zone[i].zone == ZONE_HAND){
            int playerId = owner[i].playerId;
            int handSize = handSizes->playerHandSize[playerId];
            float handCenterX = WIDTH / 2.0f;
            float handCenterY = (playerId == PLAYER0) ? 180.0f : 1000.0f;
            int playerSign = (playerId == PLAYER0) ? 1 : -1;

            float radius = 1000.0f;
            float maxAngleDeg = 30.0f;
            float middle = (float)(handSize) / 2.0f;


            float angleDeg = ((index[i].index - middle) / middle) * (maxAngleDeg / 2.0f);
            float angleRad = glm_rad(angleDeg);


            float xpos = handCenterX + (sinf(angleRad) * radius) * playerSign;
            float ypos = handCenterY - (((1 - cosf(angleRad)) * radius)) * playerSign;

            s[i].width = 100.0f;
            s[i].height = 150.0f;

            if(ecs_has(it->world, it->entities[i], IsHovering)){
                s[i].width *= 2;
                s[i].height *= 2;
            }

            if(!ecs_has(it->world, it->entities[i], IsDragging)) {
                pos[i].x = xpos;
                pos[i].y = ypos;
            }
            else{
                pos[i].x = mousePos[i].x - s[i].width / 2 * playerSign;
                pos[i].y = mousePos[i].y - s[i].height / 2 * playerSign;
            }


            r[i].angle = -angleDeg;
            r[i].angle += 180.0f * playerId;
        }
        else if(zone[i].zone == ZONE_BOARD){
            int playerId = owner[i].playerId;
            int unitCount = boardSizes->playerBoardSize[playerId];
            float unitWidth = 150.0f;
            float unitHeight = 100.0f;

            //compensating for the rotation
            float boardLeftX  = WIDTH * 0.2f - ((playerId == PLAYER0) ? unitWidth : 0);
            float boardRightX = WIDTH * 0.8f - ((playerId == PLAYER0) ? unitWidth : 0);
            float boardY      = HEIGHT * (playerId == PLAYER0 ? 0.35f : 0.65f);

            float slotWidth = (boardRightX - boardLeftX) / (float)unitCount;
            float centerX = boardLeftX + index[i].index * slotWidth + slotWidth /2;

            s[i].width = unitWidth;
            s[i].height = unitHeight;

            pos[i].x = centerX;
            pos[i].y = boardY;
            r[i].angle = (playerId == PLAYER0) ? 0 : 180.0f;
        }
    }

}


unsigned int setupCard(){
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


void drawCard(unsigned int cardProgram, unsigned int cardVAO, unsigned int cardTexture, float xpos, float ypos, float xscale, float yscale, float rotation){
    mat4 model;
    vec3 translate = {xpos, ypos, 0.0f};
    glm_mat4_identity(model);
    glm_translate(model, translate);
    glm_rotate_z(model, glm_rad(rotation), model);
    glm_scale(model, (vec3){xscale, yscale, 1.0f});
    glUseProgram(cardProgram);
    glBindTexture(GL_TEXTURE_2D, cardTexture);

    glUniformMatrix4fv(glGetUniformLocation(cardProgram, "model"), 1, GL_FALSE, (float *)model);

    glBindVertexArray(cardVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void drawGameStateSystem(ecs_iter_t *it){
    Position *pos = ecs_field(it, Position, 0);
    Size *s = ecs_field(it, Size, 1);
    Rotation *r = ecs_field(it, Rotation, 2);
    Render *render = ecs_field(it, Render, 3);
    for(int i = 0; i < it->count; i++){
        drawCard(render[i].shaderProgram, render[i].vao, render[i].texture, pos[i].x, pos[i].y, s[i].width, s[i].height, r[i].angle);
    }
}


void initMouseECS(ecs_world_t *world){

    ecs_singleton_set(world, MousePosition, {0, 0});
    ecs_singleton_set(world, MouseButtonState, {0});

}


ecs_world_t *initWorldECS(){
    ecs_world_t *world = ecs_init();

    // Import the module. This runs the componentsImport function
    // and registers everything correctly.
    ECS_IMPORT(world, components);

    return world;
}


void initGameStateECS(ecs_world_t *world){
    HandSizes hs = {{0, 0}};
    ecs_singleton_set_ptr(world, HandSizes, &hs);
    BoardSizes bs = {{0, 0}};
    ecs_singleton_set_ptr(world, BoardSizes, &bs);
    unsigned int cardVAO = setupCard();
    unsigned int cardTexture = genTexture("textures/Orca.png");
    unsigned int cardProgram = linkShaders("shaders/cardVertex.glsl", "shaders/cardFragments.glsl");

    glUseProgram(cardProgram);

    glUniform1i(glGetUniformLocation(cardProgram, "cardTexture"), 0);
    mat4 projection;
    glm_ortho(0.0f, WIDTH, 0.0f, HEIGHT, -1.0f, 1.0f, projection);
    glUniformMatrix4fv(glGetUniformLocation(cardProgram, "projection"), 1, GL_FALSE, (float*) projection);

    for(int playerId = 0; playerId < 2; playerId++){
        for(int i = 0; i < 2; i++){
            initUnitECS(world, (ManaCost){0}, (Name){"OrcaUnit"}, (ArtPath){""}, (Rarity){0}, (EffectText){""}, (Health){10}, (Attack){10}, (Owner){playerId}, (Index){i}, (Render){cardProgram, cardVAO, cardTexture}, (Zone){ZONE_BOARD});
            initCardECS(world, (ManaCost){0}, (Name){"Orca"}, (ArtPath){""}, (Rarity){0}, (EffectText){""}, (Health){10}, (Attack){10}, (CardType){0}, (Owner){playerId}, (Index){i}, (Render){cardProgram, cardVAO, cardTexture}, (Zone){ZONE_HAND});
        }
    }
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

    ecs_world_t *world = initWorldECS();
    initMouseECS(world);
    initGameStateECS(world);

    ECS_SYSTEM(world, ProcessPlayerInputSystem, EcsOnUpdate, components.Position, components.Size, components.Rotation, components.Owner);
    ECS_SYSTEM(world, drawGameStateSystem, EcsOnUpdate, components.Position, components.Size, components.Rotation, components.Render);
    ECS_SYSTEM(world, layoutGameStateSystem, EcsOnUpdate, components.Position, components.Size, components.Rotation, components.Owner, components.Index, components.Zone);

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

    unsigned int backGroundTexture = genTexture("textures/Board2.png");
    glUniform1i(glGetUniformLocation(backGroundProgram, "backGroundTexture"), 0);
    glBindTexture(GL_TEXTURE_2D, backGroundTexture);

    while(!glfwWindowShouldClose(window)){
        //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window, 0, world);

        glUseProgram(backGroundProgram);
        glBindTexture(GL_TEXTURE_2D, backGroundTexture);
        glBindVertexArray(backGroundVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        ecs_progress(world, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

}
