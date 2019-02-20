// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "glarea.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>
#include <QtMath>

static const QString vertexShaderFile   = ":/basic.vsh";
static const QString fragmentShaderFile = ":/basic.fsh";
float up=0;
float down=0;
float lefte=0;
float righte=0;
float back=0;

GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{
    qDebug() << "init GLArea" ;

    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);  // nb de sample par pixels : suréchantillonnage por l'antialiasing, en décalant à chaque fois le sommet
                        // cf https://www.khronos.org/opengl/wiki/Multisampling et https://stackoverflow.com/a/14474260
    setFormat(sf);
    qDebug() << "Depth is"<< format().depthBufferSize();

    setEnabled(true);  // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus); // accepte focus
    setFocus();                      // donne le focus

    m_timer = new QTimer(this);
    m_timer->setInterval(50);  // msec
    connect (m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect (this, SIGNAL(radiusChanged(double)), this, SLOT(setRadius(double)));
}

GLArea::~GLArea()
{
    qDebug() << "destroy GLArea";

    delete m_timer;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();
    tearGLObjects();
    doneCurrent();
}


void GLArea::initializeGL()
{
    qDebug() << __FUNCTION__ ;
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    makeGLObjects();

    // shaders
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile);  // compile
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile);
    if (! m_program->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << m_program->log();
    }

    // récupère identifiants de "variables" dans les shaders
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    //m_matrixUniform = m_program->uniformLocation("matrix");
    GLfloat Light0Amb[4] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat Light0Dif[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat Light0Spec[4]= {0.2f, 0.2f, 0.2f, 1.0f};
    // Valeur de position (ici, source ponctuelle)
    GLfloat Light0Pos[4] = {lefte,up, righte,back};
    // Fixe les paramètres de couleur de la lumière 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, Light0Amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Light0Dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Light0Spec);
    // Fixe la position de la lumière 0
    glLightfv(GL_LIGHT0, GL_POSITION, Light0Pos);
    // Active l’éclairage
    glEnable(GL_LIGHTING);
    // Active la lumière 0
    glEnable(GL_LIGHT0);
}


void GLArea::makeGLObjects()
{
    /*GLfloat vertices[] = {
       -0.7, -0.5, -0.1,
        0.8, -0.2, -0.1,
        0.1,  0.9,  0.3,
       -0.6,  0.7, -0.2,
        0.8,  0.8, -0.2,
        0.1, -0.9,  0.7
    };

    GLfloat colors[] = {
        1.0, 0.6, 0.6,
        1.0, 0.6, 0.6,
        1.0, 0.6, 0.6,
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    };*/
    m_cylindre = new Cylindre(4,6,8,1,0,0);
    m_cylindre->dessiner();
    /*QMatrix4x4 matrix;
    //matrix.translate(0, 0, 4/2);
    for (int i= 0; i < 8; i++)
    {

        //QMatrix4x4 matrix;

        float angle = qDegreesToRadians(360.0f * (double)i / 8.0);
        qDebug()<<"coucou arnaud"<<angle<<" je suis l'angle";
        //glColor3f (this->r, this->g, this->b);
        matrix.rotate(angle, 0, 1, 0);
        m_cylindre->dessiner_face(&matrix, angle);



    }*/
    qDebug()<<"AAAAAAAAAAAAAAAAAAAAAAAAAAA : "<<m_cylindre->vertData.size();
    //m_vbo = m_cylindre->return_vbo();
    /*QVector<GLfloat> vertData;
    for (int i = 0; i < 6; ++i) {
        // coordonnées sommets
        for (int j = 0; j < 3; j++)
            vertData.append(vertices[i*3+j]);
        // couleurs sommets
        for (int j = 0; j < 3; j++)
            vertData.append(colors[i*3+j]);
    }
    */
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_cylindre->vertData.constData(), m_cylindre->vertData.count() * sizeof(GLfloat));
}


void GLArea::tearGLObjects()
{
    m_vbo.destroy();
}


void GLArea::resizeGL(int w, int h)
{
    qDebug() << __FUNCTION__ << w << h;

    // C'est fait par défaut
    glViewport(0, 0, w, h);

    m_ratio = (double) w / h;
    // doProjection();
}

void GLArea::paintGL()
{
    qDebug() << __FUNCTION__ ;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind(); // active le shader program

    QMatrix4x4 matrix;
    GLfloat hr = m_radius, wr = hr * m_ratio;
    matrix.frustum(-wr, wr, -hr, hr, 1.0, 5.0);
    matrix.translate(0, 0, -3.0);

    // Rotation de la scène pour l'animation
    matrix.rotate(m_angle, 0, 1, 0);

    m_program->setUniformValue("matrix", matrix);

    m_program->setAttributeBuffer("posAttr",
        GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));
    m_program->setAttributeBuffer("colAttr",
        GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    m_program->enableAttributeArray("posAttr");
    m_program->enableAttributeArray("colAttr");
    //m_cylindre->dessiner();
    //m_vbo = m_cylindre->return_vbo();
    glDrawArrays(GL_LINE_LOOP, 0, m_cylindre->vertData.size());
    //glDrawArrays(GL_TRIANGLES, 0, 6);

    m_program->disableAttributeArray("posAttr");
    m_program->disableAttributeArray("colAttr");

    m_program->release();
}

void GLArea::keyPressEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();

    switch(ev->key()) {
        case Qt::Key_Space :
            m_angle += 1;
            if (m_angle >= 360) m_angle -= 360;
            update();
            break;
        case Qt::Key_A :
            if (m_timer->isActive())
                m_timer->stop();
            else m_timer->start();
            break;
        case Qt::Key_R :
            if (ev->text() == "r")
                 setRadius(m_radius-0.05);
            else setRadius(m_radius+0.05);
            break;
        case Qt::Key_Z:
            up=up+0.1;
        break;
        case Qt::Key_S:
            up=up-0.1;
        break;
        case Qt::Key_Q:
            lefte=lefte+0.1;
        break;
        case Qt::Key_D:
            lefte=lefte-1;
        break;
        case Qt::Key_W:
            back=back+0.1;
        break;
        case Qt::Key_X:
            back=back-0.1;
        break;
        case Qt::Key_P:
            up=0;
            down=0;
            lefte=0;
            righte=0;
            back=0;
        break;
    }
}

void GLArea::keyReleaseEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();
}

void GLArea::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void GLArea::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void GLArea::mouseMoveEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y();
}

void GLArea::onTimeout()
{
    qDebug() << __FUNCTION__ ;
    m_anim += 0.01;
    if (m_anim > 1) m_anim = 0;
    update();
}

void GLArea::setRadius(double radius)
{
    qDebug() << __FUNCTION__ << radius << sender();
    if (radius != m_radius && radius > 0.01 && radius <= 10) {
        m_radius = radius;
        qDebug() << "  emit radiusChanged()";
        emit radiusChanged(radius);
        update();
    }
}




