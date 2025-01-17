#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofDisableArbTex();
    BlockWidth = gw();
    BlockHeight = gh() / 3;
    BlockDept = 20;

    score = 0;
    m = 10;
    n = 4;
    spacing = 7.5;

    setupLights();

    RecWidth = (BlockWidth - ((m - 1) * spacing)) / m;
    RecHeigth = (BlockHeight - ((n - 1) * spacing)) / n;
    RecDept = BlockDept;
    BallRadius = 17.0;//inicialmente a 10 na meta 1
    BallPos = ofVec3f(gw() / 2, gh() - 100, 0);
    BallSpeed = ofVec3f(3, -5, 0);

    player.width = gw() / 7;
    player.height = 30.0f;//inicialmente a 20 na meta 1
    player.depth = BlockDept;
    player.position = ofVec3f(gw() / 2, gh() - 2 * player.height, 0);
    player.speed = gw() / 7;

    blockTex.load("img1.jpg");
    ballTex.load("imgb.jpg");
    playerTex.load("imgp1.jpg");
    powerupTex.load("imgpu.jpg");
    backgroundVideo.load("background.mp4");
    backgroundVideo.setLoopState(OF_LOOP_NORMAL);
    backgroundVideo.play();

    z = gh() * 0.25 * (cos(0) * PI / 180.) * 0.5 + 0.45;
    y = gh() * 0.5;
    x = 0;



    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Rec rec;
            rec.position = ofVec3f(j * (RecWidth + spacing) + RecWidth / 2,
                i * (RecHeigth + spacing) + RecHeigth / 2,
                0);
            rec.width = RecWidth;
            rec.height = RecHeigth;
            rec.depth = RecDept;
            rec.isActive = true;
            rec.points = (n - i) * 10;
            Recs.push_back(rec);
        }
    }
    selectDifficulty();
}

//Uso de chatgpt nesta função
void ofApp::selectDifficulty() {
    int choice = 0;

    while (choice != 1 && choice != 2) {
        std::cout << "=============================================\n";
        std::cout << "              Select Difficulty              \n";
        std::cout << "=============================================\n";
        std::cout << "   1. Easy                                  \n";
        std::cout << "      - Power-ups enabled:                  \n";
        std::cout << "      - Bigger paddle                       \n";
        std::cout << "      - Lower ball velocity                 \n";
        std::cout << "                                            \n";
        std::cout << "   2. Hard                                  \n";
        std::cout << "      - Power-ups disabled                  \n";
        std::cout << "=============================================\n";
        std::cout << "  Enter your choice (1 or 2) and press ENTER:\n";
        std::cout << "=============================================\n";

        
        std::cin >> choice;

       
        if (std::cin.fail()) {
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = 0;
        }
    }

    
    if (choice == 1) {
        isHardMode = false;
        std::cout << "Easy mode selected. Power-ups are enabled.\n";
    }
    else if (choice == 2) {
        isHardMode = true;
        std::cout << "Hard mode selected. Power-ups are disabled.\n";
    }

    
    std::cout << "Press ENTER to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}



void ofApp::setupLights() {
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    // Configuração para a luz pontual
    GLfloat pointLightPos[] = { gw() / 2, gh() - 2 * player.height, 100.0f, 1.0f };
    GLfloat pointAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat pointDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat pointSpecular[] = { 0.4f, 0.4f, 0.4f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, pointLightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, pointAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, pointDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, pointSpecular);
    glEnable(GL_LIGHT0);

    pointAtC = 1; // Atenuação constante (valor maior mantém mais intensa a luz em todas as distâncias)
    pointAtL = 0.0001; // Atenuação linear (menor que 1 mantém maior intensidade ao longo da distância)
    pointAtQ = 0.00002; // Atenuação quadrática (menor valor significa que a luz não diminui tanto com a distância)
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, pointAtC);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, pointAtL);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, pointAtQ);

    // Configuração para a luz direcional
    GLfloat directionalLightDir[] = { x, y, z, 0.0f };
    GLfloat directionalAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat directionalDiffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat directionalSpecular[] = { 0.2f, 0.2f, 0.2f, 1.0f };


    glLightfv(GL_LIGHT1, GL_POSITION, directionalLightDir);
    glLightfv(GL_LIGHT1, GL_AMBIENT, directionalAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, directionalDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, directionalSpecular);
    glEnable(GL_LIGHT1);

    // Configuração para a luz tipo foco
    GLfloat spotLightPos[] = { BallPos.x, BallPos.y, BallPos.z };
    GLfloat spotDirection[] = { gw() / 2, gh() / 2, -1.0f };
    GLfloat spotAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat spotDiffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat spotSpecular[] = { 0.6f, 0.6f, 0.6f, 1.0f };

    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0f);


    glLightfv(GL_LIGHT2, GL_POSITION, spotLightPos);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDirection);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0f);
    glLightfv(GL_LIGHT2, GL_AMBIENT, spotAmbient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, spotDiffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, spotSpecular);
    glEnable(GL_LIGHT2);
}


//--------------------------------------------------------------
void ofApp::gameWin() {
    isGameWon = true;
    gameOverTimer = 0.0f;
}

//--------------------------------------------------------------
void ofApp::gameLose() {
    isGameLost = true;
    gameOverTimer = 0.0f;
}

//--------------------------------------------------------------
bool isColliding(const ofVec3f& ballPos, float ballRadius, const ofApp::Rec& rec) {
    float halfWidth = rec.width / 2.0f;
    float halfHeight = rec.height / 2.0f;
    float halfDepth = rec.depth / 2.0f;

    return rec.isActive &&
        abs(ballPos.x - rec.position.x) <= (halfWidth + ballRadius) &&
        abs(ballPos.y - rec.position.y) <= (halfHeight + ballRadius) &&
        abs(ballPos.z - rec.position.z) <= (halfDepth + ballRadius);
}

//--------------------------------------------------------------
bool isCollidingPlayer(const ofVec3f& ballPos, float ballRadius, const ofApp::Player& player) {
    float halfWidth = player.width / 2.0f;
    float halfHeight = player.height / 2.0f;

    return abs(ballPos.x - player.position.x) <= (halfWidth + ballRadius) &&
        abs(ballPos.y - player.position.y) <= (halfHeight + ballRadius);
}

//--------------------------------------------------------------
void ofApp::update() {
    backgroundVideo.update();
    if (isGameLost || isGameWon) {
        gameOverTimer += ofGetLastFrameTime();
        if (gameOverTimer > 4.0f) {
            isGameLost = false;
            isGameWon = false;

            BallPos = ofVec3f(gw() / 2, gh() - 100, 0);
            BallSpeed = ofVec3f(3, -5, 0);

            for (Rec& rec : Recs) {
                rec.isActive = true;
            }

            powerUps.clear();//uso de chatgpt
            player.position.x = gw() / 2;
            score = 0;
        }
    }
    else {
        GLfloat pointLightPos[] = { BallPos.x, BallPos.y, BallPos.z + 50.0f, 1.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, pointLightPos);

        GLfloat spotLightPos[] = { BallPos.x, BallPos.y, 15.0f, 1.0f };
        GLfloat spotDirection[] = { 0.0f, -1.0f, 0.0f };

        glLightfv(GL_LIGHT2, GL_POSITION, spotLightPos);
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDirection);

        BallPos += BallSpeed;

        for (PowerUp& powerUp : powerUps) {
            if (!powerUp.isActive)
                continue;

            powerUp.position += powerUp.velocity;

            if (powerUp.position.y - powerUp.size > gh()) {
                powerUp.isActive = false;
            }
            else if (abs(powerUp.position.x - player.position.x) <= player.width / 2 &&
                abs(powerUp.position.y - player.position.y) <= player.height / 2) {
                powerUp.isActive = false;
                if (player.width < gw() / 4) {
                    player.width *= 1.1;
                }
                BallSpeed *= 0.98;
                break;
            }
        }

        for (Rec& rec : Recs) {
            if (isColliding(BallPos, BallRadius, rec)) {
                rec.isActive = false;

                if (abs(BallPos.x - rec.position.x) > rec.width / 2) {
                    BallSpeed.x *= -1;
                }
                if (abs(BallPos.y - rec.position.y) > rec.height / 2) {
                    BallSpeed.y *= -1;
                }
                score += rec.points;

                if (ofRandom(1) < 0.3 && !isHardMode) {
                    PowerUp powerUp;
                    powerUp.position = rec.position;
                    powerUp.size = 15.0f;
                    powerUp.velocity = ofVec3f(0, 2.0f, 0);
                    powerUp.isActive = true;
                    powerUps.push_back(powerUp);
                }
                break;
            }
        }

        bool allInactive = true;
        for (const Rec& rec : Recs) {
            if (rec.isActive) {
                allInactive = false;
                break;
            }
        }
        if (allInactive) {
            gameWin();
        }

        if (isCollidingPlayer(BallPos, BallRadius, player)) {
            BallSpeed.y *= -1;
            BallPos.y = player.position.y - (BallRadius + player.height / 2);
        }

        if (BallPos.x - BallRadius < 0 || BallPos.x + BallRadius > gw()) {
            BallSpeed.x *= -1;
        }
        if (BallPos.y - BallRadius < 0) {
            BallSpeed.y *= -1;
        }
        if (BallPos.y + BallRadius > gh()) {
            gameLose();
        }
    }
}

//--------------------------------------------------------------
void ofApp::setOrthographic() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, gw(), gh(), 0, -1000, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//--------------------------------------------------------------
void ofApp::setPerspective() {
    perspective(70.0f, 1.0f, 1000.0f);

    lookat(
        gw() - gw() / 6, gh() / 2, -1200,
        gw() / 2, gh() / 2, 0,
        0, -1, 0
    );
}

//--------------------------------------------------------------
void ofApp::setFirstPerson() {
    ofVec3f camera = ofVec3f(gw() / 2, gh() + gh(), 200);
    lookat(
        camera.x, camera.y, camera.z,
        BallPos.x, BallPos.y, player.position.z,
        0, 0, -1
    );
}

//--------------------------------------------------------------
void ofApp::draw() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    ofSetColor(255, 255, 255);
    backgroundVideo.draw(0, 0, gw(), gh());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    if (isGameLost) {
        if (static_cast<int>(gameOverTimer * 2) % 2 == 0) {
            ofBackground(0, 0, 0);
        }
        else {
            ofBackground(44, 44, 44);
        }
        ofSetColor(255, 255, 255);
        ofDrawBitmapString("Restarting in 4 seconds...", gw() / 2 - 120, gh() / 2 - 30);
        ofDrawBitmapString("YOU LOST! Score: " + ofToString(score), gw() / 2 - 100, gh() / 2);
        ofDrawBitmapString("Press ESC to exit", gw() / 2 - 80, gh() / 2 + 30);
    }
    else if (isGameWon) {
        ofBackground(0, 51, 0);
        ofSetColor(255, 255, 255);
        ofDrawBitmapString("CONGRATULATIONS! YOU WON! Score: " + ofToString(score), gw() / 2 - 150, gh() / 2 - 30);
        ofDrawBitmapString("Restarting in 4 seconds...", gw() / 2 - 100, gh() / 2);
        ofDrawBitmapString("Press ESC to exit", gw() / 2 - 80, gh() / 2 + 30);
    }
    else {
        switch (currentView) {
        case ORTHOGRAPHIC:
            setOrthographic();
            break;
        case PERSPECTIVE:
            setPerspective();
            break;
        case FIRST_PERSON:
            setFirstPerson();
            break;
        }

        loadMaterial(16);//white plastic
        glEnable(GL_TEXTURE);
        blockTex.bind();

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_T, GL_CLAMP);

        for (const Rec& rec : Recs) {
            if (!rec.isActive) continue;

            glPushMatrix();
            glTranslatef(rec.position.x, rec.position.y, rec.position.z);
            glScalef(rec.width, rec.height, rec.depth);

            cube_unit();
            glPopMatrix();
        }

        blockTex.unbind();
        glDisable(GL_TEXTURE);


        loadMaterial(14);//green plastic
        glEnable(GL_TEXTURE);
        powerupTex.bind();

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_T, GL_CLAMP);

        for (PowerUp& powerUp : powerUps) {
            if (!powerUp.isActive) continue;

            glPushMatrix();
            glTranslatef(powerUp.position.x, powerUp.position.y, powerUp.position.z);
            sphere_unit(10, 10, powerUp.size);
            glPopMatrix();
        }

        powerupTex.unbind();
        glDisable(GL_TEXTURE);

        loadMaterial(10);//gold
        glEnable(GL_TEXTURE);
        ballTex.bind();

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glPushMatrix();
        glTranslatef(BallPos.x, BallPos.y, BallPos.z);
        sphere_unit(12, 12, BallRadius);
        glPopMatrix();

        ballTex.unbind();
        glDisable(GL_TEXTURE);

        loadMaterial(6);//brass
        glEnable(GL_TEXTURE);
        playerTex.bind();

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glPushMatrix();
        glTranslatef(player.position.x, player.position.y, player.position.z);
        glScalef(player.width, player.height, player.depth);
        cube_unit();
        glPopMatrix();

        playerTex.unbind();
        glDisable(GL_TEXTURE);
        glDisable(GL_LIGHTING);
        loadMaterial(16);

        ofSetColor(255, 255, 255);
        ofDrawBitmapString("Score: " + ofToString(score), gw() - 90, gh() - 20);
    }
}

//---------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
    case '1':
        currentView = ORTHOGRAPHIC;
        break;
    case '2':
        currentView = PERSPECTIVE;
        break;
    case '3':
        currentView = FIRST_PERSON;
        break;
    case 'a':
    case OF_KEY_LEFT:
        player.position.x -= player.speed;
        if (player.position.x - player.width / 2 < 0) {
            player.position.x = player.width / 2;
        }
        break;
    case 'd':
    case OF_KEY_RIGHT:
        player.position.x += player.speed;
        if (player.position.x + player.width / 2 > gw()) {
            player.position.x = gw() - player.width / 2;
        }
        break;

    // Ativar/desativar luz pontual
    case '4':
        if (pointLightOn) glDisable(GL_LIGHT0);
        else glEnable(GL_LIGHT0);
        pointLightOn = !pointLightOn;
        break;

    // Ativar/desativar luz direcional
    case '5':
        if (directionalLightOn) glDisable(GL_LIGHT1);
        else glEnable(GL_LIGHT1);
        directionalLightOn = !directionalLightOn;
        break;

    // Ativar/desativar luz tipo foco
    case '6':
        if (spotLightOn) glDisable(GL_LIGHT2);
        else glEnable(GL_LIGHT2);
        spotLightOn = !spotLightOn;
        break;

    // Controlo de cada componente da luz pontual
    case 'q': // Ambiente
        ambientOnPoint = !ambientOnPoint;
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientOnPoint ? ambientColor : offColor);
        break;
    case 'w': // Difusa
        diffuseOnPoint = !diffuseOnPoint;
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseOnPoint ? diffuseColor : offColor);
        break;
    case 'e': // Especular
        specularOnPoint = !specularOnPoint;
        glLightfv(GL_LIGHT0, GL_SPECULAR, specularOnPoint ? specularColor : offColor);
        break;

    // Controlo de cada componente da luz direcional 
    case 'r': // Ambiente
        ambientOnDirectional = !ambientOnDirectional;
        glLightfv(GL_LIGHT1, GL_AMBIENT, ambientOnDirectional ? ambientColor : offColor);
        break;
    case 't': // Difusa
        diffuseOnDirectional = !diffuseOnDirectional;
        glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseOnDirectional ? diffuseColor : offColor);
        break;
    case 'y': // Especular
        specularOnDirectional = !specularOnDirectional;
        glLightfv(GL_LIGHT1, GL_SPECULAR, specularOnDirectional ? specularColor : offColor);
        break;

    // Controlo de cada componente da luz foco 
    case 'u': // Ambiente
        ambientOnSpot = !ambientOnSpot;
        glLightfv(GL_LIGHT2, GL_AMBIENT, ambientOnSpot ? ambientColor : offColor);
        break;
    case 'i': // Difusa
        diffuseOnSpot = !diffuseOnSpot;
        glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseOnSpot ? diffuseColor : offColor);
        break;
    case 'o': // Especular
        specularOnSpot = !specularOnSpot;
        glLightfv(GL_LIGHT2, GL_SPECULAR, specularOnSpot ? specularColor : offColor);
        break;
    }
}