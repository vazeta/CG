#pragma once

#include "ofMain.h"
#include "materiais.h"

class ofApp : public ofBaseApp {

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void gameLose();
    void gameWin();
    void setOrthographic();
    void setPerspective();
    void setFirstPerson();
    void setupLights();
    void selectDifficulty();


    struct Rec {
        ofVec3f position;
        float width;
        float height;
        float depth;
        bool isActive;
        int points;
    };

    struct PowerUp {
        ofVec3f position;
        float size;
        ofVec3f velocity;
        bool isActive;
    };



    struct Player {
        ofVec3f position;
        float width;
        float height;
        float depth;
        float speed;
    };

    int score;
    int m;
    int n;
    float spacing;
    float gameOverTimer = 0.0f;
    float spotLightAngle = 45.0f;
    GLfloat RecWidth;
    GLfloat RecHeigth;
    GLfloat RecDept;
    GLfloat BlockWidth;
    GLfloat BlockHeight;
    GLfloat BlockDept;
    GLfloat BallRadius;
    GLfloat ambientColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseColor[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specularColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat offColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    ofVec3f BallPos;
    ofVec3f BallSpeed;
    std::vector<Rec> Recs;
    std::vector<PowerUp> powerUps;
    Player player;
    bool isGameLost = false;
    bool isGameWon = false;
    bool isWireframe = false;
    bool ambientOn = true;
    bool diffuseOn = true;
    bool specularOn = true;
    bool pointLightOn = true;
    bool directionalLightOn = true;
    bool spotLightOn = true;
    bool isHardMode = false;
    bool ambientOnPoint = true, diffuseOnPoint = true, specularOnPoint = true;
    bool ambientOnDirectional = true, diffuseOnDirectional = true, specularOnDirectional = true;
    bool ambientOnSpot = true, diffuseOnSpot = true, specularOnSpot = true;


    enum views { ORTHOGRAPHIC, PERSPECTIVE, FIRST_PERSON };
    views currentView = ORTHOGRAPHIC;

    ofImage blockTex;
    ofImage ballTex;
    ofImage playerTex;
    ofImage powerupTex;
    ofVideoPlayer backgroundVideo;


    float z;
    float y;
    float x;
    GLfloat pointAtC; // Atenuação constante (valor maior mantém mais intensa a luz em todas as distâncias)
    GLfloat pointAtL;  // Atenuação linear (menor que 1 mantém maior intensidade ao longo da distância)
    GLfloat pointAtQ; // Atenuação quadrática (menor valor significa que a luz não diminui tanto com a distância)

    //Todas as funções em baixo foram retiradas do trabalho feito nas aulas e reutilizadas aqui

    inline float gw() {
        return ofGetWidth();
    }

    inline float gh() {
        return ofGetHeight();
    }

    inline void sphere_unit(GLint m, GLint n, GLfloat size) {
        GLfloat d_theta = 2.0f * 3.1415926f / m;  // Incremental step for theta
        GLfloat d_phi = 3.1415926f / n;           // Incremental step for phi

        glBegin(GL_QUADS);
        for (int i = 0; i < n; ++i) {
            GLfloat phi0 = i * d_phi;
            GLfloat phi1 = (i + 1) * d_phi;

            for (int j = 0; j < m; ++j) {
                GLfloat theta0 = j * d_theta;
                GLfloat theta1 = (j + 1) * d_theta;

                // Coordinates for the four vertices of the quad
                // Vertex 0 (theta0, phi0)
                GLfloat x0 = size * sinf(phi0) * cosf(theta0);
                GLfloat y0 = size * cosf(phi0);
                GLfloat z0 = size * sinf(phi0) * sinf(theta0);
                GLfloat s0 = theta0 / (2.0f * 3.1415926f); // Texture coordinate s (u)
                GLfloat t0 = phi0 / 3.1415926f;            // Texture coordinate t (v)

                // Vertex 1 (theta1, phi0)
                GLfloat x1 = size * sinf(phi0) * cosf(theta1);
                GLfloat y1 = size * cosf(phi0);
                GLfloat z1 = size * sinf(phi0) * sinf(theta1);
                GLfloat s1 = theta1 / (2.0f * 3.1415926f);
                GLfloat t1 = phi0 / 3.1415926f;

                // Vertex 2 (theta1, phi1)
                GLfloat x2 = size * sinf(phi1) * cosf(theta1);
                GLfloat y2 = size * cosf(phi1);
                GLfloat z2 = size * sinf(phi1) * sinf(theta1);
                GLfloat s2 = theta1 / (2.0f * 3.1415926f);
                GLfloat t2 = phi1 / 3.1415926f;

                // Vertex 3 (theta0, phi1)
                GLfloat x3 = size * sinf(phi1) * cosf(theta0);
                GLfloat y3 = size * cosf(phi1);
                GLfloat z3 = size * sinf(phi1) * sinf(theta0);
                GLfloat s3 = theta0 / (2.0f * 3.1415926f);
                GLfloat t3 = phi1 / 3.1415926f;

                // Specify normals, texture coordinates, and vertices for the quad
                // Vertex 0
                glNormal3f(x0 / size, y0 / size, z0 / size);
                glTexCoord2f(s0, t0);
                glVertex3f(x0, y0, z0);

                // Vertex 1
                glNormal3f(x1 / size, y1 / size, z1 / size);
                glTexCoord2f(s1, t1);
                glVertex3f(x1, y1, z1);

                // Vertex 2
                glNormal3f(x2 / size, y2 / size, z2 / size);
                glTexCoord2f(s2, t2);
                glVertex3f(x2, y2, z2);

                // Vertex 3
                glNormal3f(x3 / size, y3 / size, z3 / size);
                glTexCoord2f(s3, t3);
                glVertex3f(x3, y3, z3);
            }
        }
        glEnd();
    }


    inline void cube_unit() {
        float p = 0.5;

        glBegin(GL_QUADS);
        // Frente
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-p, -p, p);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-p, p, p);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(p, p, p);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(p, -p, p);

        // Trás
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-p, -p, -p);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(p, -p, -p);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(p, p, -p);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-p, p, -p);

        // Cima
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-p, -p, -p);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-p, -p, p);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(p, -p, p);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(p, -p, -p);

        // Baixo
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-p, p, -p);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(p, p, -p);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(p, p, p);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-p, p, p);

        // Esquerda
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-p, -p, -p);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-p, p, -p);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-p, p, p);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-p, -p, p);

        // Direita
        glTexCoord2f(0.0f, 0.0f); glVertex3f(p, -p, p);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(p, p, p);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(p, p, -p);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(p, -p, -p);

        glEnd();
    }



    inline ofVec3f cross(ofVec3f A, ofVec3f B) {
        ofVec3f aux;
        aux.x = A.y * B.z - A.z * B.y;
        aux.y = A.z * B.x - A.x * B.z;
        aux.z = A.x * B.y - A.y * B.x;

        return aux;
    }

    inline void perspective(GLfloat theta, GLfloat alpha, GLfloat beta) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        GLfloat tan = tanf(theta * 0.5 * PI / 180.0);
        GLfloat d = (gh() / 2.0) / tan;
        GLfloat nearClip = d / alpha;
        GLfloat farClip = d * beta;
        GLfloat ymax = nearClip * tan;
        GLfloat xmax = (gw() / gh()) * ymax;
        glFrustum(-xmax, xmax, -ymax, ymax, nearClip, farClip);
    }


    //implementa o algoritmo de lookAt
    inline void lookat(
        GLfloat camX,
        GLfloat camY,
        GLfloat camZ,
        GLfloat targetX,
        GLfloat targetY,
        GLfloat targetZ,
        GLfloat upX,
        GLfloat upY,
        GLfloat upZ)
    {
        ofVec3f cam = ofVec3f(camX, camY, camZ);
        ofVec3f target = ofVec3f(targetX, targetY, targetZ);
        ofVec3f up = ofVec3f(upX, upY, upZ);


        ofVec3f N = cam - target;
        N = N.normalized();
        ofVec3f U = cross(up, N);
        U = U.normalized();
        ofVec3f V = cross(N, U);
        V = V.normalized();

        /*GLfloat camTransformMatrix[4][4] = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        };*/

        GLfloat camTransformMatrix[4][4] = {
            {U.x, V.x, N.x, 0},
            {U.y, V.y, N.y, 0},
            {U.z, V.z, N.z, 0},
            {-U.dot(cam), -V.dot(cam), -N.dot(cam), 1}
        };

        /*camTransformMatrix[0][0] = U.x;
        camTransformMatrix[1][0] = U.y;
        camTransformMatrix[2][0] = U.z;
        camTransformMatrix[0][1] = V.x;
        camTransformMatrix[1][1] = V.y;
        camTransformMatrix[2][1] = V.z;
        camTransformMatrix[0][2] = N.x;
        camTransformMatrix[1][2] = N.y;
        camTransformMatrix[2][2] = N.z;
        camTransformMatrix[3][0] = -U.dot(cam);
        camTransformMatrix[3][1] = -V.dot(cam);
        camTransformMatrix[3][2] = -N.dot(cam);*/


        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glMultMatrixf(&camTransformMatrix[0][0]);
        //glTranslatef(-cam.x, -cam.y, -cam.z);

    }


};
