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
            int playerId = owner[i].playerId;
            int playerSign = (playerId == PLAYER0) ? 1 : -1;
        if(zone[i].zone == ZONE_HAND){
            int handSize = handSizes->playerHandSize[playerId];
            float handCenterX = WIDTH / 2.0f;
            float handCenterY = (playerId == PLAYER0) ? 80.0f : 1000.0f;

            //float radius = 1000.0f;
            float radius = handSize * 200.0f;
            float maxAngleDeg = 30.0f;
            float middle = (float)(handSize) / 2.0f;


            float angleDeg = ((index[i].index - middle) / middle) * (maxAngleDeg / 2.0f);
            float angleRad = glm_rad(angleDeg);


            float xpos = handCenterX + (sinf(angleRad) * radius) * playerSign;
            float ypos = handCenterY - (((1 - cosf(angleRad)) * radius)) * playerSign;

            s[i].width = 100.0f;
            s[i].height = 150.0f;

            if(ecs_has(it->world, it->entities[i], IsHovering)){
                s[i].width *= 1.5f;
                s[i].height *= 1.5f;
            }

            r[i].angle = -angleDeg;
            if(!ecs_has(it->world, it->entities[i], IsDragging)) {
                pos[i].x = xpos;
                pos[i].y = ypos;
            }
            else{
                pos[i].x = mousePos[i].x - s[i].width / 2 * playerSign;
                pos[i].y = mousePos[i].y - s[i].height / 2 * playerSign;
                r[i].angle = 0;
            }

            if(handSize == 1){
                r[i].angle = 0;
            }

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

            if(ecs_has(it->world, it->entities[i], IsHovering)){
                s[i].width *= 1.5f;
                s[i].height *= 1.5f;
            }

            if(!ecs_has(it->world, it->entities[i], IsDragging)){
                pos[i].x = centerX;
                pos[i].y = boardY;
            }
            else{
                pos[i].x = mousePos[i].x - s[i].width / 2 * playerSign;
                pos[i].y = mousePos[i].y - s[i].height / 2 * playerSign;
            }
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


unsigned int setupBackGround(){
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

    return backGroundVAO;
}


void drawCard(unsigned int cardProgram, unsigned int cardVAO, unsigned int cardTexture, float xpos, float ypos, float xscale, float yscale, float rotation, float localYOffset){
    mat4 model;
    vec3 translate = {xpos, ypos, 0.0f};
    glm_mat4_identity(model);
    glm_translate(model, translate);
    glm_rotate_z(model, glm_rad(rotation), model);
    glm_translate(model, (vec3){0.0f, localYOffset, 0.0f});
    glm_scale(model, (vec3){xscale, yscale, 1.0f});
    glUseProgram(cardProgram);
    glBindTexture(GL_TEXTURE_2D, cardTexture);

    glUniformMatrix4fv(glGetUniformLocation(cardProgram, "model"), 1, GL_FALSE, (float *)model);

    glBindVertexArray(cardVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


//Do note that this only does numbers for now
void renderText(unsigned int VAO, unsigned int VBO, unsigned int shaderProgram, Character *glyphMap, char *text, float x, float y, float scale, float rotation, vec3 textColor, float localXOffset, float localYOffset){

    glUseProgram(shaderProgram);
    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, (vec3){x, y, 0.0f});
    glm_rotate_z(model, glm_rad(rotation), model);
    glm_translate(model, (vec3){localXOffset, localYOffset, 0.0f});
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, (float *)model);

    glUniform3f(glGetUniformLocation(shaderProgram, "textColor"), textColor[0], textColor[1], textColor[2]);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float cursorX = 0.0f;

    for(int i = 0; text[i] != '\0'; i++){
        Character ch = glyphMap[text[i] - '0'];

        //float xpos = x + ch.bearing[0] * scale;
        //float ypos = y + (ch.size[1] - ch.bearing[1]) * scale;
        float xpos = cursorX + ch.bearing[0] * scale;
        float ypos = 0.0f + (ch.size[1] - ch.bearing[1]) * scale;

        float w = ch.size[0] * scale;
        float h = ch.size[1] * scale;
        float vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},
            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f},
        };
        glBindTexture(GL_TEXTURE_2D, ch.textureId);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //advance is in 1/64 pixels, don't ask me why.
        cursorX += (ch.advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void initFontECS(Character *renderChars){
    FT_Library ft;
    if(FT_Init_FreeType(&ft)){
        printf("FreeType font engine failed to start");
        return;
    }
    FT_Face face;
    if(FT_New_Face(ft, "fonts/Cinzel-VariableFont_wght.ttf", 0, &face)){
       printf("font failed to init");
       return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48); // choose a height that makes sense for your game

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // no byte-alignment restriction
    for(int i = 0; i < 10; i++){
        if(FT_Load_Char(face, i + '0', FT_LOAD_RENDER)){
            printf("Failed to load Glyph: %c\n", i + '0');
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        renderChars[i].textureId = texture;
        renderChars[i].size[0] = face->glyph->bitmap.width;
        renderChars[i].size[1] = face->glyph->bitmap.rows;
        renderChars[i].bearing[0] = face->glyph->bitmap_left;
        renderChars[i].bearing[1] = face->glyph->bitmap_top;
        renderChars[i].advance = face->glyph->advance.x;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}


void drawFullEntity(Render render, Position pos, Size size, Rotation r, Health health, const RenderText *renderTextComponent, float textScale, vec3 textColor){
    //Creating buffer for health text and loading it
    char buffer[3];
    snprintf(buffer, sizeof(buffer), "%d", health.health);

    //Rendering card template
    drawCard(render.shaderProgram, render.vao, render.texture, pos.x, pos.y, size.width, size.height, r.angle, 0.0);

    //Rendering spalsh art
    drawCard(render.shaderProgram, render.vao, render.splashArtTexture, pos.x, pos.y, size.width, size.height / 2, r.angle, size.height / 2);

    //Rendering Health Text
    renderText(renderTextComponent->VAO, renderTextComponent->VBO, renderTextComponent->shaderProgram, (Character *)renderTextComponent->text, buffer, pos.x, pos.y, 0.5f * textScale, r.angle, textColor, size.width / 20, size.height / 20);

}


void drawCardsAndUnitsSystem(ecs_iter_t *it){
    Position *pos = ecs_field(it, Position, 0);
    Size *s = ecs_field(it, Size, 1);
    Rotation *r = ecs_field(it, Rotation, 2);
    Render *render = ecs_field(it, Render, 3);
    Health *health = ecs_field(it, Health, 4);

    const RenderText *renderTextComponent = ecs_singleton_get(it->world, RenderText);

    vec3 textColor = {1.0f, 1.0f, 1.0f};


    for(int i = 0; i < it->count; i++){
        float textScale = 1.0f;
        if(ecs_has_id(it->world, it->entities[i], IsHovering)){
            textScale = 1.5f;
        }
        drawFullEntity(render[i], pos[i], s[i], r[i], health[i], renderTextComponent, textScale, textColor);
    }

    renderText(renderTextComponent->VAO, renderTextComponent->VBO, renderTextComponent->shaderProgram, (Character *)renderTextComponent->text, "20", 1700.0f, 400.0f, 1.0, 0.0f, textColor, 0, 0);


}


void initMouseECS(ecs_world_t *world){

    ecs_singleton_set(world, MousePosition, {0, 0});
    ecs_singleton_set(world, MouseButtonState, {0});

}


void initTextECS(ecs_world_t *world){
    RenderText renderTextComponent;
    initFontECS(renderTextComponent.text);

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6*4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    renderTextComponent.VAO = VAO;
    renderTextComponent.VBO = VBO;
    renderTextComponent.shaderProgram = linkShaders("shaders/charVertex.glsl", "shaders/charFragments.glsl");

    glUseProgram(renderTextComponent.shaderProgram);
    glUniform1i(glGetUniformLocation(renderTextComponent.shaderProgram, "text"), 0);
    mat4 projection;
    glm_ortho(0.0f, WIDTH, 0.0f, HEIGHT, -1.0f, 1.0f, projection);
    glUniformMatrix4fv(glGetUniformLocation(renderTextComponent.shaderProgram, "projection"), 1, GL_FALSE, (float*) projection);

    ecs_singleton_set_ptr(world, RenderText, &renderTextComponent);
}


void initGameStateECS(ecs_world_t *world){
    HandSizes hs = {{0, 0}};
    ecs_singleton_set_ptr(world, HandSizes, &hs);
    BoardSizes bs = {{0, 0}};
    ecs_singleton_set_ptr(world, BoardSizes, &bs);
    EntitySelectedState es = {0};
    ecs_singleton_set_ptr(world, EntitySelectedState, &es);

    unsigned int cardVAO = setupCard();
    unsigned int cardTexture = genTexture("textures/cardTemplate.png");
    unsigned int splashArtTexture = genTexture("textures/orcaSplashArt.png");
    unsigned int splashArtPenguin = genTexture("textures/penguin.png");
    unsigned int cardProgram = linkShaders("shaders/cardVertex.glsl", "shaders/cardFragments.glsl");

    glUseProgram(cardProgram);

    glUniform1i(glGetUniformLocation(cardProgram, "cardTexture"), 0);
    mat4 projection;
    glm_ortho(0.0f, WIDTH, 0.0f, HEIGHT, -1.0f, 1.0f, projection);
    glUniformMatrix4fv(glGetUniformLocation(cardProgram, "projection"), 1, GL_FALSE, (float*) projection);

    for(int playerId = 0; playerId < 2; playerId++){
        for(int i = 0; i < 6; i++){
            initUnitECS(world, (ManaCost){0}, (Name){"OrcaUnit"}, (ArtPath){""}, (Rarity){0}, (EffectText){""}, (Health){10}, (Attack){10}, (Owner){playerId}, (Index){i}, (Render){cardProgram, cardVAO, cardTexture, splashArtTexture}, (Zone){ZONE_BOARD});
            initCardECS(world, (ManaCost){0}, (Name){"Orca"}, (ArtPath){""}, (Rarity){0}, (EffectText){""}, (Health){10}, (Attack){10}, (CardType){0}, (Owner){playerId}, (Index){i}, (Render){cardProgram, cardVAO, cardTexture, splashArtTexture}, (Zone){ZONE_HAND});
        }
    }
    initCardECS(world, (ManaCost){0}, (Name){"Penguin"}, (ArtPath){""}, (Rarity){0}, (EffectText){""}, (Health){5}, (Attack){5}, (CardType){0}, (Owner){PLAYER0}, (Index){6}, (Render){cardProgram, cardVAO, cardTexture, splashArtPenguin}, (Zone){ZONE_HAND});
}


ecs_world_t *initWorldECS(){
    ecs_world_t *world = ecs_init();

    // Import the module. This runs the componentsImport function
    // and registers everything correctly.
    ECS_IMPORT(world, components);
    initMouseECS(world);
    initGameStateECS(world);
    initTextECS(world);

    ECS_SYSTEM(world, ProcessPlayerInputSystem, EcsOnUpdate, components.Position, components.Size, components.Rotation, components.Owner);
    ECS_SYSTEM(world, drawCardsAndUnitsSystem, EcsOnUpdate, components.Position, components.Size, components.Rotation, components.Render, components.Health);
    ECS_SYSTEM(world, layoutGameStateSystem, EcsOnUpdate, components.Position, components.Size, components.Rotation, components.Owner, components.Index, components.Zone);

    return world;
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

    unsigned int backGroundVAO = setupBackGround();
    unsigned int backGroundProgram = linkShaders("shaders/backGroundVertex.glsl", "shaders/backGroundFragments.glsl");
    glUseProgram(backGroundProgram);
    unsigned int backGroundTexture = genTexture("textures/Board2.png");
    glUniform1i(glGetUniformLocation(backGroundProgram, "backGroundTexture"), 0);

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
